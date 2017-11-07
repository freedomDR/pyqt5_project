#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_rsa_api.h"
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

static int stop_rsa;

/***************************************************************/
 /***************************************************************/
 void *j2735_rsa_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_rsa_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_rsa_func(ULONG input)
{
	(void)input;
	stop_rsa = 1;
	return;
}

void _cmd_rsa_free_struct(rsa_standard_item *free_rsa_item)
{
	if(free_rsa_item == NULL){
		printf("[RSA] Error: free_rsa_struct() input is NULL\n");
	}else{
		/*(4) description, OP*/
		if(free_rsa_item->description != NULL){
			j2735_rsa_free(free_rsa_item->description);
		}

		/*(8) position, OP*/
		if(free_rsa_item->position != NULL){
			if(free_rsa_item->position->utc_time != NULL){
				j2735_rsa_free(free_rsa_item->position->utc_time);
			}
			j2735_rsa_free(free_rsa_item->position);
		}

		j2735_rsa_free(free_rsa_item);
	}	
}

void _show_rsa_standard_item(rsa_standard_item *decoded_item)
{
	int i;

	printf("==========RSA Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) msgCnt*/
	printf("msg_cnt = %d\n", decoded_item->msg_cnt);
	/*(2) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(3) typeEvent*/
	printf("type_event = %hu\n", decoded_item->type_event);
	/*(4) description, OP*/
	if(decoded_item->mask_description == 1){
		printf("description_num = %d\n", decoded_item->description_num);
		for(i = 0; i < decoded_item->description_num; i++)
		{
			printf("description[%d] = %hu\n", i, decoded_item->description[i]);
		}
	}	
	/*(5) priority, OP*/
	if(decoded_item->mask_priority == 1){
		printf("priority = %d\n", decoded_item->priority);
	}	
	/*(6) heading, OP*/
	if(decoded_item->mask_heading == 1){
		printf("heading = %hu\n", decoded_item->heading);
	}	
	/*(7) extent, OP*/
	if(decoded_item->mask_extent == 1){
		printf("extent = %d\n", decoded_item->extent);
	}	
	/*(8) position, OP*/
	if(decoded_item->mask_position == 1){
		/*(8-1) utcTime, OPTIONAL*/
		if(decoded_item->position->mask_utc_time == 1){
			/*(8-1-1) year*/
			if(decoded_item->position->utc_time->mask_year == 1){
				printf("position->utc_time->year = %hu\n", decoded_item->position->utc_time->year);
			}
			/*(8-1-2) month*/
			if(decoded_item->position->utc_time->mask_month == 1){
				printf("position->utc_time->month = %d\n", decoded_item->position->utc_time->month);
			}
			/*(8-1-3) day*/
			if(decoded_item->position->utc_time->mask_day == 1){
				printf("position->utc_time->day = %d\n", decoded_item->position->utc_time->day);
			}
			/*(8-1-4) hour*/
			if(decoded_item->position->utc_time->mask_hour == 1){
				printf("position->utc_time->hour = %d\n", decoded_item->position->utc_time->hour);
			}
			/*(8-1-5) minute*/
			if(decoded_item->position->utc_time->mask_minute == 1){
				printf("position->utc_time->minute = %d\n", decoded_item->position->utc_time->minute);
			}
			/*(8-1-6) second*/
			if(decoded_item->position->utc_time->mask_second == 1){
				printf("position->utc_time->second = %hu\n", decoded_item->position->utc_time->second);
			}
			/*(8-1-7) offset*/
			if(decoded_item->position->utc_time->mask_offset == 1){
				printf("position->utc_time->offset = %hi\n", decoded_item->position->utc_time->offset);
			}
		}							
		/*(8-2) Long*/
		printf("position->longitude = %ld\n", decoded_item->position->longitude);
		/*(8-3) lat*/
		printf("position->lat = %ld\n", decoded_item->position->lat);
		/*(8-4) elevation, OPTIONAL*/
		if(decoded_item->position->mask_elevation == 1){
			printf("position->elevation = %d\n", decoded_item->position->elevation);
		}						
		/*(8-5) heading, OPTIONAL*/
		if(decoded_item->position->mask_heading == 1){
			printf("position->heading = %hu\n", decoded_item->position->heading);
		}						
		/*(8-6) speed, OPTIONAL*/
		if(decoded_item->position->mask_speed == 1){
			printf("position->speed.transmission_state = %d\n", decoded_item->position->speed.transmission_state);
			printf("position->speed.speed = %hu\n", decoded_item->position->speed.speed);
		}							
		/*(8-7) posAccuracy, OPTIONAL*/
		if(decoded_item->position->mask_pos_accuracy == 1){
			printf("position->pos_accuracy.semi_major = %d\n", decoded_item->position->pos_accuracy.semi_major);
			printf("position->pos_accuracy.semi_minor = %d\n", decoded_item->position->pos_accuracy.semi_minor);
			printf("position->pos_accuracy.orientation = %hu\n", decoded_item->position->pos_accuracy.orientation);
		}							
		/*(8-8) timeConfidence, OPTIONAL*/
		if(decoded_item->position->mask_time_confidence == 1){
			printf("position->time_confidence = %d\n", decoded_item->position->time_confidence);
		}						
		/*(8-9) posConfidence, OPTIONAL*/
		if(decoded_item->position->mask_pos_confidence == 1){
			printf("position->pos_confidence.pos = %d\n", decoded_item->position->pos_confidence.pos);
			printf("position->pos_confidence.elevation = %d\n", decoded_item->position->pos_confidence.elevation);
		}						
		/*(8-10) speedCondifence, OPTIONAL*/
		if(decoded_item->position->mask_speed_confidence == 1){
			printf("position->speed_confidence.heading = %d\n", decoded_item->position->speed_confidence.heading);
			printf("position->speed_confidence.speed = %d\n", decoded_item->position->speed_confidence.speed);
			printf("position->speed_confidence.throttle = %d\n", decoded_item->position->speed_confidence.throttle);
		}
	}	
	/*(9) furtherInfoID, OP*/
	if(decoded_item->mask_further_info_id == 1){
		printf("further_info_id = %x %x\n", decoded_item->further_info_id[0], decoded_item->further_info_id[1]);
	}	
	
	printf("===============================\n");
}

int cmd_rxRsa(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	rsa_standard_item *decoded_item = NULL;
	int err_rsa;

	int err, ret;
	TX_TIMER stop_rsa_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxRsa <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxRsa 123\n");
		return 1;
	}
	psid = atoi(argv[0]);

	err = wme_init(&handle);
	if(err != 0){
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);

	/* Create a wsm service */
	memset(&wsm_info, 0, sizeof(wsm_info));
	wsm_info.psid = psid;
	ret = wme_wsm_serv_create(&handle, &serv_index, &wsm_info);
	if(ret < 0) {
		printf("wme_wsm_service = %d\n", ret);
		return 0;
	}
	printf("wme_wsm_service success\n");

	printf("waiting 5s to recv wsm packets\n");
	stop_rsa = 0;
	tx_timer_create(&stop_rsa_timer, NULL, stop_rsa_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_rsa == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (rsa_standard_item *)j2735_rsa_calloc(1, sizeof(rsa_standard_item));
		err_rsa = j2735r63_rsa_decode(decoded_item, in_wsm.data, MAX_RSA_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_rsa < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_rsa_standard_item(decoded_item);}
    
		j2735r63_rsa_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_rsa_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_rsa(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	rsa_standard_item *encode_item = NULL;
	rsa_standard_item *decoded_item = NULL;
	int err_rsa, i, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 RSA test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_rsa_calloc(1, MAX_RSA_BUFF_SIZE);
		encode_item = (rsa_standard_item *)j2735_rsa_calloc(1, sizeof(rsa_standard_item));
		/*(1) msgCnt*/
		encode_item->msg_cnt = 127;
		/*(2) timeStamp, OP*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;		
		/*(3) typeEvent*/
		encode_item->type_event = 65535;
		/*(4) description, OP*/
		encode_item->mask_description = 1;
		encode_item->description_num = 4;
		encode_item->description = (unsigned short *)j2735_rsa_calloc(encode_item->description_num, sizeof(unsigned short));
		for(i = 0; i < encode_item->description_num; i++)
		{
			encode_item->description[i] = 65535;
		}			
		/*(5) priority, OP*/
		encode_item->mask_priority = 1;
		encode_item->priority = 255;			
		/*(6) heading, OP*/
		encode_item->mask_heading = 1;
		encode_item->heading = 65534;		
		/*(7) extent, OP*/
		encode_item->mask_extent = 1;
		encode_item->extent = 15;			
		/*(8) position, OP*/
		encode_item->mask_position = 1;
		encode_item->position = (full_position_vector_item *)j2735_rsa_calloc(1, sizeof(full_position_vector_item));
		if(!(encode_item->position))
		{
			printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(8-1) utc_time, op*/
			encode_item->position->mask_utc_time = 1;
			encode_item->position->utc_time = (ddate_time_item *)j2735_rsa_calloc(1, sizeof(ddate_time_item));
			if(!(encode_item->position->utc_time))
			{
				printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				encode_item->position->utc_time->mask_year = 1;
				encode_item->position->utc_time->year = 4095;
				encode_item->position->utc_time->mask_month = 1;
				encode_item->position->utc_time->month = 12;
				encode_item->position->utc_time->mask_day = 1;
				encode_item->position->utc_time->day = 31;
				encode_item->position->utc_time->mask_hour = 1;
				encode_item->position->utc_time->hour = 23;
				encode_item->position->utc_time->mask_minute = 1;
				encode_item->position->utc_time->minute = 59;
				encode_item->position->utc_time->mask_second = 1;
				encode_item->position->utc_time->second = 59;
				encode_item->position->utc_time->mask_offset = 1;
				encode_item->position->utc_time->offset = 840;
			}
			/*(8-2) longitude*/
			encode_item->position->longitude = 1800000001;
			/*(8-3) lat*/
			encode_item->position->lat = 900000001;
			/*(8-4) elevation*/
			encode_item->position->mask_elevation = 1;
			encode_item->position->elevation = 61439;
			/*(8-5) heading, op*/
			encode_item->position->mask_heading = 1;
			encode_item->position->heading = 28800;
			/*(8-6) speed, op*/
			encode_item->position->mask_speed = 1;
			encode_item->position->speed.speed = 8191;
			encode_item->position->speed.transmission_state = 7;
			/*(8-7) pos_accuracy, op*/
			encode_item->position->mask_pos_accuracy = 1;
			encode_item->position->pos_accuracy.semi_major = 255;
			encode_item->position->pos_accuracy.semi_minor = 254;
			encode_item->position->pos_accuracy.orientation = 65535;
			/*(8-8) time_confidence, op*/
			encode_item->position->mask_time_confidence = 1;
			encode_item->position->time_confidence = 39;
			/*(8-9) pos_confidence, op*/
			encode_item->position->mask_pos_confidence = 1;
			encode_item->position->pos_confidence.pos = 15;
			encode_item->position->pos_confidence.elevation = 15;
			/*(8-10) speed_confidence, op*/
			encode_item->position->mask_speed_confidence = 1;
			encode_item->position->speed_confidence.heading = 7;
			encode_item->position->speed_confidence.speed = 7;
			encode_item->position->speed_confidence.throttle = 3;
		}		
		/*(9) furtherInfoID, OP*/
		encode_item->mask_further_info_id = 1;
		encode_item->further_info_id[0] = 0x41;
		encode_item->further_info_id[1] = 0x42;			

		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("RSA loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_rsa_standard_item(&encode_item);}

		err_rsa = j2735r63_rsa_encode(encode_item, encodeBuf, MAX_RSA_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_rsa < 0)
		{
			printf("rsa encode fail...\n");		
			return -1;	
		}
		_cmd_rsa_free_struct(encode_item);

		decoded_item = (rsa_standard_item *)j2735_rsa_calloc(1, sizeof(rsa_standard_item));
		err_rsa = j2735r63_rsa_decode(decoded_item, encodeBuf, MAX_RSA_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_rsa < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_rsa_standard_item(decoded_item);}
		
		j2735r63_rsa_free_struct(decoded_item);
		j2735_rsa_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}

	printf("%s", "J2735R63 RSA Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txRsa(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, err_rsa;
	rsa_standard_item *encode_item = NULL;

	encode_item = (rsa_standard_item *)j2735_rsa_calloc(1, sizeof(rsa_standard_item));
	/*(1) msgCnt*/
	encode_item->msg_cnt = 127;
	/*(2) timeStamp, OP*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;		
	/*(3) typeEvent*/
	encode_item->type_event = 65535;
	/*(4) description, OP*/
	encode_item->mask_description = 1;
	encode_item->description_num = 4;
	encode_item->description = (unsigned short *)j2735_rsa_calloc(encode_item->description_num, sizeof(unsigned short));
	for(i = 0; i < encode_item->description_num; i++)
	{
		encode_item->description[i] = 65535;
	}			
	/*(5) priority, OP*/
	encode_item->mask_priority = 1;
	encode_item->priority = 255;			
	/*(6) heading, OP*/
	encode_item->mask_heading = 1;
	encode_item->heading = 65534;		
	/*(7) extent, OP*/
	encode_item->mask_extent = 1;
	encode_item->extent = 15;			
	/*(8) position, OP*/
	encode_item->mask_position = 1;
	encode_item->position = (full_position_vector_item *)j2735_rsa_calloc(1, sizeof(full_position_vector_item));
	if(!(encode_item->position))
	{
		printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}else{
		/*(8-1) utc_time, op*/
		encode_item->position->mask_utc_time = 1;
		encode_item->position->utc_time = (ddate_time_item *)j2735_rsa_calloc(1, sizeof(ddate_time_item));
		if(!(encode_item->position->utc_time))
		{
			printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			encode_item->position->utc_time->mask_year = 1;
			encode_item->position->utc_time->year = 4095;
			encode_item->position->utc_time->mask_month = 1;
			encode_item->position->utc_time->month = 12;
			encode_item->position->utc_time->mask_day = 1;
			encode_item->position->utc_time->day = 31;
			encode_item->position->utc_time->mask_hour = 1;
			encode_item->position->utc_time->hour = 23;
			encode_item->position->utc_time->mask_minute = 1;
			encode_item->position->utc_time->minute = 59;
			encode_item->position->utc_time->mask_second = 1;
			encode_item->position->utc_time->second = 59;
			encode_item->position->utc_time->mask_offset = 1;
			encode_item->position->utc_time->offset = 840;
		}
		/*(8-2) longitude*/
		encode_item->position->longitude = 1800000001;
		/*(8-3) lat*/
		encode_item->position->lat = 900000001;
		/*(8-4) elevation*/
		encode_item->position->mask_elevation = 1;
		encode_item->position->elevation = 61439;
		/*(8-5) heading, op*/
		encode_item->position->mask_heading = 1;
		encode_item->position->heading = 28800;
		/*(8-6) speed, op*/
		encode_item->position->mask_speed = 1;
		encode_item->position->speed.speed = 8191;
		encode_item->position->speed.transmission_state = 7;
		/*(8-7) pos_accuracy, op*/
		encode_item->position->mask_pos_accuracy = 1;
		encode_item->position->pos_accuracy.semi_major = 255;
		encode_item->position->pos_accuracy.semi_minor = 254;
		encode_item->position->pos_accuracy.orientation = 65535;
		/*(8-8) time_confidence, op*/
		encode_item->position->mask_time_confidence = 1;
		encode_item->position->time_confidence = 39;
		/*(8-9) pos_confidence, op*/
		encode_item->position->mask_pos_confidence = 1;
		encode_item->position->pos_confidence.pos = 15;
		encode_item->position->pos_confidence.elevation = 15;
		/*(8-10) speed_confidence, op*/
		encode_item->position->mask_speed_confidence = 1;
		encode_item->position->speed_confidence.heading = 7;
		encode_item->position->speed_confidence.speed = 7;
		encode_item->position->speed_confidence.throttle = 3;
	}		
	/*(9) furtherInfoID, OP*/
	encode_item->mask_further_info_id = 1;
	encode_item->further_info_id[0] = 0x41;
	encode_item->further_info_id[1] = 0x42;			

	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nRSA Encode Data:\n");
		_show_rsa_standard_item(encode_item);
	}

	err_rsa = j2735r63_rsa_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_rsa < 0)
	{		
		printf("rsa encode fail...\n");		
		return -1;	
	}		
	_cmd_rsa_free_struct(encode_item);
	
	return 0;
}

int cmd_txRsa(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txRsa <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txRsa 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_rsa_calloc(1, MAX_RSA_BUFF_SIZE);
	_encode_txRsa(encodeBuf, MAX_RSA_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_rsa_free(encodeBuf);

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

	printf("%s", "cmd_txRsa command executed successfully.\n");
	return CLI_OK;
}

