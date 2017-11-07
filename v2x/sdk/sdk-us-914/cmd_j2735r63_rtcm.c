#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_rtcm_api.h"
#include "j2735r63_common.h"

#include <libcli.h>
#include <craton/cli.h>
#include <atlk/nav_service.h>

#include <dot3_common.h>
#include <dot3_api.h>
//#include <j2735_mem.h>

#define SHOW_ENCODE 1
#define SHOW_DECODED 1
#define ENCODE_PRINT_LEVEL 1
#define DECODE_PRINT_LEVEL 1
#define MAX_LOOP_COUNT 1

static int stop_rtcm = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_rtcm_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_rtcm_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_rtcm_func(ULONG input)
{
	(void)input;
	stop_rtcm = 1;
	return;
}

void _cmd_rtcm_free_struct(rtcm_standard_item *free_rtcm_item)
{
	if(free_rtcm_item == NULL){
		printf("[RTCM] Error: free_rtcm_item() input is NULL\n");
	}else{
		/*(4) anchorPoint, OP*/
		if(free_rtcm_item->anchorPoint != NULL){
			if(free_rtcm_item->anchorPoint->utc_time != NULL){
				j2735_rtcm_free(free_rtcm_item->anchorPoint->utc_time);
			}
			j2735_rtcm_free(free_rtcm_item->anchorPoint);
		}
		/*(5) rtcmHeader, OP*/
		if(free_rtcm_item->rtcmHeader != NULL){
			j2735_rtcm_free(free_rtcm_item->rtcmHeader);
		}
		/*(6) msgs, OP*/
		if(free_rtcm_item->msgs != NULL){
			j2735_rtcm_free(free_rtcm_item->msgs);
		}

		j2735_rtcm_free(free_rtcm_item);
	}	
}

void _show_rtcm_standard_item(rtcm_standard_item *decoded_item)
{
	int i;

	printf("==========RTCM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) msgCnt*/
	printf("msg_cnt = %d\n", decoded_item->msg_cnt);
	/*(2) rev*/
	printf("rev = %d\n", decoded_item->rev);
	/*(3) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(4) anchorPoint, OP*/
	if(decoded_item->mask_anchorPoint == 1){
		/*(4-1) utcTime, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_utc_time == 1){
			/*(4-1-1) year*/
			if(decoded_item->anchorPoint->utc_time->mask_year == 1){
				printf("anchorPoint->utc_time->year = %hu\n", decoded_item->anchorPoint->utc_time->year);
			}
			/*(4-1-2) month*/
			if(decoded_item->anchorPoint->utc_time->mask_month == 1){
				printf("anchorPoint->utc_time->month = %d\n", decoded_item->anchorPoint->utc_time->month);
			}
			/*(4-1-3) day*/
			if(decoded_item->anchorPoint->utc_time->mask_day == 1){
				printf("anchorPoint->utc_time->day = %d\n", decoded_item->anchorPoint->utc_time->day);
			}
			/*(4-1-4) hour*/
			if(decoded_item->anchorPoint->utc_time->mask_hour == 1){
				printf("anchorPoint->utc_time->hour = %d\n", decoded_item->anchorPoint->utc_time->hour);
			}
			/*(4-1-5) minute*/
			if(decoded_item->anchorPoint->utc_time->mask_minute == 1){
				printf("anchorPoint->utc_time->minute = %d\n", decoded_item->anchorPoint->utc_time->minute);
			}
			/*(4-1-6) second*/
			if(decoded_item->anchorPoint->utc_time->mask_second == 1){
				printf("anchorPoint->utc_time->second = %hu\n", decoded_item->anchorPoint->utc_time->second);
			}
			/*(4-1-7) offset*/
			if(decoded_item->anchorPoint->utc_time->mask_offset == 1){
				printf("anchorPoint->utc_time->offset = %hi\n", decoded_item->anchorPoint->utc_time->offset);
			}
		}							
		/*(4-2) Long*/
		printf("anchorPoint->longitude = %ld\n", decoded_item->anchorPoint->longitude);
		/*(4-3) lat*/
		printf("anchorPoint->lat = %ld\n", decoded_item->anchorPoint->lat);
		/*(4-4) elevation, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_elevation == 1){
			printf("anchorPoint->elevation = %d\n", decoded_item->anchorPoint->elevation);
		}						
		/*(4-5) heading, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_heading == 1){
			printf("anchorPoint->heading = %hu\n", decoded_item->anchorPoint->heading);
		}						
		/*(4-6) speed, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_speed == 1){
			printf("anchorPoint->speed.transmission_state = %d\n", decoded_item->anchorPoint->speed.transmission_state);
			printf("anchorPoint->speed.speed = %hu\n", decoded_item->anchorPoint->speed.speed);
		}							
		/*(4-7) posAccuracy, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_pos_accuracy == 1){
			printf("anchorPoint->pos_accuracy.semi_major = %d\n", decoded_item->anchorPoint->pos_accuracy.semi_major);
			printf("anchorPoint->pos_accuracy.semi_minor = %d\n", decoded_item->anchorPoint->pos_accuracy.semi_minor);
			printf("anchorPoint->pos_accuracy.orientation = %hu\n", decoded_item->anchorPoint->pos_accuracy.orientation);
		}							
		/*(4-8) timeConfidence, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_time_confidence == 1){
			printf("anchorPoint->time_confidence = %d\n", decoded_item->anchorPoint->time_confidence);
		}						
		/*(4-9) posConfidence, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_pos_confidence == 1){
			printf("anchorPoint->pos_confidence.pos = %d\n", decoded_item->anchorPoint->pos_confidence.pos);
			printf("anchorPoint->pos_confidence.elevation = %d\n", decoded_item->anchorPoint->pos_confidence.elevation);
		}						
		/*(4-10) speedCondifence, OPTIONAL*/
		if(decoded_item->anchorPoint->mask_speed_confidence == 1){
			printf("anchorPoint->speed_confidence.heading = %d\n", decoded_item->anchorPoint->speed_confidence.heading);
			printf("anchorPoint->speed_confidence.speed = %d\n", decoded_item->anchorPoint->speed_confidence.speed);
			printf("anchorPoint->speed_confidence.throttle = %d\n", decoded_item->anchorPoint->speed_confidence.throttle);
		}
	}		
	/*(5) rtcmHeader, OP*/
	if(decoded_item->mask_rtcmHeader == 1){
		printf("rtcmHeader->status = %d\n", decoded_item->rtcmHeader->status);
		
		printf("rtcmHeader->offset_set.ant_offset_x = %hi\n", decoded_item->rtcmHeader->offset_set.ant_offset_x);
		printf("rtcmHeader->offset_set.ant_offset_y = %hi\n", decoded_item->rtcmHeader->offset_set.ant_offset_y);
		printf("rtcmHeader->offset_set.ant_offset_z = %hi\n", decoded_item->rtcmHeader->offset_set.ant_offset_z);
	}	
	/*(6) msgs*/
	for(i = 0; i < decoded_item->msgs_num; i++)
	{
		printf("msgs[%d].rtcm_message = %s\n", i, decoded_item->msgs[i].rtcm_message);
	}	
	printf("===============================\n");
}

