#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_eva_api.h"
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

static int stop_eva = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_eva_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_eva_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_eva_func(ULONG input)
{
	(void)input;
	stop_eva = 1;
	return;
}

void _cmd_eva_free_struct(eva_standard_item *free_eva_item)
{
	if(free_eva_item == NULL){
		printf("[EVA] Error: free_eva_struct() input is NULL\n");
	}
	else
	{
		if(free_eva_item->rsaMsg)
		{ 		
			if(free_eva_item->rsaMsg->description){
				j2735_eva_free(free_eva_item->rsaMsg->description);
			}
			if(free_eva_item->rsaMsg->position)
			{
				if(free_eva_item->rsaMsg->position->utc_time){
					j2735_eva_free(free_eva_item->rsaMsg->position->utc_time);
				}
				j2735_eva_free(free_eva_item->rsaMsg->position);
			}	
		j2735_eva_free(free_eva_item->rsaMsg);
		}
	j2735_eva_free(free_eva_item);
	}
}

void _show_eva_standard_item(eva_standard_item *decoded_item)
{
	int i;

	printf("==========EVA Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(2) id, OP*/
	if(decoded_item->mask_id == 1){
		for(i = 0; i < 4; i++)
			printf("id[%d] = %x\t", i, decoded_item->id[i]);
	}
	/*======================RSA=========================*/
	/*(1) msgCnt*/
	printf("msg_cnt = %d\n", decoded_item->rsaMsg->msg_cnt);
	/*(2) timeStamp, OP*/
	if(decoded_item->rsaMsg->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->rsaMsg->time_stamp);
	}
	/*(3) typeEvent*/
	printf("type_event = %hu\n", decoded_item->rsaMsg->type_event);
	/*(4) description, OP*/
	if(decoded_item->rsaMsg->mask_description == 1){
		printf("description_num = %d\n", decoded_item->rsaMsg->description_num);
		for(i = 0; i < decoded_item->rsaMsg->description_num; i++)
		{
			printf("description[%d] = %hu\n", i, decoded_item->rsaMsg->description[i]);
		}
	}	
	/*(5) priority, OP*/
	if(decoded_item->rsaMsg->mask_priority == 1){
		printf("priority = %d\n", decoded_item->rsaMsg->priority);
	}	
	/*(6) heading, OP*/
	if(decoded_item->rsaMsg->mask_heading == 1){
		printf("heading = %hu\n", decoded_item->rsaMsg->heading);
	}	
	/*(7) extent, OP*/
	if(decoded_item->rsaMsg->mask_extent == 1){
		printf("extent = %d\n", decoded_item->rsaMsg->extent);
	}	
	/*(8) position, OP*/
	if(decoded_item->rsaMsg->mask_position == 1){
		/*(8-1) utcTime, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_utc_time == 1){
			/*(8-1-1) year*/
			if(decoded_item->rsaMsg->position->utc_time->mask_year == 1){
				printf("position->utc_time->year = %hu\n", decoded_item->rsaMsg->position->utc_time->year);
			}
			/*(8-1-2) month*/
			if(decoded_item->rsaMsg->position->utc_time->mask_month == 1){
				printf("position->utc_time->month = %d\n", decoded_item->rsaMsg->position->utc_time->month);
			}
			/*(8-1-3) day*/
			if(decoded_item->rsaMsg->position->utc_time->mask_day == 1){
				printf("position->utc_time->day = %d\n", decoded_item->rsaMsg->position->utc_time->day);
			}
			/*(8-1-4) hour*/
			if(decoded_item->rsaMsg->position->utc_time->mask_hour == 1){
				printf("position->utc_time->hour = %d\n", decoded_item->rsaMsg->position->utc_time->hour);
			}
			/*(8-1-5) minute*/
			if(decoded_item->rsaMsg->position->utc_time->mask_minute == 1){
				printf("position->utc_time->minute = %d\n", decoded_item->rsaMsg->position->utc_time->minute);
			}
			/*(8-1-6) second*/
			if(decoded_item->rsaMsg->position->utc_time->mask_second == 1){
				printf("position->utc_time->second = %hu\n", decoded_item->rsaMsg->position->utc_time->second);
			}
			/*(8-1-7) offset*/
			if(decoded_item->rsaMsg->position->utc_time->mask_offset == 1){
				printf("position->utc_time->offset = %hi\n", decoded_item->rsaMsg->position->utc_time->offset);
			}
		}							
		/*(8-2) Long*/
		printf("position->longitude = %ld\n", decoded_item->rsaMsg->position->longitude);
		/*(8-3) lat*/
		printf("position->lat = %ld\n", decoded_item->rsaMsg->position->lat);
		/*(8-4) elevation, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_elevation == 1){
			printf("position->elevation = %d\n", decoded_item->rsaMsg->position->elevation);
		}						
		/*(8-5) heading, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_heading == 1){
			printf("position->heading = %hu\n", decoded_item->rsaMsg->position->heading);
		}						
		/*(8-6) speed, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_speed == 1){
			printf("position->speed.transmission_state = %d\n", decoded_item->rsaMsg->position->speed.transmission_state);
			printf("position->speed.speed = %hu\n", decoded_item->rsaMsg->position->speed.speed);
		}							
		/*(8-7) posAccuracy, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_pos_accuracy == 1){
			printf("position->pos_accuracy.semi_major = %d\n", decoded_item->rsaMsg->position->pos_accuracy.semi_major);
			printf("position->pos_accuracy.semi_minor = %d\n", decoded_item->rsaMsg->position->pos_accuracy.semi_minor);
			printf("position->pos_accuracy.orientation = %hu\n", decoded_item->rsaMsg->position->pos_accuracy.orientation);
		}							
		/*(8-8) timeConfidence, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_time_confidence == 1){
			printf("position->time_confidence = %d\n", decoded_item->rsaMsg->position->time_confidence);
		}						
		/*(8-9) posConfidence, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_pos_confidence == 1){
			printf("position->pos_confidence.pos = %d\n", decoded_item->rsaMsg->position->pos_confidence.pos);
			printf("position->pos_confidence.elevation = %d\n", decoded_item->rsaMsg->position->pos_confidence.elevation);
		}						
		/*(8-10) speedCondifence, OPTIONAL*/
		if(decoded_item->rsaMsg->position->mask_speed_confidence == 1){
			printf("position->speed_confidence.heading = %d\n", decoded_item->rsaMsg->position->speed_confidence.heading);
			printf("position->speed_confidence.speed = %d\n", decoded_item->rsaMsg->position->speed_confidence.speed);
			printf("position->speed_confidence.throttle = %d\n", decoded_item->rsaMsg->position->speed_confidence.throttle);
		}
	}	
	/*(9) furtherInfoID, OP*/
	if(decoded_item->rsaMsg->mask_further_info_id == 1){
		printf("further_info_id = %x %x\n", decoded_item->rsaMsg->further_info_id[0], decoded_item->rsaMsg->further_info_id[1]);
	}	
	/*======================RSA=========================*/
	/*(4) ResponseType, OP*/
	if(decoded_item->mask_responseType == 1){
		printf("responseType = %d\n", decoded_item->responseType);
	}
	/*(5) EmergencyDetails, OPTIONAL*/
	if(decoded_item->mask_details == 1){
		/*(1-1) detail->sspRights*/
		printf("detail.ssp_rights = %d\n", decoded_item->details.ssp_rights);
		/*(1-2) detail->sirenUse*/
		printf("detail.siren_use = %d\n", decoded_item->details.siren_use);
		/*(1-3) detail->lightsUse*/
		printf("detail.lights_use = %d\n", decoded_item->details.lights_use);
		/*(1-4) detail->multi*/
		printf("detail.multi = %d\n", decoded_item->details.multi);
		/*(1-5) detail->events, OPTIONAL*/
		if(decoded_item->details.mask_events == 1){
			printf("detail.events.ssp_rights = %d\n", decoded_item->details.events.ssp_rights);
			printf("detail.events.event = %hu\n", decoded_item->details.events.event);
		}						
		/*(1-6) detail->responseType, OPTIONAL*/
		if(decoded_item->details.mask_response_type == 1){
			printf("detail.response_type = %d\n", decoded_item->details.response_type);
		}
	}					
	if(decoded_item->mask_mass == 1){
		printf("mass = %d\n", decoded_item->mass);
	}	
	if(decoded_item->mask_basicType == 1){
		printf("basicType = %d\n", decoded_item->basicType);
	}
	if(decoded_item->mask_vehicleType == 1){
		printf("vehicleType = %d\n", decoded_item->vehicleType);
	}
	if(decoded_item->mask_responseEquip== 1){
		printf("responseEquip = %d\n", decoded_item->responseEquip);
	}
	if(decoded_item->mask_responderType == 1){
		printf("responderType = %d\n", decoded_item->responderType);
	}

	printf("===============================\n");
}

