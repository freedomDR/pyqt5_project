#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_pdm_api.h"
#include "j2735r63_common.h"

#include <libcli.h>
#include <craton/cli.h>
#include <atlk/nav_service.h>

#include <dot3_common.h>
#include <dot3_api.h>

#define SHOW_ENCODE 1
#define SHOW_DECODED 1
#define ENCODE_PRINT_LEVEL 1
#define DECODE_PRINT_LEVEL 1
#define MAX_LOOP_COUNT 1

static int stop_pdm = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_pdm_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_pdm_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_pdm_func(ULONG input)
{
	(void)input;
	stop_pdm = 1;
	return;
}

void _cmd_pdm_free_struct(pdm_standard_item *free_pdm_item)
{
	if(free_pdm_item == NULL){
		printf("[PDM] Error: free_pdm_struct() input is NULL\n");
	}else{
		if(free_pdm_item->data_elements!= NULL){ 		
			j2735_pdm_free(free_pdm_item->data_elements);
		}
		j2735_pdm_free(free_pdm_item);
	}
}

void _show_pdm_standard_item(pdm_standard_item *decoded_item)
{
	int i;

	printf("==========PDM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(2) sample*/
	printf("sample.sample_start = %d\n", decoded_item->sample.sample_start);
	printf("sample.sample_end = %d\n", decoded_item->sample.sample_end);
	/*(3) directions*/
	printf("directions = %d\n", decoded_item->directions);
	/*(4) term CHOICE: TERM TIME*/
	if(decoded_item->term_choice == PDM_TERM_TERM_TIME)
	{
		printf("term_choice = TERM_TIME\n");
		printf("term_time = %d\n", decoded_item->term_time);
	}
	else if(decoded_item->term_choice == PDM_TERM_TERM_DISTANCE)
	{
		printf("term_choice = TERM_DISTANCE\n");
		printf("term_distance = %d\n", decoded_item->term_distance);
	}
	/*(5) snapshot CHOICE: SNAPSHOT TIME*/
	if(decoded_item->snapshot_choice == PDM_SNAPSHOT_SNAPSHOT_TIME)
	{
		printf("snapshot_choice = SNAPSHOT_TIME\n");
		printf("snapshot_time.speed1 = %d\n", decoded_item->snapshot_time.speed1);
		printf("snapshot_time.time1  = %d\n", decoded_item->snapshot_time.time1);
		printf("snapshot_time.speed2 = %d\n", decoded_item->snapshot_time.speed2);
		printf("snapshot_time.time2  = %d\n", decoded_item->snapshot_time.time2);
	}
	else if(decoded_item->snapshot_choice == PDM_SNAPSHOT_SNAPSHOT_DISTANCE)
	{
		printf("snapshot_choice = SNAPSHOT_DISTANCE\n");
		printf("snapshot_distance.distance1 = %d\n", decoded_item->snapshot_distance.distance1);
		printf("snapshot_distance.speed1 = %d\n", decoded_item->snapshot_distance.speed1);
		printf("snapshot_distance.distance2 = %d\n", decoded_item->snapshot_distance.distance2);
		printf("snapshot_distance.speed2 = %d\n", decoded_item->snapshot_distance.speed2);
	}
	/*(6) txInterval*/
	printf("tx_interval = %d\n", decoded_item->tx_interval);
	
	/*(7) requests*/
	if(decoded_item->mask_data_elements == 1){
		for(i = 0; i < decoded_item->data_elements_num; i++)
		{
			printf("\ndata_elements[%d].dataType = %d\n", i, decoded_item->data_elements[i].dataType);
			if(decoded_item->data_elements[i].mask_subType == 1)
				printf("data_elements[%d].subType = %d\n", i, decoded_item->data_elements[i].subType);
			if(decoded_item->data_elements[i].mask_sendOnLessThenValue == 1)
				printf("data_elements[%d].sendOnLessThenValue = %d\n", i, decoded_item->data_elements[i].sendOnLessThenValue);
			if(decoded_item->data_elements[i].mask_sendOnMoreThenValue == 1)
				printf("data_elements[%d].sendOnMoreThenValue = %d\n", i, decoded_item->data_elements[i].sendOnMoreThenValue);
			if(decoded_item->data_elements[i].mask_sendAll == 1)
				printf("data_elements[%d].sendAll = %d\n", i, decoded_item->data_elements[i].sendAll);
		}	
	}
	printf("===============================\n");
}

int cmd_rxPdm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	pdm_standard_item *decoded_item = NULL;
	int err_pdm;

	int err, ret;
	TX_TIMER stop_pdm_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxPdm <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxPdm 123\n");
		return 1;
	}
	psid = atoi(argv[0]);

	err = wme_init(&handle);
	if(err != 0){
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);

	/* Add wsm service */
	memset(&wsm_info, 0, sizeof(wsm_info));
	
	wsm_info.psid = psid;
	ret = wme_wsm_serv_create(&handle, &serv_index, &wsm_info);
	if(ret < 0) {
		printf("wme_wsm_service = %d\n", ret);
		return 0;
	}
	printf("wme_wsm_service success\n");

	printf("waiting 5s to recv wsm packets\n");
	stop_pdm = 0;
	tx_timer_create(&stop_pdm_timer, NULL, stop_pdm_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_pdm == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (pdm_standard_item *)j2735_pdm_calloc(1, sizeof(pdm_standard_item));
		err_pdm = j2735r63_pdm_decode(decoded_item, in_wsm.data, MAX_PDM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_pdm < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_pdm_standard_item(decoded_item);}
    
		j2735r63_pdm_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_pdm_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_pdm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	pdm_standard_item *encode_item = NULL;
	pdm_standard_item *decoded_item = NULL;
	int err_pdm, i, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 PDM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_pdm_calloc(1, MAX_PDM_BUFF_SIZE);
		encode_item = (pdm_standard_item *)j2735_pdm_calloc(1, sizeof(pdm_standard_item));

		/*(1) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;		
		/*(2) sample*/
		encode_item->sample.sample_start = 255;
		encode_item->sample.sample_end = 255;
		/*(3) directions*/
		encode_item->directions = 65534;
		/*(4) term CHOICE: TERM TIME*/
		encode_item->term_choice = PDM_TERM_TERM_TIME;
		encode_item->term_time = 1800;
		/*(4) term CHOICE: TERM DISTANCE
		encode_item->term_choice = PDM_TERM_TERM_DISTANCE;
		encode_item->term_distance = 30000;*/
		/*(5) snapshot CHOICE: SNAPSHOT TIME*/
		encode_item->snapshot_choice = PDM_SNAPSHOT_SNAPSHOT_TIME;
		encode_item->snapshot_time.speed1 = 31;
		encode_item->snapshot_time.time1  = 61;
		encode_item->snapshot_time.speed2 = 31;
		encode_item->snapshot_time.time2  = 61;
		/*(5) snapshot CHOICE: SNAPSHOT DISTANCE
		encode_item->snapshot_choice = PDM_SNAPSHOT_SNAPSHOT_DISTANCE;
		encode_item->snapshot_distance.distance1 = 1023;
		encode_item->snapshot_distance.speed1 = 31;
		encode_item->snapshot_distance.distance2 = 1023;
		encode_item->snapshot_distance.speed2 = 31;*/
		
		/*(6) txInterval*/
		encode_item->tx_interval = 61;
		
		/*(7) dataElements*/
		encode_item->mask_data_elements = 1;

		encode_item->data_elements_num = 1;
		encode_item->data_elements = (vehicle_status_request_item *)j2735_pdm_calloc(encode_item->data_elements_num, sizeof(vehicle_status_request_item));
		if(!(encode_item->data_elements))
		{
			printf("[PDM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else
		{
			for(i = 0; i < encode_item->data_elements_num; i++)
			{
				encode_item->data_elements[i].dataType = 28;
				encode_item->data_elements[i].mask_subType = 1;
				encode_item->data_elements[i].subType = 15;
				encode_item->data_elements[i].mask_sendOnLessThenValue = 1;
				encode_item->data_elements[i].sendOnLessThenValue = -32767;
				encode_item->data_elements[i].mask_sendOnMoreThenValue = 1;
				encode_item->data_elements[i].sendOnMoreThenValue = 32767;
				encode_item->data_elements[i].mask_sendAll = 1;
				encode_item->data_elements[i].sendAll = 1;
			}			
		}
		
		encode_item->mask_regional = 0;		
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("PDM loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_pdm_standard_item(&encode_item);}

		err_pdm = j2735r63_pdm_encode(encode_item, encodeBuf, MAX_PDM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_pdm < 0)
		{
			printf("pdm encode fail...\n");		
			return -1;	
		}
		_cmd_pdm_free_struct(encode_item);

		decoded_item = (pdm_standard_item *)j2735_pdm_calloc(1, sizeof(pdm_standard_item));
		err_pdm = j2735r63_pdm_decode(decoded_item, encodeBuf, MAX_PDM_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_pdm < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_pdm_standard_item(decoded_item);}
		
		j2735r63_pdm_free_struct(decoded_item);
		j2735_pdm_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 PDM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txPdm(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, err_pdm;
	pdm_standard_item *encode_item = NULL;

	encode_item = (pdm_standard_item *)j2735_pdm_calloc(1, sizeof(pdm_standard_item));
	
	/*(1) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;		
	/*(2) sample*/
	encode_item->sample.sample_start = 255;
	encode_item->sample.sample_end = 255;
	/*(3) directions*/
	encode_item->directions = 1;
	/*(4) term CHOICE: TERM TIME*/
	encode_item->term_choice = PDM_TERM_TERM_TIME;
	encode_item->term_time = 1800;
	/*(4) term CHOICE: TERM DISTANCE
	encode_item->term_choice = PDM_TERM_TERM_DISTANCE;
	encode_item->term_distance = 30000;*/
	/*(5) snapshot CHOICE: SNAPSHOT TIME
	encode_item->snapshot_choice = PDM_SNAPSHOT_SNAPSHOT_TIME;
	encode_item->snapshot_time.speed1 = 31;
	encode_item->snapshot_time.time1  = 61;
	encode_item->snapshot_time.speed2 = 31;
	encode_item->snapshot_time.time2  = 61*/;
	/*(5) snapshot CHOICE: SNAPSHOT DISTANCE*/
	encode_item->snapshot_choice = PDM_SNAPSHOT_SNAPSHOT_DISTANCE;
	encode_item->snapshot_distance.distance1 = 1023;
	encode_item->snapshot_distance.speed1 = 31;
	encode_item->snapshot_distance.distance2 = 1023;
	encode_item->snapshot_distance.speed2 = 31;
	
	/*(6) txInterval*/
	encode_item->tx_interval = 61;
	
	/*(7) dataElements*/
	encode_item->mask_data_elements = 1;

	encode_item->data_elements_num = 1;
	encode_item->data_elements = (vehicle_status_request_item *)j2735_pdm_calloc(encode_item->data_elements_num, sizeof(vehicle_status_request_item));
	if(!(encode_item->data_elements))
	{
		printf("[PDM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}else
	{
		for(i = 0; i < encode_item->data_elements_num; i++)
		{
			encode_item->data_elements[i].dataType = 28;
			encode_item->data_elements[i].mask_subType = 1;
			encode_item->data_elements[i].subType = 15;
			encode_item->data_elements[i].mask_sendOnLessThenValue = 1;
			encode_item->data_elements[i].sendOnLessThenValue = -32767;
			encode_item->data_elements[i].mask_sendOnMoreThenValue = 1;
			encode_item->data_elements[i].sendOnMoreThenValue = 32767;
			encode_item->data_elements[i].mask_sendAll = 1;
			encode_item->data_elements[i].sendAll = 1;
		}			
	}
	
	encode_item->mask_regional = 0;
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nPDM Encode Data:\n");
		_show_pdm_standard_item(encode_item);
	}
	err_pdm = j2735r63_pdm_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_pdm < 0)
	{				
		printf("pdm encode fail...\n");				
		return -1;		
	}			
	_cmd_pdm_free_struct(encode_item);		

	return 0;
}

int cmd_txPdm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int err, i;
	wme_handle_t handle;
	unsigned short num;
	unsigned char channel_number, data_rate, tx_power, interval;
	unsigned int psid;
	unsigned char macAddr_c[6] = {0};
	unsigned int macAddr_i[6];
	struct out_wsm out_wsm;
	size_t len;
	unsigned char *encodeBuf = NULL;

	if(argc != 6){ /* In threadx, cmd is not included in argc */
		printf("txPdm <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txPdm 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
		return 1;
	}

	psid = atoi(argv[0]);
	channel_number = 178;
	data_rate = atoi(argv[1]);
	tx_power = atoi(argv[2]);

	/* Convert the mac addr */
	if(sscanf(argv[3], "%2x:%2x:%2x:%2x:%2x:%2x\n", &macAddr_i[0], &macAddr_i[1], &macAddr_i[2], &macAddr_i[3], &macAddr_i[4], &macAddr_i[5]) != 6){
		printf("peerMacAddress invalid...\n");
		return 0;
	}	
	for(i = 0; i < 6; i++){
		macAddr_c[i] = (unsigned char) macAddr_i[i];
	}

	interval = atoi(argv[4]);
	num = atoi(argv[5]);
	
	err = wme_init(&handle);
	if(err != 0) {
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);

	out_wsm.psid = psid;
	out_wsm.radio_number = RADIO_0;
	out_wsm.channel_number = channel_number;
	out_wsm.data_rate = data_rate;
	out_wsm.txpwr_level = tx_power;
	out_wsm.extensions.channel = 1;
	out_wsm.extensions.rate = 1;
	out_wsm.extensions.power = 1;
	memcpy(out_wsm.dest_mac, macAddr_c, 6);

	encodeBuf = (unsigned char *)j2735_pdm_calloc(1, MAX_PDM_BUFF_SIZE);
	_encode_txPdm(encodeBuf, MAX_PDM_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_pdm_free(encodeBuf);
	
	while(num != 0){
		err = wme_wsm_send(&handle, &out_wsm);
		if(err != 0) {
			printf("wsm_send = %d\n", err);
			return 0;
		}
		printf("wsm_send success\n");
		num--;
		(void) tx_thread_sleep(interval);
	}
	
	wme_exit(&handle);

	printf("%s", "cmd_txPdm command executed successfully.\n");
	return CLI_OK;
}

