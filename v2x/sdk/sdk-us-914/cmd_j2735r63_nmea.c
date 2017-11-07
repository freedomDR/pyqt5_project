#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_nmea_api.h"
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

static int stop_nmea = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_nmea_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_nmea_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_nmea_func(ULONG input)
{
	(void)input;
	stop_nmea = 1;
	return;
}

void _cmd_nmea_free_struct(nmea_standard_item *free_nmea_item)
{
	if(free_nmea_item == NULL){
		printf("[NMEA] Error: free_nmea_struct() input is NULL\n");
	}else{
		j2735_nmea_free(free_nmea_item);
	}
}

void _show_nmea_standard_item(nmea_standard_item *decoded_item)
{
	int i;

	printf("==========NMEA Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(2) rev, OP*/
	if(decoded_item->mask_rev == 1){
		printf("rev = %d\n", decoded_item->rev);
	}
	/*(3) msg, OP*/
	if(decoded_item->mask_msg == 1){
		printf("msg = %d\n", decoded_item->msg);
	}
	/*(4) wdCount, OP*/
	if(decoded_item->mask_wdCount == 1){
		printf("wdCount = %d\n", decoded_item->wdCount);
	}	
	/*(5) payload*/
	printf("payload = ");
	for(i = 0; i < 1023; i++)
		printf("%x ", decoded_item->payload[i]);

	printf("\n===============================\n");
}

int cmd_rxNmea(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	nmea_standard_item *decoded_item = NULL;
	int err_nmea;

	int err, ret;
	TX_TIMER stop_nmea_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxNmea <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxNmea 123\n");
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
	stop_nmea = 0;
	tx_timer_create(&stop_nmea_timer, NULL, stop_nmea_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_nmea == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (nmea_standard_item *)j2735_nmea_calloc(1, sizeof(nmea_standard_item));
		err_nmea = j2735r63_nmea_decode(decoded_item, in_wsm.data, MAX_NMEA_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_nmea < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_nmea_standard_item(decoded_item);}
    
		j2735r63_nmea_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_nmea_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_nmea(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	nmea_standard_item *encode_item = NULL;
	nmea_standard_item *decoded_item = NULL;
	int err_nmea, i, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 NMEA test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_nmea_calloc(1, MAX_NMEA_BUFF_SIZE);
		encode_item = (nmea_standard_item *)j2735_nmea_calloc(1, sizeof(nmea_standard_item));

		/*(1) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;		
		/*(2) rev, OPTIONAL*/
		encode_item->mask_rev = 1;
		encode_item->rev = 6;
		/*(3) msg, OPTIONAL*/
		encode_item->mask_msg = 1;
		encode_item->msg = 32767;
		/*(4) wdCount, OPTIONAL*/
		encode_item->mask_wdCount = 1;
		encode_item->wdCount= 1023;
		/*(5) payload*/
		for(i = 0; i < 1023; i++)
			encode_item->payload[i] = i % 0xFF;
		
		encode_item->mask_regional = 0;
		
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("NMEA loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_nmea_standard_item(&encode_item);}

		err_nmea = j2735r63_nmea_encode(encode_item, encodeBuf, MAX_NMEA_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_nmea < 0)
		{
			printf("nmea encode fail...\n");		
			return -1;	
		}
		_cmd_nmea_free_struct(encode_item);

		decoded_item = (nmea_standard_item *)j2735_nmea_calloc(1, sizeof(nmea_standard_item));
		err_nmea = j2735r63_nmea_decode(decoded_item, encodeBuf, MAX_NMEA_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_nmea < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_nmea_standard_item(decoded_item);}
		
		j2735r63_nmea_free_struct(decoded_item);
		j2735_nmea_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 NMEA Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txNmea(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, err_nmea;
	nmea_standard_item *encode_item = NULL;

	encode_item = (nmea_standard_item *)j2735_nmea_calloc(1, sizeof(nmea_standard_item));
	/*(1) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;		
	/*(2) rev, OPTIONAL*/
	encode_item->mask_rev = 1;
	encode_item->rev = 6;
	/*(3) msg, OPTIONAL*/
	encode_item->mask_msg = 1;
	encode_item->msg = 32767;
	/*(4) wdCount, OPTIONAL*/
	encode_item->mask_wdCount = 1;
	encode_item->wdCount= 1023;
	/*(5) payload*/
	for(i = 0; i < 1023; i++)
		encode_item->payload[i] = i % 0xFF;

	encode_item->mask_regional = 0;
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nNMEA Encode Data:\n");
		_show_nmea_standard_item(encode_item);
	}
	err_nmea = j2735r63_nmea_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_nmea < 0)
	{				
		printf("nmea encode fail...\n");				
		return -1;		
	}			
	_cmd_nmea_free_struct(encode_item);		

	return 0;
}

int cmd_txNmea(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txNmea <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txNmea 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_nmea_calloc(1, MAX_NMEA_BUFF_SIZE);
	_encode_txNmea(encodeBuf, MAX_NMEA_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_nmea_free(encodeBuf);
	
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

	printf("%s", "cmd_txNmea command executed successfully.\n");
	return CLI_OK;
}