int cmd_rxEva(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	eva_standard_item *decoded_item = NULL;
	int err_eva;

	int err, ret;
	TX_TIMER stop_eva_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxEva <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxEva 123\n");
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
	stop_eva = 0;
	tx_timer_create(&stop_eva_timer, NULL, stop_eva_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_eva == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (eva_standard_item *)j2735_eva_calloc(1, sizeof(eva_standard_item));
		err_eva = j2735r63_eva_decode(decoded_item, in_wsm.data, MAX_EVA_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_eva < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_eva_standard_item(decoded_item);}
    
		j2735r63_eva_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_eva_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_eva(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	eva_standard_item *encode_item = NULL;
	eva_standard_item *decoded_item = NULL;
	int err_eva, i, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 EVA test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_eva_calloc(1, MAX_EVA_BUFF_SIZE);
		encode_item = (eva_standard_item *)j2735_eva_calloc(1, sizeof(eva_standard_item));
		
		/*(1) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;		
		/*(2) id*/
		encode_item->mask_id = 1;
		for(i = 0; i < 4; i++)
			encode_item->id[i] = 0x31+i;
		/*(3) rsaMsg*/
		encode_item->rsaMsg = (rsa_standard_item *)j2735_eva_calloc(1, sizeof(rsa_standard_item));
		if(encode_item->rsaMsg)
		{
			/*(1) msgCnt*/
			encode_item->rsaMsg->msg_cnt = 127;
			/*(2) timeStamp, OP*/
			encode_item->rsaMsg->mask_time_stamp = 1;
			encode_item->rsaMsg->time_stamp = 527040;		
			/*(3) typeEvent*/
			encode_item->rsaMsg->type_event = 65535;
			/*(4) description, OP*/
			encode_item->rsaMsg->mask_description = 1;
			encode_item->rsaMsg->description_num = 4;
			encode_item->rsaMsg->description = (unsigned short *)j2735_eva_calloc(encode_item->rsaMsg->description_num, sizeof(unsigned short));
			for(i = 0; i < encode_item->rsaMsg->description_num; i++)
			{
				encode_item->rsaMsg->description[i] = 65535;
			}			
			/*(5) priority, OP*/
			encode_item->rsaMsg->mask_priority = 1;
			encode_item->rsaMsg->priority = 255;			
			/*(6) heading, OP*/
			encode_item->rsaMsg->mask_heading = 1;
			encode_item->rsaMsg->heading = 65534;		
			/*(7) extent, OP*/
			encode_item->rsaMsg->mask_extent = 1;
			encode_item->rsaMsg->extent = 15;			
			/*(8) position, OP*/
			encode_item->rsaMsg->mask_position = 1;
			encode_item->rsaMsg->position = (full_position_vector_item *)j2735_eva_calloc(1, sizeof(full_position_vector_item));
			if(!(encode_item->rsaMsg->position))
			{
				printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				/*(8-1) utc_time, op*/
				encode_item->rsaMsg->position->mask_utc_time = 1;
				encode_item->rsaMsg->position->utc_time = (ddate_time_item *)j2735_eva_calloc(1, sizeof(ddate_time_item));
				if(!(encode_item->rsaMsg->position->utc_time))
				{
					printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}else{
					encode_item->rsaMsg->position->utc_time->mask_year = 1;
					encode_item->rsaMsg->position->utc_time->year = 4095;
					encode_item->rsaMsg->position->utc_time->mask_month = 1;
					encode_item->rsaMsg->position->utc_time->month = 12;
					encode_item->rsaMsg->position->utc_time->mask_day = 1;
					encode_item->rsaMsg->position->utc_time->day = 31;
					encode_item->rsaMsg->position->utc_time->mask_hour = 1;
					encode_item->rsaMsg->position->utc_time->hour = 23;
					encode_item->rsaMsg->position->utc_time->mask_minute = 1;
					encode_item->rsaMsg->position->utc_time->minute = 59;
					encode_item->rsaMsg->position->utc_time->mask_second = 1;
					encode_item->rsaMsg->position->utc_time->second = 59;
					encode_item->rsaMsg->position->utc_time->mask_offset = 1;
					encode_item->rsaMsg->position->utc_time->offset = 840;
				}
				/*(8-2) longitude*/
				encode_item->rsaMsg->position->longitude = 1800000001;
				/*(8-3) lat*/
				encode_item->rsaMsg->position->lat = 900000001;
				/*(8-4) elevation*/
				encode_item->rsaMsg->position->mask_elevation = 1;
				encode_item->rsaMsg->position->elevation = 61439;
				/*(8-5) heading, op*/
				encode_item->rsaMsg->position->mask_heading = 1;
				encode_item->rsaMsg->position->heading = 28800;
				/*(8-6) speed, op*/
				encode_item->rsaMsg->position->mask_speed = 1;
				encode_item->rsaMsg->position->speed.speed = 8191;
				encode_item->rsaMsg->position->speed.transmission_state = 7;
				/*(8-7) pos_accuracy, op*/
				encode_item->rsaMsg->position->mask_pos_accuracy = 1;
				encode_item->rsaMsg->position->pos_accuracy.semi_major = 255;
				encode_item->rsaMsg->position->pos_accuracy.semi_minor = 254;
				encode_item->rsaMsg->position->pos_accuracy.orientation = 65535;
				/*(8-8) time_confidence, op*/
				encode_item->rsaMsg->position->mask_time_confidence = 1;
				encode_item->rsaMsg->position->time_confidence = 39;
				/*(8-9) pos_confidence, op*/
				encode_item->rsaMsg->position->mask_pos_confidence = 1;
				encode_item->rsaMsg->position->pos_confidence.pos = 15;
				encode_item->rsaMsg->position->pos_confidence.elevation = 15;
				/*(8-10) speed_confidence, op*/
				encode_item->rsaMsg->position->mask_speed_confidence = 1;
				encode_item->rsaMsg->position->speed_confidence.heading = 7;
				encode_item->rsaMsg->position->speed_confidence.speed = 7;
				encode_item->rsaMsg->position->speed_confidence.throttle = 3;
			}		
			/*(9) furtherInfoID, OP*/
			encode_item->rsaMsg->mask_further_info_id = 1;
			encode_item->rsaMsg->further_info_id[0] = 0x41;
			encode_item->rsaMsg->further_info_id[1] = 0x42; 		
		}
		/*(4) ResponseType, OPTIONAL*/
		encode_item->mask_responseType = 1;
		encode_item->responseType = 6;	
		/*(5) EmergencyDetails*/
		encode_item->mask_details = 1;
		encode_item->details.ssp_rights = 31;
		encode_item->details.siren_use = 3;
		encode_item->details.lights_use = 7;
		encode_item->details.multi = 3;
		encode_item->details.mask_events = 1;
		encode_item->details.events.ssp_rights = 31;
		encode_item->details.events.event = 32;
		encode_item->details.mask_response_type = 1;
		encode_item->details.response_type = 6;
		/*(6) VehicleMass*/
		encode_item->mask_mass = 1;
		encode_item->mass = 255;	
		/*(7) VehicleType*/
		encode_item->mask_basicType = 1;
		encode_item->basicType= 15; 
		/*(8) VehicleGroupAffected*/
		encode_item->mask_vehicleType = 1;
		encode_item->vehicleType = 9251;	
		/*(9) IncidentResponseEquipment*/
		encode_item->mask_responseEquip = 1;
		encode_item->responseEquip = 10113; 
		/*(10) ResponderGroupAffected*/
		encode_item->mask_responderType = 1;
		encode_item->responderType= 9742;	
		
		encode_item->mask_regional = 0;

		
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("EVA loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_eva_standard_item(&encode_item);}

		err_eva = j2735r63_eva_encode(encode_item, encodeBuf, MAX_EVA_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_eva < 0)
		{
			printf("eva encode fail...\n");		
			return -1;	
		}
		_cmd_eva_free_struct(encode_item);

		decoded_item = (eva_standard_item *)j2735_eva_calloc(1, sizeof(eva_standard_item));
		err_eva = j2735r63_eva_decode(decoded_item, encodeBuf, MAX_EVA_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_eva < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_eva_standard_item(decoded_item);}
		
		j2735r63_eva_free_struct(decoded_item);
		j2735_eva_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 EVA Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txEva(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, err_eva;
	eva_standard_item *encode_item = NULL;

	encode_item = (eva_standard_item *)j2735_eva_calloc(1, sizeof(eva_standard_item));
	/*(1) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;		
	/*(2) id*/
	encode_item->mask_id = 1;
	for(i = 0; i < 4; i++)
		encode_item->id[i] = 0x31+i;
	/*(3) rsaMsg*/
	encode_item->rsaMsg = (rsa_standard_item *)j2735_eva_calloc(1, sizeof(rsa_standard_item));
	if(encode_item->rsaMsg)
	{
		/*(1) msgCnt*/
		encode_item->rsaMsg->msg_cnt = 127;
		/*(2) timeStamp, OP*/
		encode_item->rsaMsg->mask_time_stamp = 1;
		encode_item->rsaMsg->time_stamp = 527040;		
		/*(3) typeEvent*/
		encode_item->rsaMsg->type_event = 65535;
		/*(4) description, OP*/
		encode_item->rsaMsg->mask_description = 1;
		encode_item->rsaMsg->description_num = 4;
		encode_item->rsaMsg->description = (unsigned short *)j2735_eva_calloc(encode_item->rsaMsg->description_num, sizeof(unsigned short));
		for(i = 0; i < encode_item->rsaMsg->description_num; i++)
		{
			encode_item->rsaMsg->description[i] = 65535;
		}			
		/*(5) priority, OP*/
		encode_item->rsaMsg->mask_priority = 1;
		encode_item->rsaMsg->priority = 255;			
		/*(6) heading, OP*/
		encode_item->rsaMsg->mask_heading = 1;
		encode_item->rsaMsg->heading = 65534;		
		/*(7) extent, OP*/
		encode_item->rsaMsg->mask_extent = 1;
		encode_item->rsaMsg->extent = 15;			
		/*(8) position, OP*/
		encode_item->rsaMsg->mask_position = 1;
		encode_item->rsaMsg->position = (full_position_vector_item *)j2735_eva_calloc(1, sizeof(full_position_vector_item));
		if(!(encode_item->rsaMsg->position))
		{
			printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(8-1) utc_time, op*/
			encode_item->rsaMsg->position->mask_utc_time = 1;
			encode_item->rsaMsg->position->utc_time = (ddate_time_item *)j2735_eva_calloc(1, sizeof(ddate_time_item));
			if(!(encode_item->rsaMsg->position->utc_time))
			{
				printf("[RSA] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				encode_item->rsaMsg->position->utc_time->mask_year = 1;
				encode_item->rsaMsg->position->utc_time->year = 4095;
				encode_item->rsaMsg->position->utc_time->mask_month = 1;
				encode_item->rsaMsg->position->utc_time->month = 12;
				encode_item->rsaMsg->position->utc_time->mask_day = 1;
				encode_item->rsaMsg->position->utc_time->day = 31;
				encode_item->rsaMsg->position->utc_time->mask_hour = 1;
				encode_item->rsaMsg->position->utc_time->hour = 23;
				encode_item->rsaMsg->position->utc_time->mask_minute = 1;
				encode_item->rsaMsg->position->utc_time->minute = 59;
				encode_item->rsaMsg->position->utc_time->mask_second = 1;
				encode_item->rsaMsg->position->utc_time->second = 59;
				encode_item->rsaMsg->position->utc_time->mask_offset = 1;
				encode_item->rsaMsg->position->utc_time->offset = 840;
			}
			/*(8-2) longitude*/
			encode_item->rsaMsg->position->longitude = 1800000001;
			/*(8-3) lat*/
			encode_item->rsaMsg->position->lat = 900000001;
			/*(8-4) elevation*/
			encode_item->rsaMsg->position->mask_elevation = 1;
			encode_item->rsaMsg->position->elevation = 61439;
			/*(8-5) heading, op*/
			encode_item->rsaMsg->position->mask_heading = 1;
			encode_item->rsaMsg->position->heading = 28800;
			/*(8-6) speed, op*/
			encode_item->rsaMsg->position->mask_speed = 1;
			encode_item->rsaMsg->position->speed.speed = 8191;
			encode_item->rsaMsg->position->speed.transmission_state = 7;
			/*(8-7) pos_accuracy, op*/
			encode_item->rsaMsg->position->mask_pos_accuracy = 1;
			encode_item->rsaMsg->position->pos_accuracy.semi_major = 255;
			encode_item->rsaMsg->position->pos_accuracy.semi_minor = 254;
			encode_item->rsaMsg->position->pos_accuracy.orientation = 65535;
			/*(8-8) time_confidence, op*/
			encode_item->rsaMsg->position->mask_time_confidence = 1;
			encode_item->rsaMsg->position->time_confidence = 39;
			/*(8-9) pos_confidence, op*/
			encode_item->rsaMsg->position->mask_pos_confidence = 1;
			encode_item->rsaMsg->position->pos_confidence.pos = 15;
			encode_item->rsaMsg->position->pos_confidence.elevation = 15;
			/*(8-10) speed_confidence, op*/
			encode_item->rsaMsg->position->mask_speed_confidence = 1;
			encode_item->rsaMsg->position->speed_confidence.heading = 7;
			encode_item->rsaMsg->position->speed_confidence.speed = 7;
			encode_item->rsaMsg->position->speed_confidence.throttle = 3;
		}		
		/*(9) furtherInfoID, OP*/
		encode_item->rsaMsg->mask_further_info_id = 1;
		encode_item->rsaMsg->further_info_id[0] = 0x41;
		encode_item->rsaMsg->further_info_id[1] = 0x42;			
	}
	/*(4) ResponseType, OPTIONAL*/
	encode_item->mask_responseType = 1;
	encode_item->responseType = 6;	
	/*(5) EmergencyDetails*/
	encode_item->mask_details = 1;
	encode_item->details.ssp_rights = 31;
	encode_item->details.siren_use = 3;
	encode_item->details.lights_use = 7;
	encode_item->details.multi = 3;
	encode_item->details.mask_events = 1;
	encode_item->details.events.ssp_rights = 31;
	encode_item->details.events.event = 32;
	encode_item->details.mask_response_type = 1;
	encode_item->details.response_type = 6;
	/*(6) VehicleMass*/
	encode_item->mask_mass = 1;
	encode_item->mass = 255;	
	/*(7) VehicleType*/
	encode_item->mask_basicType = 1;
	encode_item->basicType= 15;	
	/*(8) VehicleGroupAffected*/
	encode_item->mask_vehicleType = 1;
	encode_item->vehicleType = 9251;	
	/*(9) IncidentResponseEquipment*/
	encode_item->mask_responseEquip = 1;
	encode_item->responseEquip = 10113;	
	/*(10) ResponderGroupAffected*/
	encode_item->mask_responderType = 1;
	encode_item->responderType= 9742;	
	
	encode_item->mask_regional = 0;
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nEVA Encode Data:\n");
		_show_eva_standard_item(encode_item);
	}
	err_eva = j2735r63_eva_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_eva < 0)
	{				
		printf("eva encode fail...\n");				
		return -1;		
	}			
	_cmd_eva_free_struct(encode_item);		

	return 0;
}

int cmd_txEva(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txEva <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txEva 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_eva_calloc(1, MAX_EVA_BUFF_SIZE);
	_encode_txEva(encodeBuf, MAX_EVA_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_eva_free(encodeBuf);
	
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

	printf("%s", "cmd_txEva command executed successfully.\n");
	return CLI_OK;
}

