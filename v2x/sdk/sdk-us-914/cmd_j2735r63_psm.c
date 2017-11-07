#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_psm_api.h"
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

static int stop_psm = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_psm_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_psm_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_psm_func(ULONG input)
{
	(void)input;
	stop_psm = 1;
	return;
}

void _cmd_psm_free_struct(psm_standard_item *free_item)
{
	if(free_item == NULL){
		printf("[PSM] Error: free_psm_struct() input is NULL\n");
	}
	else
	{
		if(free_item->pathHistory.initial_position)
		{
			if(free_item->pathHistory.initial_position->utc_time)
				j2735_psm_free(free_item->pathHistory.initial_position->utc_time);
			j2735_psm_free(free_item->pathHistory.initial_position);
		}
		if(free_item->pathHistory.crumb_data)
			j2735_psm_free(free_item->pathHistory.crumb_data);
		j2735_psm_free(free_item);
	}
}

void _show_psm_standard_item(psm_standard_item *decoded_item)
{
	int j;

	printf("==========PSM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	printf("basicType = %d\n", decoded_item->basicType);
	printf("secMark = %d\n", decoded_item->secMark);
	printf("msgCnt = %d\n", decoded_item->msgCnt);

	printf("id = %x %x %x %x\n", decoded_item->id[0],decoded_item->id[1],decoded_item->id[2],decoded_item->id[3]);
	
	printf("position.lat = %ld\n", decoded_item->position.lat);
	printf("position.longitude = %ld\n", decoded_item->position.longitude);

	if(decoded_item->position.mask_elevation == 1)
		printf("position.elevation = %ld\n", decoded_item->position.elevation);

	printf("accuracy.semi_major = %d\n", decoded_item->accuracy.semi_major);
	printf("accuracy.semi_minor = %d\n", decoded_item->accuracy.semi_minor);
	printf("accuracy.orientation = %d\n", decoded_item->accuracy.orientation);

	printf("speed = %d\n", decoded_item->speed);
	printf("heading = %d\n", decoded_item->heading);

	if(decoded_item->mask_accelSet == 1)
	{
		printf("accelSet.longitude = %d\n", decoded_item->accelSet.longitude);
		printf("accelSet.lat = %d\n", decoded_item->accelSet.lat);
		printf("accelSet.vert = %d\n", decoded_item->accelSet.vert);
		printf("accelSet.yaw = %d\n", decoded_item->accelSet.yaw);
	}

	/*(5) PathHistory, OPTIONAL*/
	if(decoded_item->mask_pathHistory == 1){
		/*(5-1) initialPosition*/
		if(decoded_item->pathHistory.mask_initial_position == 1){
			/*(5-1-1) utcTime, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_utc_time == 1){
				/*(5-1-1-1) year*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_year == 1){
					printf("path_history->initial_position->utc_time->year = %hu\n", decoded_item->pathHistory.initial_position->utc_time->year);
				}
				/*(5-1-1-2) month*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_month == 1){
					printf("path_history->initial_position->utc_time->month = %d\n", decoded_item->pathHistory.initial_position->utc_time->month);
				}
				/*(5-1-1-3) day*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_day == 1){
					printf("path_history->initial_position->utc_time->day = %d\n", decoded_item->pathHistory.initial_position->utc_time->day);
				}
				/*(5-1-1-4) hour*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_hour == 1){
					printf("path_history->initial_position->utc_time->hour = %d\n", decoded_item->pathHistory.initial_position->utc_time->hour);
				}
				/*(5-1-1-5) minute*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_minute == 1){
					printf("path_history->initial_position->utc_time->minute = %d\n", decoded_item->pathHistory.initial_position->utc_time->minute);
				}
				/*(5-1-1-6) second*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_second == 1){
					printf("path_history->initial_position->utc_time->second = %hu\n", decoded_item->pathHistory.initial_position->utc_time->second);
				}
				/*(5-1-1-7) offset*/
				if(decoded_item->pathHistory.initial_position->utc_time->mask_offset == 1){
					printf("path_history->initial_position->utc_time->offset = %hi\n", decoded_item->pathHistory.initial_position->utc_time->offset);
				}
			}							
			/*(5-1-2) Long*/
			printf("path_history->initial_position->longitude = %ld\n", decoded_item->pathHistory.initial_position->longitude);
			/*(5-1-3) lat*/
			printf("path_history->initial_position->lat = %ld\n", decoded_item->pathHistory.initial_position->lat);
			/*(5-1-4) elevation, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_elevation == 1){
				printf("path_history->initial_position->elevation = %d\n", decoded_item->pathHistory.initial_position->elevation);
			}						
			/*(5-1-5) heading, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_heading == 1){
				printf("path_history->initial_position->heading = %hu\n", decoded_item->pathHistory.initial_position->heading);
			}						
			/*(5-1-6) speed, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_speed == 1){
				printf("path_history->initial_position->speed.transmission_state = %d\n", decoded_item->pathHistory.initial_position->speed.transmission_state);
				printf("path_history->initial_position->speed.speed = %hu\n", decoded_item->pathHistory.initial_position->speed.speed);
			}							
			/*(5-1-7) posAccuracy, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_pos_accuracy == 1){
				printf("path_history->initial_position->pos_accuracy.semi_major = %d\n", decoded_item->pathHistory.initial_position->pos_accuracy.semi_major);
				printf("path_history->initial_position->pos_accuracy.semi_minor = %d\n", decoded_item->pathHistory.initial_position->pos_accuracy.semi_minor);
				printf("path_history->initial_position->pos_accuracy.orientation = %hu\n", decoded_item->pathHistory.initial_position->pos_accuracy.orientation);
			}							
			/*(5-1-8) timeConfidence, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_time_confidence == 1){
				printf("path_history->initial_position->time_confidence = %d\n", decoded_item->pathHistory.initial_position->time_confidence);
			}						
			/*(5-1-9) posConfidence, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_pos_confidence == 1){
				printf("path_history->initial_position->pos_confidence.pos = %d\n", decoded_item->pathHistory.initial_position->pos_confidence.pos);
				printf("path_history->initial_position->pos_confidence.elevation = %d\n", decoded_item->pathHistory.initial_position->pos_confidence.elevation);
			}						
			/*(5-1-10) speedCondifence, OPTIONAL*/
			if(decoded_item->pathHistory.initial_position->mask_speed_confidence == 1){
				printf("path_history->initial_position->speed_confidence.heading = %d\n", decoded_item->pathHistory.initial_position->speed_confidence.heading);
				printf("path_history->initial_position->speed_confidence.speed = %d\n", decoded_item->pathHistory.initial_position->speed_confidence.speed);
				printf("path_history->initial_position->speed_confidence.throttle = %d\n", decoded_item->pathHistory.initial_position->speed_confidence.throttle);
			}						
		}						
		/*(5-2) currGNSSstatus, OPTIONAL*/
		if(decoded_item->pathHistory.mask_curr_gps_status == 1){
			printf("path_history->curr_gps_status = %d\n", decoded_item->pathHistory.curr_gps_status);
		}					
		/*(5-3) crumbData*/ 				
		for(j = 0; j < decoded_item->pathHistory.crumb_data_num; j++)
		{
			/*(5-3-1) latOffset */
			printf("path_history->crumb_data[%d].lat_offset = %d\n", j, decoded_item->pathHistory.crumb_data[j].lat_offset);
			/*(5-3-2) lonOffset */
			printf("path_history->crumb_data[%d].lon_offset = %d\n", j, decoded_item->pathHistory.crumb_data[j].lon_offset);
			/*(5-3-3) elevationOffset */
			printf("path_history->crumb_data[%d].elevation_offset = %d\n", j, decoded_item->pathHistory.crumb_data[j].elevation_offset);
			/*(5-3-4) timeOffset */
			printf("path_history->crumb_data[%d].time_offset = %hu\n", j, decoded_item->pathHistory.crumb_data[j].time_offset);
			/*(5-3-5) speed, OPTIONAL */
			if(decoded_item->pathHistory.crumb_data[j].mask_speed == 1){
				printf("path_history->crumb_data[%d].speed = %hu\n", j, decoded_item->pathHistory.crumb_data[j].speed); 					
			}
			/*(5-3-6) posAccuracy, OPTIONAL */
			if(decoded_item->pathHistory.crumb_data[j].mask_pos_accuracy == 1){
				printf("path_history->crumb_data[%d].pos_accuracy.semi_major = %d\n", j, decoded_item->pathHistory.crumb_data[j].pos_accuracy.semi_major);
				printf("path_history->crumb_data[%d].pos_accuracy.semi_minor = %d\n", j, decoded_item->pathHistory.crumb_data[j].pos_accuracy.semi_minor);
				printf("path_history->crumb_data[%d].pos_accuracy.orientation = %hu\n", j, decoded_item->pathHistory.crumb_data[j].pos_accuracy.orientation);
			}
			/*(5-3-7) heading, OPTIONAL */
			if(decoded_item->pathHistory.crumb_data[j].mask_heading == 1){
				printf("path_history->crumb_data[%d].heading = %d\n", j, decoded_item->pathHistory.crumb_data[j].heading);
			}					
		}					
	}					
	/*(6) path_prediction, OPTIONAL*/
	if(decoded_item->mask_pathPrediction == 1){
		printf("pathPrediction.radius_of_curve = %d\n", decoded_item->pathPrediction.radius_of_curve);
		printf("pathPrediction.confidence = %d\n", decoded_item->pathPrediction.confidence);
	}

	if(decoded_item->mask_propulsion == 1)
	{
		if(decoded_item->propulsion.propelled_information_choice == PROPELLED_INFORMATION_HUMAN)
			printf("propulsion.human = %d\n", decoded_item->propulsion.human);
		else if(decoded_item->propulsion.propelled_information_choice == PROPELLED_INFORMATION_ANIMAL)
			printf("propulsion.animal = %d\n", decoded_item->propulsion.animal);
		else if(decoded_item->propulsion.propelled_information_choice == PROPELLED_INFORMATION_MOTOR)
			printf("propulsion.motor = %d\n", decoded_item->propulsion.motor);
	}

	if(decoded_item->mask_useState == 1)
		printf("useState = %d\n", decoded_item->useState);
	if(decoded_item->mask_crossRequest == 1)
		printf("crossRequest = %d\n", decoded_item->crossRequest);
	if(decoded_item->mask_crossState == 1)
		printf("crossState = %d\n", decoded_item->crossState);
	if(decoded_item->mask_clusterSize == 1)
		printf("clusterSize = %d\n", decoded_item->clusterSize);
	if(decoded_item->mask_clusterRadius == 1)
		printf("clusterRadius = %d\n", decoded_item->clusterRadius);
	if(decoded_item->mask_eventResponderType == 1)
		printf("eventResponderType = %d\n", decoded_item->eventResponderType);
	if(decoded_item->mask_activityType == 1)
		printf("activityType = %d\n", decoded_item->activityType);
	if(decoded_item->mask_activitySubType == 1)
		printf("activitySubType = %d\n", decoded_item->activitySubType);
	if(decoded_item->mask_assistType == 1)
		printf("assistType = %d\n", decoded_item->assistType);
	if(decoded_item->mask_sizing == 1)
		printf("sizing = %d\n", decoded_item->sizing);
	if(decoded_item->mask_attachment == 1)
		printf("attachment = %d\n", decoded_item->attachment);
	if(decoded_item->mask_attachmentRadius == 1)
		printf("attachmentRadius = %d\n", decoded_item->attachmentRadius);
	if(decoded_item->mask_animalType == 1)
		printf("animalType = %d\n", decoded_item->animalType);

	printf("===============================\n");
}

