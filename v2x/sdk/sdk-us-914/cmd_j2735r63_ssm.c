#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_ssm_api.h"
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

static int stop_ssm = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_ssm_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_ssm_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_ssm_func(ULONG input)
{
	(void)input;
	stop_ssm = 1;
	return;
}

void _cmd_ssm_free_struct(ssm_standard_item *free_item)
{
	int i;
	
	if(free_item == NULL)
	{
		printf("[SSM] Error: free_struct() input is NULL\n");
	}
	else
	{
		if(free_item->status)
		{ 		
			for(i = 0; i < free_item->status_num; i++)
			{
				if(free_item->status[i].sig_status)
				{
					j2735_ssm_free(free_item->status[i].sig_status);
				}
			}
			j2735_ssm_free(free_item->status);
		}
		j2735_ssm_free(free_item);
	}
}

void _show_ssm_standard_item(ssm_standard_item *decoded_item)
{
	int i, j, k;

	printf("==========SSM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	printf("second = %d\n", decoded_item->second);
	/*(3) sequence_number, OP*/
	if(decoded_item->mask_sequence_number == 1){
		printf("sequence_number = %d\n", decoded_item->sequence_number);
	}
	/*(4) status*/
	
	printf("status_num = %d\n", decoded_item->status_num);
	for(i = 0; i < decoded_item->status_num; i++)
	{
		/*(4-1) sequenceNumber*/
		printf("status[%d]->sequence_number = %d\n", i, decoded_item->status[i].sequence_number);
		/*(4-2) id*/
		if(decoded_item->status[i].id.mask_region == 1)
			printf("status[%d]->id.region = %d\n", i, decoded_item->status[i].id.region);
		printf("status[%d]->id.id = %d\n", i, decoded_item->status[i].id.id);
		/*(4-3) sigStatus*/
		printf("status[%d]->sig_status_num = %d\n", i, decoded_item->status[i].sig_status_num);
		for(j = 0; j < decoded_item->status[i].sig_status_num; j++)
		{
			/*(4-3-1) requester*/
			if(decoded_item->status[i].sig_status[j].mask_requester == 1){
				printf("status[%d]->sig_status[%d]->requester.id.vehicle_id_choice = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.id.vehicle_id_choice);
				if(decoded_item->status[i].sig_status[j].requester.id.vehicle_id_choice == VEHICLE_ID_ENTITY_ID)
					for(k = 0; k < 4; k++)
						printf("status[%d]->sig_status[%d]->requester.id.entity_id[%d] = %x\n", i, j, k, decoded_item->status[i].sig_status[j].requester.id.entity_id[k]);
				else if(decoded_item->status[i].sig_status[j].requester.id.vehicle_id_choice == VEHICLE_ID_STATION_ID)
					printf("status[%d]->sig_status[%d]->requester.id.station_id = %ld\n", i, j, decoded_item->status[i].sig_status[j].requester.id.station_id);
				printf("status[%d]->sig_status[%d]->requester.request = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.request);
				printf("status[%d]->sig_status[%d]->requester.sequence_number = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.sequence_number);
	
				if(decoded_item->status[i].sig_status[j].requester.mask_role == 1)
					printf("status[%d]->sig_status[%d]->requester.role = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.role);
				if(decoded_item->status[i].sig_status[j].requester.mask_type_data == 1)
				{
					printf("status[%d]->sig_status[%d]->requester.type_data.role = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.type_data.role);
					if(decoded_item->status[i].sig_status[j].requester.type_data.mask_subrole == 1)
						printf("status[%d]->sig_status[%d]->requester.type_data.subrole = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.type_data.subrole);
					if(decoded_item->status[i].sig_status[j].requester.type_data.mask_request == 1)
						printf("status[%d]->sig_status[%d]->requester.type_data.request = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.type_data.request);

					if(decoded_item->status[i].sig_status[j].requester.type_data.mask_iso3883 == 1)
						printf("status[%d]->sig_status[%d]->requester.type_data.iso3883 = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.type_data.iso3883);

					if(decoded_item->status[i].sig_status[j].requester.type_data.mask_hpmsType == 1)
						printf("status[%d]->sig_status[%d]->requester.type_data.hpmsType = %d\n", i, j, decoded_item->status[i].sig_status[j].requester.type_data.hpmsType);

					/*(4-3-2) inboundOn*/
					printf("status[%d]->sig_status[%d]->inboundOn.intersection_access_point_choice = %d\n", i, j, decoded_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice);
					if(decoded_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_LANE)
						printf("status[%d]->sig_status[%d]->inboundOn.lane = %d\n", i, j, decoded_item->status[i].sig_status[j].inboundOn.lane);
					else if(decoded_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_APPROACH)
						printf("status[%d]->sig_status[%d]->inboundOn.approach = %d\n", i, j, decoded_item->status[i].sig_status[j].inboundOn.approach);
					else if(decoded_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_CONNECTION)
						printf("status[%d]->sig_status[%d]->inboundOn.connection = %d\n", i, j, decoded_item->status[i].sig_status[j].inboundOn.connection);

					/*(4-3-3) */
					if(decoded_item->status[i].sig_status[j].mask_outboundOn == 1)
					{
						printf("status[%d]->sig_status[%d]->outboundOn.intersection_access_point_choice = %d\n", i, j, decoded_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice);
						if(decoded_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_LANE)
							printf("status[%d]->sig_status[%d]->outboundOn.lane = %d\n", i, j, decoded_item->status[i].sig_status[j].outboundOn.lane);
						else if(decoded_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_APPROACH)
							printf("status[%d]->sig_status[%d]->outboundOn.approach = %d\n", i, j, decoded_item->status[i].sig_status[j].outboundOn.approach);
						else if(decoded_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_CONNECTION)
							printf("status[%d]->sig_status[%d]->outboundOn.connection = %d\n", i, j, decoded_item->status[i].sig_status[j].outboundOn.connection);

					}
					if(decoded_item->status[i].sig_status[j].mask_minute == 1)
						printf("status[%d]->sig_status[%d]->minute = %d\n", i, j, decoded_item->status[i].sig_status[j].minute);
					if(decoded_item->status[i].sig_status[j].mask_second == 1)
						printf("status[%d]->sig_status[%d]->second = %d\n", i, j, decoded_item->status[i].sig_status[j].second);
					if(decoded_item->status[i].sig_status[j].mask_duration == 1)
						printf("status[%d]->sig_status[%d]->duration = %d\n", i, j, decoded_item->status[i].sig_status[j].duration);
					printf("status[%d]->sig_status[%d]->status = %d\n", i, j, decoded_item->status[i].sig_status[j].status);
				}
			}
		}
	}	
	printf("===============================\n");

}

int _fill_ssm_standard_item(ssm_standard_item *encode_item)
{
	int i, j, k;
	
	/*(1) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;	

	/*(2) second*/
	encode_item->second = 65535;
	
	/*(3) sequenceNumber*/
	encode_item->mask_sequence_number = 1;
	encode_item->sequence_number = 127;
	/*(4) status*/
	encode_item->status_num = 1;
	encode_item->status = (signal_status_item *)j2735_ssm_calloc(encode_item->status_num, sizeof(signal_status_item));
	if(!(encode_item->status))
	{
		printf("[SSM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}
	else
	{
		for(i = 0; i < encode_item->status_num; i++)
		{
			/*(4-1) sequenceNumber*/
			encode_item->status[i].sequence_number = 127;
			/*(4-2) id*/
			encode_item->status[i].id.mask_region = 1;
			encode_item->status[i].id.region = 65535;
			encode_item->status[i].id.id = 65535;
			/*(4-3) sigStatus*/
			encode_item->status[i].sig_status_num = 1;
			encode_item->status[i].sig_status = (signal_status_package_item *)j2735_ssm_calloc(encode_item->status[i].sig_status_num, sizeof(signal_status_package_item));
			if(!(encode_item->status[i].sig_status))
			{
				printf("[SSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			else
			{
				for(j = 0; j < encode_item->status[i].sig_status_num; j++)
				{
					/*(4-3-1) requester*/
					encode_item->status[i].sig_status[j].mask_requester = 1;
					encode_item->status[i].sig_status[j].requester.id.vehicle_id_choice = VEHICLE_ID_ENTITY_ID;
					for(k = 0; k < 4; k++)
						encode_item->status[i].sig_status[j].requester.id.entity_id[k] = 0x31+k;
					/*
					encode_item->status[i].sig_status[j].requester.id.vehicle_id_choice = VEHICLE_ID_STATION_ID;
					encode_item->status[i].sig_status[j].requester.id.station_id = 4294967295;*/
					encode_item->status[i].sig_status[j].requester.request = 255;
					encode_item->status[i].sig_status[j].requester.sequence_number = 127;
					encode_item->status[i].sig_status[j].requester.mask_role = 1;
					encode_item->status[i].sig_status[j].requester.role = 22;
					encode_item->status[i].sig_status[j].requester.mask_type_data = 1;
					encode_item->status[i].sig_status[j].requester.type_data.role = 22;
					encode_item->status[i].sig_status[j].requester.type_data.mask_subrole = 1;
					encode_item->status[i].sig_status[j].requester.type_data.subrole = 15;
					encode_item->status[i].sig_status[j].requester.type_data.mask_request = 1;
					encode_item->status[i].sig_status[j].requester.type_data.request = 15;
					encode_item->status[i].sig_status[j].requester.type_data.mask_iso3883 = 1;
					encode_item->status[i].sig_status[j].requester.type_data.iso3883 = 100;
					encode_item->status[i].sig_status[j].requester.type_data.mask_hpmsType = 1;
					encode_item->status[i].sig_status[j].requester.type_data.hpmsType = 15;
					encode_item->status[i].sig_status[j].requester.type_data.mask_regional = 0;

					/*(4-3-2) inboundOn*/
					encode_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_LANE;
					encode_item->status[i].sig_status[j].inboundOn.lane = 255;
					/*
					encode_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_APPROACH;
					encode_item->status[i].sig_status[j].inboundOn.approach = 15;
					
					encode_item->status[i].sig_status[j].inboundOn.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_CONNECTION;
					encode_item->status[i].sig_status[j].inboundOn.connection = 255;*/
					/*(4-3-3) */
					encode_item->status[i].sig_status[j].mask_outboundOn = 1;
					/*
					encode_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_LANE;
					encode_item->status[i].sig_status[j].outboundOn.lane = 255;
					
					encode_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_APPROACH;
					encode_item->status[i].sig_status[j].outboundOn.approach = 15;*/
					
					encode_item->status[i].sig_status[j].outboundOn.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_CONNECTION;
					encode_item->status[i].sig_status[j].outboundOn.connection = 255;
					/*(4-3-4) */
					encode_item->status[i].sig_status[j].mask_minute = 1;
					encode_item->status[i].sig_status[j].minute = 527040;
					encode_item->status[i].sig_status[j].mask_second = 1;
					encode_item->status[i].sig_status[j].second = 65535;
					encode_item->status[i].sig_status[j].mask_duration = 1;
					encode_item->status[i].sig_status[j].duration = 65535;
					encode_item->status[i].sig_status[j].status = 7;
					encode_item->status[i].sig_status[j].mask_regional = 0;
				}
			}
		}			
	}
	return SUCCESS;
}

int cmd_rxSsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	ssm_standard_item *decoded_item = NULL;
	int err_ssm;

	int err, ret;
	TX_TIMER stop_ssm_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxSsm <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxSsm 123\n");
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
	stop_ssm = 0;
	tx_timer_create(&stop_ssm_timer, NULL, stop_ssm_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_ssm == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (ssm_standard_item *)j2735_ssm_calloc(1, sizeof(ssm_standard_item));
		err_ssm = j2735r63_ssm_decode(decoded_item, in_wsm.data, MAX_SSM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_ssm < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_ssm_standard_item(decoded_item);}
    
		j2735r63_ssm_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_ssm_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_ssm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	ssm_standard_item *encode_item = NULL;
	ssm_standard_item *decoded_item = NULL;
	int err_ssm, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 SSM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_ssm_calloc(1, MAX_SSM_BUFF_SIZE);
		encode_item = (ssm_standard_item *)j2735_ssm_calloc(1, sizeof(ssm_standard_item));

		_fill_ssm_standard_item(encode_item);
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("SSM loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_ssm_standard_item(&encode_item);}

		err_ssm = j2735r63_ssm_encode(encode_item, encodeBuf, MAX_SSM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_ssm < 0)
		{
			printf("ssm encode fail...\n");		
			return -1;	
		}
		_cmd_ssm_free_struct(encode_item);

		decoded_item = (ssm_standard_item *)j2735_ssm_calloc(1, sizeof(ssm_standard_item));
		err_ssm = j2735r63_ssm_decode(decoded_item, encodeBuf, MAX_SSM_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_ssm < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_ssm_standard_item(decoded_item);}
		
		j2735r63_ssm_free_struct(decoded_item);
		j2735_ssm_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 SSM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txSsm(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int err_ssm;
	ssm_standard_item *encode_item = NULL;

	encode_item = (ssm_standard_item *)j2735_ssm_calloc(1, sizeof(ssm_standard_item));

	_fill_ssm_standard_item(encode_item);

	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nSSM Encode Data:\n");
		_show_ssm_standard_item(encode_item);
	}
	err_ssm = j2735r63_ssm_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_ssm < 0)
	{				
		printf("ssm encode fail...\n");				
		return -1;		
	}			
	_cmd_ssm_free_struct(encode_item);		

	return 0;
}

int cmd_txSsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txSsm <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txSsm 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_ssm_calloc(1, MAX_SSM_BUFF_SIZE);
	_encode_txSsm(encodeBuf, MAX_SSM_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_ssm_free(encodeBuf);
	
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

	printf("%s", "cmd_txSsm command executed successfully.\n");
	return CLI_OK;
}