int cmd_rxRtcm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	rtcm_standard_item *decoded_item = NULL;
	int err_rtcm;

	int err, ret;
	TX_TIMER stop_rtcm_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxRtcm <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxRtcm 123\n");
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
	stop_rtcm = 0;
	tx_timer_create(&stop_rtcm_timer, NULL, stop_rtcm_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_rtcm == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (rtcm_standard_item *)j2735_rtcm_calloc(1, sizeof(rtcm_standard_item));
		err_rtcm = j2735r63_rtcm_decode(decoded_item, in_wsm.data, MAX_RTCM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_rtcm < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_rtcm_standard_item(decoded_item);}
    
		j2735r63_rtcm_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_rtcm_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_rtcm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	rtcm_standard_item *encode_item = NULL;
	rtcm_standard_item *decoded_item = NULL;
	int err_rtcm, i, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 RTCM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_rtcm_calloc(1, MAX_RTCM_BUFF_SIZE);
		encode_item = (rtcm_standard_item *)j2735_rtcm_calloc(1, sizeof(rtcm_standard_item));
		/*(1) msgCnt*/
		encode_item->msg_cnt = 127;
		/*(2) rev*/
		encode_item->rev = 3;
		/*(3) timeStamp, OP*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;		
		/*(4) anchorPoint, OP*/
		encode_item->mask_anchorPoint = 1;
		encode_item->anchorPoint = (full_position_vector_item *)j2735_rtcm_calloc(1, sizeof(full_position_vector_item));
		if(!(encode_item->anchorPoint))
		{
			printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(4-1) utc_time, op*/
			encode_item->anchorPoint->mask_utc_time = 1;
			encode_item->anchorPoint->utc_time = (ddate_time_item *)j2735_rtcm_calloc(1, sizeof(ddate_time_item));
			if(!(encode_item->anchorPoint->utc_time))
			{
				printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				encode_item->anchorPoint->utc_time->mask_year = 1;
				encode_item->anchorPoint->utc_time->year = 4095;
				encode_item->anchorPoint->utc_time->mask_month = 1;
				encode_item->anchorPoint->utc_time->month = 12;
				encode_item->anchorPoint->utc_time->mask_day = 1;
				encode_item->anchorPoint->utc_time->day = 31;
				encode_item->anchorPoint->utc_time->mask_hour = 1;
				encode_item->anchorPoint->utc_time->hour = 23;
				encode_item->anchorPoint->utc_time->mask_minute = 1;
				encode_item->anchorPoint->utc_time->minute = 59;
				encode_item->anchorPoint->utc_time->mask_second = 1;
				encode_item->anchorPoint->utc_time->second = 59;
				encode_item->anchorPoint->utc_time->mask_offset = 1;
				encode_item->anchorPoint->utc_time->offset = 840;
			}
			/*(4-2) longitude*/
			encode_item->anchorPoint->longitude = 1800000001;
			/*(4-3) lat*/
			encode_item->anchorPoint->lat = 900000001;
			/*(4-4) elevation*/
			encode_item->anchorPoint->mask_elevation = 1;
			encode_item->anchorPoint->elevation = 61439;
			/*(4-5) heading, op*/
			encode_item->anchorPoint->mask_heading = 1;
			encode_item->anchorPoint->heading = 28800;
			/*(4-6) speed, op*/
			encode_item->anchorPoint->mask_speed = 1;
			encode_item->anchorPoint->speed.speed = 8191;
			encode_item->anchorPoint->speed.transmission_state = 7;
			/*(4-7) pos_accuracy, op*/
			encode_item->anchorPoint->mask_pos_accuracy = 1;
			encode_item->anchorPoint->pos_accuracy.semi_major = 255;
			encode_item->anchorPoint->pos_accuracy.semi_minor = 254;
			encode_item->anchorPoint->pos_accuracy.orientation = 65535;
			/*(4-8) time_confidence, op*/
			encode_item->anchorPoint->mask_time_confidence = 1;
			encode_item->anchorPoint->time_confidence = 39;
			/*(4-9) pos_confidence, op*/
			encode_item->anchorPoint->mask_pos_confidence = 1;
			encode_item->anchorPoint->pos_confidence.pos = 15;
			encode_item->anchorPoint->pos_confidence.elevation = 15;
			/*(4-10) speed_confidence, op*/
			encode_item->anchorPoint->mask_speed_confidence = 1;
			encode_item->anchorPoint->speed_confidence.heading = 7;
			encode_item->anchorPoint->speed_confidence.speed = 7;
			encode_item->anchorPoint->speed_confidence.throttle = 3;
		}	
		/*(5)RTCMheader, OPTIONAL*/
		encode_item->mask_rtcmHeader = 1;
		encode_item->rtcmHeader = (rtcm_header_item *)j2735_rtcm_calloc(1, sizeof(rtcm_header_item));
		if(!(encode_item->rtcmHeader))
		{
			printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		else
		{
			encode_item->rtcmHeader->status = 7;
			encode_item->rtcmHeader->offset_set.ant_offset_x = 2047;
			encode_item->rtcmHeader->offset_set.ant_offset_y = 255;
			encode_item->rtcmHeader->offset_set.ant_offset_z = 511;
		}
		
		/*(6) msgs*/
		encode_item->msgs_num = 1;
		encode_item->msgs = (rtcm_message_item *)j2735_rtcm_calloc(encode_item->msgs_num, sizeof(rtcm_message_item));
		if(!(encode_item->msgs))
		{
			printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		else
		{
			for(i = 0; i < encode_item->msgs_num; i++)
			{
				memset(encode_item->msgs[i].rtcm_message, 0xFF, 1023);
			}			
		}
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("RSA loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_rsa_standard_item(&encode_item);}

		err_rtcm = j2735r63_rtcm_encode(encode_item, encodeBuf, MAX_RTCM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_rtcm < 0)
		{
			printf("rtcm encode fail...\n");		
			return -1;	
		}
		_cmd_rtcm_free_struct(encode_item);

		decoded_item = (rtcm_standard_item *)j2735_rtcm_calloc(1, sizeof(rtcm_standard_item));
		err_rtcm = j2735r63_rtcm_decode(decoded_item, encodeBuf, MAX_RTCM_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_rtcm < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_rtcm_standard_item(decoded_item);}
		
		j2735r63_rtcm_free_struct(decoded_item);
		j2735_rtcm_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 RTCM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txRtcm(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, err_rtcm;
	rtcm_standard_item *encode_item = NULL;

	encode_item = (rtcm_standard_item *)j2735_rtcm_calloc(1, sizeof(rtcm_standard_item));
	/*(1) msgCnt*/
	encode_item->msg_cnt = 127;
	/*(2) rev*/
	encode_item->rev = 3;
	/*(3) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;		
	/*(4) anchorPoint, OPTIONAL*/
	encode_item->mask_anchorPoint = 1;
	encode_item->anchorPoint = (full_position_vector_item *)j2735_rtcm_calloc(1, sizeof(full_position_vector_item));
	if(!(encode_item->anchorPoint))
	{
		printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}else{
		/*(4-1) utc_time, op*/
		encode_item->anchorPoint->mask_utc_time = 1;
		encode_item->anchorPoint->utc_time = (ddate_time_item *)j2735_rtcm_calloc(1, sizeof(ddate_time_item));
		if(!(encode_item->anchorPoint->utc_time))
		{
			printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			encode_item->anchorPoint->utc_time->mask_year = 1;
			encode_item->anchorPoint->utc_time->year = 4095;
			encode_item->anchorPoint->utc_time->mask_month = 1;
			encode_item->anchorPoint->utc_time->month = 12;
			encode_item->anchorPoint->utc_time->mask_day = 1;
			encode_item->anchorPoint->utc_time->day = 31;
			encode_item->anchorPoint->utc_time->mask_hour = 1;
			encode_item->anchorPoint->utc_time->hour = 23;
			encode_item->anchorPoint->utc_time->mask_minute = 1;
			encode_item->anchorPoint->utc_time->minute = 59;
			encode_item->anchorPoint->utc_time->mask_second = 1;
			encode_item->anchorPoint->utc_time->second = 59;
			encode_item->anchorPoint->utc_time->mask_offset = 1;
			encode_item->anchorPoint->utc_time->offset = 840;
		}
		/*(4-2) longitude*/
		encode_item->anchorPoint->longitude = 1800000001;
		/*(4-3) lat*/
		encode_item->anchorPoint->lat = 900000001;
		/*(4-4) elevation*/
		encode_item->anchorPoint->mask_elevation = 1;
		encode_item->anchorPoint->elevation = 61439;
		/*(4-5) heading, op*/
		encode_item->anchorPoint->mask_heading = 1;
		encode_item->anchorPoint->heading = 28800;
		/*(4-6) speed, op*/
		encode_item->anchorPoint->mask_speed = 1;
		encode_item->anchorPoint->speed.speed = 8191;
		encode_item->anchorPoint->speed.transmission_state = 7;
		/*(4-7) pos_accuracy, op*/
		encode_item->anchorPoint->mask_pos_accuracy = 1;
		encode_item->anchorPoint->pos_accuracy.semi_major = 255;
		encode_item->anchorPoint->pos_accuracy.semi_minor = 254;
		encode_item->anchorPoint->pos_accuracy.orientation = 65535;
		/*(4-8) time_confidence, op*/
		encode_item->anchorPoint->mask_time_confidence = 1;
		encode_item->anchorPoint->time_confidence = 39;
		/*(4-9) pos_confidence, op*/
		encode_item->anchorPoint->mask_pos_confidence = 1;
		encode_item->anchorPoint->pos_confidence.pos = 15;
		encode_item->anchorPoint->pos_confidence.elevation = 15;
		/*(4-10) speed_confidence, op*/
		encode_item->anchorPoint->mask_speed_confidence = 1;
		encode_item->anchorPoint->speed_confidence.heading = 7;
		encode_item->anchorPoint->speed_confidence.speed = 7;
		encode_item->anchorPoint->speed_confidence.throttle = 3;
	}	

	/*(5)RTCMheader, OPTIONAL*/
	encode_item->mask_rtcmHeader = 1;
	encode_item->rtcmHeader	= (rtcm_header_item *)j2735_rtcm_calloc(1, sizeof(rtcm_header_item));
	if(!(encode_item->rtcmHeader))
	{
		printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}
	else
	{
		encode_item->rtcmHeader->status = 7;
		encode_item->rtcmHeader->offset_set.ant_offset_x = 2047;
		encode_item->rtcmHeader->offset_set.ant_offset_y = 255;
		encode_item->rtcmHeader->offset_set.ant_offset_z = 511;
	}

	/*(6) msgs*/
	encode_item->msgs_num = 1;
	encode_item->msgs = (rtcm_message_item *)j2735_rtcm_calloc(encode_item->msgs_num, sizeof(rtcm_message_item));
	if(!(encode_item->msgs))
	{
		printf("[RTCM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}
	else
	{
		for(i = 0; i < encode_item->msgs_num; i++)
		{
			memset(encode_item->msgs[i].rtcm_message, 0xFF, 1023);
		}			
	}
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nRTCM Encode Data:\n");
		_show_rtcm_standard_item(encode_item);
	}
	err_rtcm = j2735r63_rtcm_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_rtcm < 0)
	{				
		printf("rtcm encode fail...\n");				
		return -1;		
	}			
	_cmd_rtcm_free_struct(encode_item);		

	return 0;
}

int cmd_txRtcm(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txRtcm <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txRtcm 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_rtcm_calloc(1, MAX_RTCM_BUFF_SIZE);
	_encode_txRtcm(encodeBuf, MAX_RTCM_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_rtcm_free(encodeBuf);
	
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

	printf("%s", "cmd_txRtcm command executed successfully.\n");
	return CLI_OK;
}