int cmd_rxPsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	psm_standard_item *decoded_item = NULL;
	int err_psm;

	int err, ret;
	TX_TIMER stop_psm_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxPsm <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxPsm 123\n");
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
	stop_psm = 0;
	tx_timer_create(&stop_psm_timer, NULL, stop_psm_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_psm == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (psm_standard_item *)j2735_psm_calloc(1, sizeof(psm_standard_item));
		err_psm = j2735r63_psm_decode(decoded_item, in_wsm.data, MAX_PSM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_psm < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_psm_standard_item(decoded_item);}
    
		j2735r63_psm_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_psm_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_psm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	psm_standard_item *encode_item = NULL;
	psm_standard_item *decoded_item = NULL;
	int err_psm, j, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 PSM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_psm_calloc(1, MAX_PSM_BUFF_SIZE);
		encode_item = (psm_standard_item *)j2735_psm_calloc(1, sizeof(psm_standard_item));
		
		encode_item->basicType = 4;
		encode_item->secMark = 65535;		
		encode_item->msgCnt = 127;
		
		encode_item->id[0] = 0x31;
		encode_item->id[1] = 0x32;
		encode_item->id[2] = 0x33;
		encode_item->id[3] = 0x34;
		
		encode_item->position.lat = -900000000;
		encode_item->position.longitude = -1799999999;
		encode_item->position.mask_elevation = 1;
		encode_item->position.elevation = -4096;
		encode_item->position.mask_regional = 0;
		
		encode_item->accuracy.semi_major = 254;
		encode_item->accuracy.semi_minor = 254;
		encode_item->accuracy.orientation = 65535;
		
		encode_item->speed = 8191;		
		encode_item->heading = 28800;
		
		encode_item->mask_accelSet = 1;
		encode_item->accelSet.longitude = 2001;
		encode_item->accelSet.lat = -2000;
		encode_item->accelSet.vert = -127;
		encode_item->accelSet.yaw = -32767;
		
		/*(10) path_history*/
		encode_item->mask_pathHistory = 1;
		encode_item->pathHistory.mask_initial_position = 1;
		encode_item->pathHistory.initial_position = (full_position_vector_item *)j2735_psm_calloc(1, sizeof(full_position_vector_item));
		if(!(encode_item->pathHistory.initial_position))
		{
			printf("[PSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(10-1-1) path_history->initial_position->utc_time, op*/
			encode_item->pathHistory.initial_position->mask_utc_time = 1;
			encode_item->pathHistory.initial_position->utc_time = (ddate_time_item *)j2735_psm_calloc(1, sizeof(ddate_time_item));
			if(!(encode_item->pathHistory.initial_position->utc_time))
			{
				printf("[PSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				encode_item->pathHistory.initial_position->utc_time->mask_year = 1;
				encode_item->pathHistory.initial_position->utc_time->year = 2016;
				encode_item->pathHistory.initial_position->utc_time->mask_month = 1;
				encode_item->pathHistory.initial_position->utc_time->month = 12;
				encode_item->pathHistory.initial_position->utc_time->mask_day = 1;
				encode_item->pathHistory.initial_position->utc_time->day = 31;
				encode_item->pathHistory.initial_position->utc_time->mask_hour = 1;
				encode_item->pathHistory.initial_position->utc_time->hour = 23;
				encode_item->pathHistory.initial_position->utc_time->mask_minute = 1;
				encode_item->pathHistory.initial_position->utc_time->minute = 59;
				encode_item->pathHistory.initial_position->utc_time->mask_second = 1;
				encode_item->pathHistory.initial_position->utc_time->second = 59;
				encode_item->pathHistory.initial_position->utc_time->mask_offset = 1;
				encode_item->pathHistory.initial_position->utc_time->offset = 840;
			}					
			/*(10-1-2) path_history->initial_position->longitude*/
			encode_item->pathHistory.initial_position->longitude = 1800000001;
		
			/*(10-1-3) path_history->initial_position->lat*/
			encode_item->pathHistory.initial_position->lat = 900000001;
		
			/*(10-1-4) path_history->initial_position->elevation*/
			encode_item->pathHistory.initial_position->mask_elevation = 1;
			encode_item->pathHistory.initial_position->elevation = -4095;
		
			/*(10-1-5) path_history->initial_position->heading, op*/
			encode_item->pathHistory.initial_position->mask_heading = 1;
			encode_item->pathHistory.initial_position->heading = 27800;
		
			/*(10-1-6) path_history->initial_position->speed, op*/
			encode_item->pathHistory.initial_position->mask_speed = 1;
			encode_item->pathHistory.initial_position->speed.speed = 8191;
			encode_item->pathHistory.initial_position->speed.transmission_state = 7;
		
			/*(10-1-7) path_history->initial_position->pos_accuracy, op*/
			encode_item->pathHistory.initial_position->mask_pos_accuracy = 1;
			encode_item->pathHistory.initial_position->pos_accuracy.semi_major = 253;
			encode_item->pathHistory.initial_position->pos_accuracy.semi_minor = 252;
			encode_item->pathHistory.initial_position->pos_accuracy.orientation = 65534;
		
			/*(10-1-8) path_history->initial_position->time_confidence, op*/
			encode_item->pathHistory.initial_position->mask_time_confidence = 1;
			encode_item->pathHistory.initial_position->time_confidence = 39;
		
			/*(10-1-9) path_history->initial_position->pos_confidence, op*/
			encode_item->pathHistory.initial_position->mask_pos_confidence = 1;
			encode_item->pathHistory.initial_position->pos_confidence.pos = 15;
			encode_item->pathHistory.initial_position->pos_confidence.elevation = 15;
		
			/*(10-1-10) path_history->initial_position->speed_confidence, op*/
			encode_item->pathHistory.initial_position->mask_speed_confidence = 1;
			encode_item->pathHistory.initial_position->speed_confidence.heading = 1;
			encode_item->pathHistory.initial_position->speed_confidence.speed = 2;
			encode_item->pathHistory.initial_position->speed_confidence.throttle = 0;
		}
		/*(10-2) path_history->curr_gps_status, op*/
		encode_item->pathHistory.mask_curr_gps_status = 0;
		encode_item->pathHistory.curr_gps_status = 64;
		
		/*(10-3) path_history->crumb_data*/
		encode_item->pathHistory.crumb_data_num = 1;
		encode_item->pathHistory.crumb_data = (path_history_point_item *)j2735_psm_calloc(encode_item->pathHistory.crumb_data_num, sizeof(path_history_point_item));
		if(!(encode_item->pathHistory.crumb_data))
		{
			printf("[PSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		
		for(j = 0; j < encode_item->pathHistory.crumb_data_num; j++)
		{
			/*(5-3-1) path_history->crumb_data.lat_offset*/
			encode_item->pathHistory.crumb_data[j].lat_offset = 131071;
		
			/*(5-3-2) path_history->crumb_data.lon_offset*/
			encode_item->pathHistory.crumb_data[j].lon_offset = 131071;
		
			/*(5-3-3) path_history->crumb_data.elevation_offset*/
			encode_item->pathHistory.crumb_data[j].elevation_offset = 2047;
		
			/*(5-3-4) path_history->crumb_data.time_offset*/
			encode_item->pathHistory.crumb_data[j].time_offset = 65535;
		
			/*(5-3-5) path_history->crumb_data.speed, op*/
			encode_item->pathHistory.crumb_data[j].mask_speed = 1;
			encode_item->pathHistory.crumb_data[j].speed = 8191;
		
			/*(5-3-6) path_history->crumb_data.pos_accuracy, op*/
			encode_item->pathHistory.crumb_data[j].mask_pos_accuracy = 1;
			encode_item->pathHistory.crumb_data[j].pos_accuracy.semi_major = 251;
			encode_item->pathHistory.crumb_data[j].pos_accuracy.semi_minor = 250;
			encode_item->pathHistory.crumb_data[j].pos_accuracy.orientation = 65533;
		
			/*(5-3-7) path_history->crumb_data.heading, op*/
			encode_item->pathHistory.crumb_data[j].mask_heading = 1;
			encode_item->pathHistory.crumb_data[j].heading = 240;
		}
		
		/*(11) path_prediction, op*/
		encode_item->mask_pathPrediction = 1;
		encode_item->pathPrediction.radius_of_curve = -32767;
		encode_item->pathPrediction.confidence = 200;
		
		encode_item->mask_propulsion = 1;
		encode_item->propulsion.propelled_information_choice = PROPELLED_INFORMATION_MOTOR;
		encode_item->propulsion.motor = 5;
		
		encode_item->mask_useState = 1;
		encode_item->useState = 511;
		encode_item->mask_crossRequest = 1;
		encode_item->crossRequest = 1;
		encode_item->mask_crossState = 1;
		encode_item->crossState = 1;
		encode_item->mask_clusterSize = 1;
		encode_item->clusterSize = 3;
		encode_item->mask_clusterRadius = 1;
		encode_item->clusterRadius = 100;
		encode_item->mask_eventResponderType = 1;
		encode_item->eventResponderType = 7;
		encode_item->mask_activityType = 1;
		encode_item->activityType = 63;
		encode_item->mask_activitySubType = 1;
		encode_item->activitySubType = 127;
		encode_item->mask_assistType = 1;
		encode_item->assistType = 32;
		encode_item->mask_sizing = 1;
		encode_item->sizing = 31;
		encode_item->mask_attachment = 1;
		encode_item->attachment = 6;
		encode_item->mask_attachmentRadius = 1;
		encode_item->attachmentRadius = 200;
		encode_item->mask_animalType = 1;
		encode_item->animalType = 3;
		
		encode_item->mask_regional = 0;

		
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("PSM loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_psm_standard_item(&encode_item);}

		err_psm = j2735r63_psm_encode(encode_item, encodeBuf, MAX_PSM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_psm < 0)
		{
			printf("psm encode fail...\n");		
			return -1;	
		}
		_cmd_psm_free_struct(encode_item);

		decoded_item = (psm_standard_item *)j2735_psm_calloc(1, sizeof(psm_standard_item));
		err_psm = j2735r63_psm_decode(decoded_item, encodeBuf, MAX_PSM_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_psm < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_psm_standard_item(decoded_item);}
		
		j2735r63_psm_free_struct(decoded_item);
		j2735_psm_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 PSM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txPsm(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int j, err_psm;
	psm_standard_item *encode_item = NULL;

	encode_item = (psm_standard_item *)j2735_psm_calloc(1, sizeof(psm_standard_item));
	
	encode_item->basicType = 4;
	encode_item->secMark = 65535;		
	encode_item->msgCnt = 127;

	encode_item->id[0] = 0x31;
	encode_item->id[1] = 0x32;
	encode_item->id[2] = 0x33;
	encode_item->id[3] = 0x34;
	
	encode_item->position.lat = -900000000;
	encode_item->position.longitude = -1799999999;
	encode_item->position.mask_elevation = 1;
	encode_item->position.elevation = -4096;
	encode_item->position.mask_regional = 0;

	encode_item->accuracy.semi_major = 254;
	encode_item->accuracy.semi_minor = 254;
	encode_item->accuracy.orientation = 65535;

	encode_item->speed = 8191;		
	encode_item->heading = 28800;
	
	encode_item->mask_accelSet = 1;
	encode_item->accelSet.longitude = 2001;
	encode_item->accelSet.lat = -2000;
	encode_item->accelSet.vert = -127;
	encode_item->accelSet.yaw = -32767;

	/*(10) path_history*/
	encode_item->mask_pathHistory = 1;
	encode_item->pathHistory.mask_initial_position = 1;
	encode_item->pathHistory.initial_position = (full_position_vector_item *)j2735_psm_calloc(1, sizeof(full_position_vector_item));
	if(!(encode_item->pathHistory.initial_position))
	{
		printf("[PSM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}else{
		/*(10-1-1) path_history->initial_position->utc_time, op*/
		encode_item->pathHistory.initial_position->mask_utc_time = 1;
		encode_item->pathHistory.initial_position->utc_time = (ddate_time_item *)j2735_psm_calloc(1, sizeof(ddate_time_item));
		if(!(encode_item->pathHistory.initial_position->utc_time))
		{
			printf("[PSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			encode_item->pathHistory.initial_position->utc_time->mask_year = 1;
			encode_item->pathHistory.initial_position->utc_time->year = 2016;
			encode_item->pathHistory.initial_position->utc_time->mask_month = 1;
			encode_item->pathHistory.initial_position->utc_time->month = 12;
			encode_item->pathHistory.initial_position->utc_time->mask_day = 1;
			encode_item->pathHistory.initial_position->utc_time->day = 31;
			encode_item->pathHistory.initial_position->utc_time->mask_hour = 1;
			encode_item->pathHistory.initial_position->utc_time->hour = 23;
			encode_item->pathHistory.initial_position->utc_time->mask_minute = 1;
			encode_item->pathHistory.initial_position->utc_time->minute = 59;
			encode_item->pathHistory.initial_position->utc_time->mask_second = 1;
			encode_item->pathHistory.initial_position->utc_time->second = 59;
			encode_item->pathHistory.initial_position->utc_time->mask_offset = 1;
			encode_item->pathHistory.initial_position->utc_time->offset = 840;
		}					
		/*(10-1-2) path_history->initial_position->longitude*/
		encode_item->pathHistory.initial_position->longitude = 1800000001;

		/*(10-1-3) path_history->initial_position->lat*/
		encode_item->pathHistory.initial_position->lat = 900000001;

		/*(10-1-4) path_history->initial_position->elevation*/
		encode_item->pathHistory.initial_position->mask_elevation = 1;
		encode_item->pathHistory.initial_position->elevation = -4095;

		/*(10-1-5) path_history->initial_position->heading, op*/
		encode_item->pathHistory.initial_position->mask_heading = 1;
		encode_item->pathHistory.initial_position->heading = 27800;

		/*(10-1-6) path_history->initial_position->speed, op*/
		encode_item->pathHistory.initial_position->mask_speed = 1;
		encode_item->pathHistory.initial_position->speed.speed = 8191;
		encode_item->pathHistory.initial_position->speed.transmission_state = 7;

		/*(10-1-7) path_history->initial_position->pos_accuracy, op*/
		encode_item->pathHistory.initial_position->mask_pos_accuracy = 1;
		encode_item->pathHistory.initial_position->pos_accuracy.semi_major = 253;
		encode_item->pathHistory.initial_position->pos_accuracy.semi_minor = 252;
		encode_item->pathHistory.initial_position->pos_accuracy.orientation = 65534;

		/*(10-1-8) path_history->initial_position->time_confidence, op*/
		encode_item->pathHistory.initial_position->mask_time_confidence = 1;
		encode_item->pathHistory.initial_position->time_confidence = 39;

		/*(10-1-9) path_history->initial_position->pos_confidence, op*/
		encode_item->pathHistory.initial_position->mask_pos_confidence = 1;
		encode_item->pathHistory.initial_position->pos_confidence.pos = 15;
		encode_item->pathHistory.initial_position->pos_confidence.elevation = 15;

		/*(10-1-10) path_history->initial_position->speed_confidence, op*/
		encode_item->pathHistory.initial_position->mask_speed_confidence = 1;
		encode_item->pathHistory.initial_position->speed_confidence.heading = 1;
		encode_item->pathHistory.initial_position->speed_confidence.speed = 2;
		encode_item->pathHistory.initial_position->speed_confidence.throttle = 0;
	}
	/*(10-2) path_history->curr_gps_status, op*/
	encode_item->pathHistory.mask_curr_gps_status = 0;
	encode_item->pathHistory.curr_gps_status = 64;

	/*(10-3) path_history->crumb_data*/
	encode_item->pathHistory.crumb_data_num = 1;
	encode_item->pathHistory.crumb_data = (path_history_point_item *)j2735_psm_calloc(encode_item->pathHistory.crumb_data_num, sizeof(path_history_point_item));
	if(!(encode_item->pathHistory.crumb_data))
	{
		printf("[PSM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}
	
	for(j = 0; j < encode_item->pathHistory.crumb_data_num; j++)
	{
		/*(5-3-1) path_history->crumb_data.lat_offset*/
		encode_item->pathHistory.crumb_data[j].lat_offset = 131071;

		/*(5-3-2) path_history->crumb_data.lon_offset*/
		encode_item->pathHistory.crumb_data[j].lon_offset = 131071;

		/*(5-3-3) path_history->crumb_data.elevation_offset*/
		encode_item->pathHistory.crumb_data[j].elevation_offset = 2047;

		/*(5-3-4) path_history->crumb_data.time_offset*/
		encode_item->pathHistory.crumb_data[j].time_offset = 65535;

		/*(5-3-5) path_history->crumb_data.speed, op*/
		encode_item->pathHistory.crumb_data[j].mask_speed = 1;
		encode_item->pathHistory.crumb_data[j].speed = 8191;

		/*(5-3-6) path_history->crumb_data.pos_accuracy, op*/
		encode_item->pathHistory.crumb_data[j].mask_pos_accuracy = 1;
		encode_item->pathHistory.crumb_data[j].pos_accuracy.semi_major = 251;
		encode_item->pathHistory.crumb_data[j].pos_accuracy.semi_minor = 250;
		encode_item->pathHistory.crumb_data[j].pos_accuracy.orientation = 65533;

		/*(5-3-7) path_history->crumb_data.heading, op*/
		encode_item->pathHistory.crumb_data[j].mask_heading = 1;
		encode_item->pathHistory.crumb_data[j].heading = 240;
	}
	
	/*(11) path_prediction, op*/
	encode_item->mask_pathPrediction = 1;
	encode_item->pathPrediction.radius_of_curve = -32767;
	encode_item->pathPrediction.confidence = 200;

	encode_item->mask_propulsion = 1;
	encode_item->propulsion.propelled_information_choice = PROPELLED_INFORMATION_MOTOR;
	encode_item->propulsion.motor = 5;

	encode_item->mask_useState = 1;
	encode_item->useState = 511;
	encode_item->mask_crossRequest = 1;
	encode_item->crossRequest = 1;
	encode_item->mask_crossState = 1;
	encode_item->crossState = 1;
	encode_item->mask_clusterSize = 1;
	encode_item->clusterSize = 3;
	encode_item->mask_clusterRadius = 1;
	encode_item->clusterRadius = 100;
	encode_item->mask_eventResponderType = 1;
	encode_item->eventResponderType = 7;
	encode_item->mask_activityType = 1;
	encode_item->activityType = 63;
	encode_item->mask_activitySubType = 1;
	encode_item->activitySubType = 127;
	encode_item->mask_assistType = 1;
	encode_item->assistType = 32;
	encode_item->mask_sizing = 1;
	encode_item->sizing = 31;
	encode_item->mask_attachment = 1;
	encode_item->attachment = 6;
	encode_item->mask_attachmentRadius = 1;
	encode_item->attachmentRadius = 200;
	encode_item->mask_animalType = 1;
	encode_item->animalType = 3;

	encode_item->mask_regional = 0;
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nPSM Encode Data:\n");
		_show_psm_standard_item(encode_item);
	}
	err_psm = j2735r63_psm_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_psm < 0)
	{				
		printf("psm encode fail...\n");				
		return -1;		
	}			
	_cmd_psm_free_struct(encode_item);		

	return 0;
}

int cmd_txPsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txPsm <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txPsm 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_psm_calloc(1, MAX_PSM_BUFF_SIZE);
	_encode_txPsm(encodeBuf, MAX_PSM_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_psm_free(encodeBuf);
	
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

	printf("%s", "cmd_txPsm command executed successfully.\n");
	return CLI_OK;
}

