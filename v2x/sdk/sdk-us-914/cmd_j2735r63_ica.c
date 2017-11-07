#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_ica_api.h"
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

static int stop_ica = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_ica_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_ica_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_ica_func(ULONG input)
{
	(void)input;
	stop_ica = 1;
	return;
}

void _cmd_ica_free_struct(ica_standard_item *free_ica_item)
{
	if(free_ica_item == NULL){
		printf("[ICA] Error: free_ica_struct() input is NULL\n");
	}else{
		if(free_ica_item->path)
		{ 		
			if(free_ica_item->path->initial_position)
			{
				if(free_ica_item->path->initial_position->utc_time)
					j2735_ica_free(free_ica_item->path->initial_position->utc_time);
				j2735_ica_free(free_ica_item->path->initial_position);
			}
			if(free_ica_item->path->crumb_data)
				j2735_ica_free(free_ica_item->path->crumb_data);
			j2735_ica_free(free_ica_item->path);
		}
		j2735_ica_free(free_ica_item);
	}
}

void _show_ica_standard_item(ica_standard_item *decoded_item)
{
	int i, j;

	printf("==========ICA Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) msgCnt*/
	printf("msg_cnt = %d\n", decoded_item->msg_cnt);
	
	/*(2) id*/
	for(i = 0; i < 4; i++)
		printf("id[%d] = %x\t", i, decoded_item->id[i]);
	
	/*(3) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(4) coreData, OP*/
	if(decoded_item->mask_partOne == 1){
		/*(4-1) msgCnt*/
		printf("part_one.msg_cnt = %d\n", decoded_item->part_one.msg_cnt);
		/*(4-2) id*/
		printf("part_one.id = %x %x %x %x\n", decoded_item->part_one.id[0], decoded_item->part_one.id[1], decoded_item->part_one.id[2], decoded_item->part_one.id[3]);
		/*(4-3) secMark*/
		printf("part_one.sec_mark = %hu\n", decoded_item->part_one.sec_mark);
		/*(4-4) lat*/
		printf("part_one.lat = %d\n", decoded_item->part_one.lat);
		/*(4-5) Long*/
		printf("part_one.longitude = %d\n", decoded_item->part_one.longitude);
		/*(4-6) elev*/
		printf("part_one.elev = %d\n", decoded_item->part_one.elev);
		/*(4-7) accuracy*/
		printf("part_one.accuracy.semi_major = %d\n", decoded_item->part_one.accuracy.semi_major);
		printf("part_one.accuracy.semi_minor = %d\n", decoded_item->part_one.accuracy.semi_minor);
		printf("part_one.accuracy.orientation = %hu\n", decoded_item->part_one.accuracy.orientation);	
		/*(4-8) transmission*/
		printf("part_one.transmission_state = %d\n", decoded_item->part_one.transmission_state);
		/*(4-9) speed*/
		printf("part_one.speed = %hu\n", decoded_item->part_one.speed);
		/*(4-10) heading*/
		printf("part_one.heading = %hu\n", decoded_item->part_one.heading);
		/*(4-11) angle*/
		printf("part_one.angle = %d\n", decoded_item->part_one.angle);
		/*(4-12) accelSet*/
		printf("part_one.accel_set.longitude = %hu\n", decoded_item->part_one.accel_set.longitude);
		printf("part_one.accel_set.lat = %hu\n", decoded_item->part_one.accel_set.lat);
		printf("part_one.accel_set.vert = %d\n", decoded_item->part_one.accel_set.vert);
		printf("part_one.accel_set.yaw = %hi\n", decoded_item->part_one.accel_set.yaw);
		/*(4-13) brakes*/
		printf("part_one.brakes.wheel_brakes = %d\n", decoded_item->part_one.brakes.wheel_brakes);
		printf("part_one.brakes.traction = %d\n", decoded_item->part_one.brakes.traction);
		printf("part_one.brakes.abs = %d\n", decoded_item->part_one.brakes.abs);
		printf("part_one.brakes.scs = %d\n", decoded_item->part_one.brakes.scs);
		printf("part_one.brakes.brake_boost = %d\n", decoded_item->part_one.brakes.brake_boost);
		printf("part_one.brakes.aux_brakes = %d\n", decoded_item->part_one.brakes.aux_brakes);
		/*(4-14) size*/
		printf("part_one.size.width = %hu\n", decoded_item->part_one.size.width);
		printf("part_one.size.length = %hu\n", decoded_item->part_one.size.length);	
	}

	
	/*(5) PathHistory, OPTIONAL*/
	if(decoded_item->mask_path == 1){
		/*(5-1) initialPosition*/
		if(decoded_item->path->mask_initial_position == 1){
			/*(5-1-1) utcTime, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_utc_time == 1){
				/*(5-1-1-1) year*/
				if(decoded_item->path->initial_position->utc_time->mask_year == 1){
					printf("path_history->initial_position->utc_time->year = %hu\n", decoded_item->path->initial_position->utc_time->year);
				}
				/*(5-1-1-2) month*/
				if(decoded_item->path->initial_position->utc_time->mask_month == 1){
					printf("path_history->initial_position->utc_time->month = %d\n", decoded_item->path->initial_position->utc_time->month);
				}
				/*(5-1-1-3) day*/
				if(decoded_item->path->initial_position->utc_time->mask_day == 1){
					printf("path_history->initial_position->utc_time->day = %d\n", decoded_item->path->initial_position->utc_time->day);
				}
				/*(5-1-1-4) hour*/
				if(decoded_item->path->initial_position->utc_time->mask_hour == 1){
					printf("path_history->initial_position->utc_time->hour = %d\n", decoded_item->path->initial_position->utc_time->hour);
				}
				/*(5-1-1-5) minute*/
				if(decoded_item->path->initial_position->utc_time->mask_minute == 1){
					printf("path_history->initial_position->utc_time->minute = %d\n", decoded_item->path->initial_position->utc_time->minute);
				}
				/*(5-1-1-6) second*/
				if(decoded_item->path->initial_position->utc_time->mask_second == 1){
					printf("path_history->initial_position->utc_time->second = %hu\n", decoded_item->path->initial_position->utc_time->second);
				}
				/*(5-1-1-7) offset*/
				if(decoded_item->path->initial_position->utc_time->mask_offset == 1){
					printf("path_history->initial_position->utc_time->offset = %hi\n", decoded_item->path->initial_position->utc_time->offset);
				}
			}							
			/*(5-1-2) Long*/
			printf("path_history->initial_position->longitude = %ld\n", decoded_item->path->initial_position->longitude);
			/*(5-1-3) lat*/
			printf("path_history->initial_position->lat = %ld\n", decoded_item->path->initial_position->lat);
			/*(5-1-4) elevation, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_elevation == 1){
				printf("path_history->initial_position->elevation = %d\n", decoded_item->path->initial_position->elevation);
			}						
			/*(5-1-5) heading, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_heading == 1){
				printf("path_history->initial_position->heading = %hu\n", decoded_item->path->initial_position->heading);
			}						
			/*(5-1-6) speed, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_speed == 1){
				printf("path_history->initial_position->speed.transmission_state = %d\n", decoded_item->path->initial_position->speed.transmission_state);
				printf("path_history->initial_position->speed.speed = %hu\n", decoded_item->path->initial_position->speed.speed);
			}							
			/*(5-1-7) posAccuracy, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_pos_accuracy == 1){
				printf("path_history->initial_position->pos_accuracy.semi_major = %d\n", decoded_item->path->initial_position->pos_accuracy.semi_major);
				printf("path_history->initial_position->pos_accuracy.semi_minor = %d\n", decoded_item->path->initial_position->pos_accuracy.semi_minor);
				printf("path_history->initial_position->pos_accuracy.orientation = %hu\n", decoded_item->path->initial_position->pos_accuracy.orientation);
			}							
			/*(5-1-8) timeConfidence, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_time_confidence == 1){
				printf("path_history->initial_position->time_confidence = %d\n", decoded_item->path->initial_position->time_confidence);
			}						
			/*(5-1-9) posConfidence, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_pos_confidence == 1){
				printf("path_history->initial_position->pos_confidence.pos = %d\n", decoded_item->path->initial_position->pos_confidence.pos);
				printf("path_history->initial_position->pos_confidence.elevation = %d\n", decoded_item->path->initial_position->pos_confidence.elevation);
			}						
			/*(5-1-10) speedCondifence, OPTIONAL*/
			if(decoded_item->path->initial_position->mask_speed_confidence == 1){
				printf("path_history->initial_position->speed_confidence.heading = %d\n", decoded_item->path->initial_position->speed_confidence.heading);
				printf("path_history->initial_position->speed_confidence.speed = %d\n", decoded_item->path->initial_position->speed_confidence.speed);
				printf("path_history->initial_position->speed_confidence.throttle = %d\n", decoded_item->path->initial_position->speed_confidence.throttle);
			}						
		}						
		/*(5-2) currGNSSstatus, OPTIONAL*/
		if(decoded_item->path->mask_curr_gps_status == 1){
			printf("path_history->curr_gps_status = %d\n", decoded_item->path->curr_gps_status);
		}					
		/*(5-3) crumbData*/ 				
		for(j = 0; j < decoded_item->path->crumb_data_num; j++)
		{
			/*(5-3-1) latOffset */
			printf("path_history->crumb_data[%d].lat_offset = %d\n", j, decoded_item->path->crumb_data[j].lat_offset);
			/*(5-3-2) lonOffset */
			printf("path_history->crumb_data[%d].lon_offset = %d\n", j, decoded_item->path->crumb_data[j].lon_offset);
			/*(5-3-3) elevationOffset */
			printf("path_history->crumb_data[%d].elevation_offset = %d\n", j, decoded_item->path->crumb_data[j].elevation_offset);
			/*(5-3-4) timeOffset */
			printf("path_history->crumb_data[%d].time_offset = %hu\n", j, decoded_item->path->crumb_data[j].time_offset);
			/*(5-3-5) speed, OPTIONAL */
			if(decoded_item->path->crumb_data[j].mask_speed == 1){
				printf("path_history->crumb_data[%d].speed = %hu\n", j, decoded_item->path->crumb_data[j].speed); 					
			}
			/*(5-3-6) posAccuracy, OPTIONAL */
			if(decoded_item->path->crumb_data[j].mask_pos_accuracy == 1){
				printf("path_history->crumb_data[%d].pos_accuracy.semi_major = %d\n", j, decoded_item->path->crumb_data[j].pos_accuracy.semi_major);
				printf("path_history->crumb_data[%d].pos_accuracy.semi_minor = %d\n", j, decoded_item->path->crumb_data[j].pos_accuracy.semi_minor);
				printf("path_history->crumb_data[%d].pos_accuracy.orientation = %hu\n", j, decoded_item->path->crumb_data[j].pos_accuracy.orientation);
			}
			/*(5-3-7) heading, OPTIONAL */
			if(decoded_item->path->crumb_data[j].mask_heading == 1){
				printf("path_history->crumb_data[%d].heading = %d\n", j, decoded_item->path->crumb_data[j].heading);
			}					
		}					
	}					
	/*(6) path_prediction, OPTIONAL*/
	if(decoded_item->mask_pathPrediction == 1){
		printf("path_prediction.radius_of_curve = %d\n", decoded_item->path_prediction.radius_of_curve);
		printf("path_prediction.confidence = %d\n", decoded_item->path_prediction.confidence);
	}
	/*(7) intersectionID */
	if(decoded_item->intersectionID.mask_region == 1)
	{
		printf("intersectionID.region = %d\n", decoded_item->intersectionID.region);
	}			
	printf("intersectionID.id = %d\n", decoded_item->intersectionID.id);

	/*(8) ApproachOrLane*/
	if(decoded_item->laneNumber.approach_or_lane_choice == APPROACHORLANE_LANE)
	{
		printf("laneNumber.choice = APPROACHORLANE_LANE");
		printf("laneNumber.lane = %d\n",decoded_item->laneNumber.lane);
	}
	else if(decoded_item->laneNumber.approach_or_lane_choice == APPROACHORLANE_APPROACH)
	{
		printf("laneNumber.choice = APPROACHORLANE_APPROACH");
		printf("laneNumber.approach = %d\n",decoded_item->laneNumber.approach);
	}
	/*(9) VehicleEventFlags*/
	printf("events = %hu\n", decoded_item->eventFlag);

	printf("===============================\n");
}

int cmd_rxIca(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	ica_standard_item *decoded_item = NULL;
	int err_ica;

	int err, ret;
	TX_TIMER stop_ica_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxIca <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxIca 123\n");
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
	stop_ica = 0;
	tx_timer_create(&stop_ica_timer, NULL, stop_ica_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_ica == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (ica_standard_item *)j2735_ica_calloc(1, sizeof(ica_standard_item));
		err_ica = j2735r63_ica_decode(decoded_item, in_wsm.data, MAX_ICA_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_ica < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_ica_standard_item(decoded_item);}
    
		j2735r63_ica_free_struct(decoded_item);
	}
	

	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_ica_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_ica(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	ica_standard_item *encode_item = NULL;
	ica_standard_item *decoded_item = NULL;
	int err_ica, i, j, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 ICA test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_ica_calloc(1, MAX_ICA_BUFF_SIZE);
		encode_item = (ica_standard_item *)j2735_ica_calloc(1, sizeof(ica_standard_item));
		/*(1) msgCnt*/
		encode_item->msg_cnt = 127;
		/*(2) id*/
		for(i = 0; i < 4; i++)
			encode_item->id[i] = 0x31+i;
		/*(3) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;		
		/*(4) BSMcoreData, OP */
		encode_item->mask_partOne = 1;
		encode_item->part_one.msg_cnt = 127;
		memset(encode_item->part_one.id, 0xFF, 4);
		encode_item->part_one.sec_mark = 65535;
		encode_item->part_one.lat = 900000001;
		encode_item->part_one.longitude = 1800000001;
		encode_item->part_one.elev = 61439;
		encode_item->part_one.accuracy.semi_major = 255;
		encode_item->part_one.accuracy.semi_minor = 254;
		encode_item->part_one.accuracy.orientation = 65535;
		encode_item->part_one.transmission_state = 7;
		encode_item->part_one.speed = 8191;
		encode_item->part_one.heading = 28800;
		encode_item->part_one.angle = 127;
		encode_item->part_one.accel_set.longitude = 2001;
		encode_item->part_one.accel_set.lat = 2001;
		encode_item->part_one.accel_set.vert = 127;
		encode_item->part_one.accel_set.yaw = 32767;
		encode_item->part_one.brakes.wheel_brakes = 2;
		encode_item->part_one.brakes.traction = 3;
		encode_item->part_one.brakes.abs = 3;
		encode_item->part_one.brakes.scs = 2;
		encode_item->part_one.brakes.brake_boost = 2;
		encode_item->part_one.brakes.aux_brakes = 3;
		encode_item->part_one.size.width = 1023;
		encode_item->part_one.size.length = 4095;
		
		/*(5) PathHistory, OP*/
		encode_item->mask_path = 1;
		encode_item->path = (path_history_item *)j2735_ica_calloc(1, sizeof(path_history_item));
		if(!(encode_item->path))
		{
			printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(5-1) path_history->initial_position, op*/
			encode_item->path->mask_initial_position = 1;
			encode_item->path->initial_position = (full_position_vector_item *)j2735_ica_calloc(1, sizeof(full_position_vector_item));
			if(!(encode_item->path->initial_position))
			{
				printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				/*(5-1-1) path_history->initial_position->utc_time, op*/
				encode_item->path->initial_position->mask_utc_time = 1;
				encode_item->path->initial_position->utc_time = (ddate_time_item *)j2735_ica_calloc(1, sizeof(ddate_time_item));
				if(!(encode_item->path->initial_position->utc_time))
				{
					printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}else{
					encode_item->path->initial_position->utc_time->mask_year = 1;
					encode_item->path->initial_position->utc_time->year = 2016;
					encode_item->path->initial_position->utc_time->mask_month = 1;
					encode_item->path->initial_position->utc_time->month = 12;
					encode_item->path->initial_position->utc_time->mask_day = 1;
					encode_item->path->initial_position->utc_time->day = 31;
					encode_item->path->initial_position->utc_time->mask_hour = 1;
					encode_item->path->initial_position->utc_time->hour = 23;
					encode_item->path->initial_position->utc_time->mask_minute = 1;
					encode_item->path->initial_position->utc_time->minute = 59;
					encode_item->path->initial_position->utc_time->mask_second = 1;
					encode_item->path->initial_position->utc_time->second = 59;
					encode_item->path->initial_position->utc_time->mask_offset = 1;
					encode_item->path->initial_position->utc_time->offset = 840;
				}					
		
				/*(5-1-2) path_history->initial_position->longitude*/
				encode_item->path->initial_position->longitude = 1800000001;
		
				/*(5-1-3) path_history->initial_position->lat*/
				encode_item->path->initial_position->lat = 900000001;
		
				/*(5-1-4) path_history->initial_position->elevation*/
				encode_item->path->initial_position->mask_elevation = 1;
				encode_item->path->initial_position->elevation = -4095;
		
				/*(5-1-5) path_history->initial_position->heading, op*/
				encode_item->path->initial_position->mask_heading = 1;
				encode_item->path->initial_position->heading = 27800;
		
				/*(5-1-6) path_history->initial_position->speed, op*/
				encode_item->path->initial_position->mask_speed = 1;
				encode_item->path->initial_position->speed.speed = 8191;
				encode_item->path->initial_position->speed.transmission_state = 7;
		
				/*(5-1-7) path_history->initial_position->pos_accuracy, op*/
				encode_item->path->initial_position->mask_pos_accuracy = 1;
				encode_item->path->initial_position->pos_accuracy.semi_major = 253;
				encode_item->path->initial_position->pos_accuracy.semi_minor = 252;
				encode_item->path->initial_position->pos_accuracy.orientation = 65534;
		
				/*(5-1-8) path_history->initial_position->time_confidence, op*/
				encode_item->path->initial_position->mask_time_confidence = 1;
				encode_item->path->initial_position->time_confidence = 39;
		
				/*(5-1-9) path_history->initial_position->pos_confidence, op*/
				encode_item->path->initial_position->mask_pos_confidence = 1;
				encode_item->path->initial_position->pos_confidence.pos = 15;
				encode_item->path->initial_position->pos_confidence.elevation = 15;
		
				/*(5-1-10) path_history->initial_position->speed_confidence, op*/
				encode_item->path->initial_position->mask_speed_confidence = 1;
				encode_item->path->initial_position->speed_confidence.heading = 7;
				encode_item->path->initial_position->speed_confidence.speed = 7;
				encode_item->path->initial_position->speed_confidence.throttle = 3;
			}
		
			/*(5-2) path_history->curr_gps_status, op*/
			encode_item->path->mask_curr_gps_status = 0;
			encode_item->path->curr_gps_status = 64;
		
			/*(5-3) path_history->crumb_data*/
			encode_item->path->crumb_data_num = 1;
			encode_item->path->crumb_data = (path_history_point_item *)j2735_ica_calloc(encode_item->path->crumb_data_num, sizeof(path_history_point_item));
			if(!(encode_item->path->crumb_data))
			{
				printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			
			for(j = 0; j < encode_item->path->crumb_data_num; j++)
			{
				/*(5-3-1) path_history->crumb_data.lat_offset*/
				encode_item->path->crumb_data[j].lat_offset = 131071;
		
				/*(5-3-2) path_history->crumb_data.lon_offset*/
				encode_item->path->crumb_data[j].lon_offset = 131071;
		
				/*(5-3-3) path_history->crumb_data.elevation_offset*/
				encode_item->path->crumb_data[j].elevation_offset = 2047;
		
				/*(5-3-4) path_history->crumb_data.time_offset*/
				encode_item->path->crumb_data[j].time_offset = 65535;
		
				/*(5-3-5) path_history->crumb_data.speed, op*/
				encode_item->path->crumb_data[j].mask_speed = 1;
				encode_item->path->crumb_data[j].speed = 8191;
		
				/*(5-3-6) path_history->crumb_data.pos_accuracy, op*/
				encode_item->path->crumb_data[j].mask_pos_accuracy = 1;
				encode_item->path->crumb_data[j].pos_accuracy.semi_major = 251;
				encode_item->path->crumb_data[j].pos_accuracy.semi_minor = 250;
				encode_item->path->crumb_data[j].pos_accuracy.orientation = 65533;
		
				/*(5-3-7) path_history->crumb_data.heading, op*/
				encode_item->path->crumb_data[j].mask_heading = 1;
				encode_item->path->crumb_data[j].heading = 240;
			}
		}
		
		/*(6) path_prediction, op*/
		encode_item->mask_pathPrediction = 1;
		encode_item->path_prediction.radius_of_curve = 32767;
		encode_item->path_prediction.confidence = 200;
		
		/*(7) intersectionID*/
		encode_item->intersectionID.mask_region = 1;
		encode_item->intersectionID.region = 65535; 	
		encode_item->intersectionID.id = 65535;
		
		/*(8) laneNumber*/
		encode_item->laneNumber.approach_or_lane_choice = APPROACHORLANE_LANE;
		encode_item->laneNumber.lane = 255;
		
		/*(9) eventFlag*/
		encode_item->eventFlag= 1024;
		
		encode_item->mask_regional = 0;

		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("ICA loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_ica_standard_item(&encode_item);}

		err_ica = j2735r63_ica_encode(encode_item, encodeBuf, MAX_ICA_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_ica < 0)
		{
			printf("ica encode fail...\n");		
			return -1;	
		}
		_cmd_ica_free_struct(encode_item);

		decoded_item = (ica_standard_item *)j2735_ica_calloc(1, sizeof(ica_standard_item));
		err_ica = j2735r63_ica_decode(decoded_item, encodeBuf, MAX_ICA_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_ica < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_ica_standard_item(decoded_item);}
		
		j2735r63_ica_free_struct(decoded_item);
		j2735_ica_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 ICA Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txIca(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, j, err_ica;
	ica_standard_item *encode_item = NULL;

	encode_item = (ica_standard_item *)j2735_ica_calloc(1, sizeof(ica_standard_item));
	/*(1) msgCnt*/
	encode_item->msg_cnt = 127;
	/*(2) id*/
	for(i = 0; i < 4; i++)
		encode_item->id[i] = 0x31+i;
	/*(3) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;		
	/*(4) BSMcoreData, OP */
	encode_item->mask_partOne = 1;
	encode_item->part_one.msg_cnt = 127;
	memset(encode_item->part_one.id, 0xFF, 4);
	encode_item->part_one.sec_mark = 65535;
	encode_item->part_one.lat = 900000001;
	encode_item->part_one.longitude = 1800000001;
	encode_item->part_one.elev = 61439;
	encode_item->part_one.accuracy.semi_major = 255;
	encode_item->part_one.accuracy.semi_minor = 254;
	encode_item->part_one.accuracy.orientation = 65535;
	encode_item->part_one.transmission_state = 7;
	encode_item->part_one.speed = 8191;
	encode_item->part_one.heading = 28800;
	encode_item->part_one.angle = 127;
	encode_item->part_one.accel_set.longitude = 2001;
	encode_item->part_one.accel_set.lat = 2001;
	encode_item->part_one.accel_set.vert = 127;
	encode_item->part_one.accel_set.yaw = 32767;
	encode_item->part_one.brakes.wheel_brakes = 2;
	encode_item->part_one.brakes.traction = 3;
	encode_item->part_one.brakes.abs = 3;
	encode_item->part_one.brakes.scs = 2;
	encode_item->part_one.brakes.brake_boost = 2;
	encode_item->part_one.brakes.aux_brakes = 3;
	encode_item->part_one.size.width = 1023;
	encode_item->part_one.size.length = 4095;

	/*(5) PathHistory, OP*/
	encode_item->mask_path = 1;
	encode_item->path = (path_history_item *)j2735_ica_calloc(1, sizeof(path_history_item));
	if(!(encode_item->path))
	{
		printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}else
	{
		/*(5-1) path_history->initial_position, op*/
		encode_item->path->mask_initial_position = 1;
		encode_item->path->initial_position = (full_position_vector_item *)j2735_ica_calloc(1, sizeof(full_position_vector_item));
		if(!(encode_item->path->initial_position))
		{
			printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(5-1-1) path_history->initial_position->utc_time, op*/
			encode_item->path->initial_position->mask_utc_time = 1;
			encode_item->path->initial_position->utc_time = (ddate_time_item *)j2735_ica_calloc(1, sizeof(ddate_time_item));
			if(!(encode_item->path->initial_position->utc_time))
			{
				printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				encode_item->path->initial_position->utc_time->mask_year = 1;
				encode_item->path->initial_position->utc_time->year = 2016;
				encode_item->path->initial_position->utc_time->mask_month = 1;
				encode_item->path->initial_position->utc_time->month = 12;
				encode_item->path->initial_position->utc_time->mask_day = 1;
				encode_item->path->initial_position->utc_time->day = 31;
				encode_item->path->initial_position->utc_time->mask_hour = 1;
				encode_item->path->initial_position->utc_time->hour = 23;
				encode_item->path->initial_position->utc_time->mask_minute = 1;
				encode_item->path->initial_position->utc_time->minute = 59;
				encode_item->path->initial_position->utc_time->mask_second = 1;
				encode_item->path->initial_position->utc_time->second = 59;
				encode_item->path->initial_position->utc_time->mask_offset = 1;
				encode_item->path->initial_position->utc_time->offset = 840;
			}					
			/*(5-1-2) path_history->initial_position->longitude*/
			encode_item->path->initial_position->longitude = 1800000001;
	
			/*(5-1-3) path_history->initial_position->lat*/
			encode_item->path->initial_position->lat = 900000001;
	
			/*(5-1-4) path_history->initial_position->elevation*/
			encode_item->path->initial_position->mask_elevation = 1;
			encode_item->path->initial_position->elevation = -4095;
	
			/*(5-1-5) path_history->initial_position->heading, op*/
			encode_item->path->initial_position->mask_heading = 1;
			encode_item->path->initial_position->heading = 27800;
	
			/*(5-1-6) path_history->initial_position->speed, op*/
			encode_item->path->initial_position->mask_speed = 1;
			encode_item->path->initial_position->speed.speed = 8191;
			encode_item->path->initial_position->speed.transmission_state = 7;
	
			/*(5-1-7) path_history->initial_position->pos_accuracy, op*/
			encode_item->path->initial_position->mask_pos_accuracy = 1;
			encode_item->path->initial_position->pos_accuracy.semi_major = 253;
			encode_item->path->initial_position->pos_accuracy.semi_minor = 252;
			encode_item->path->initial_position->pos_accuracy.orientation = 65534;
	
			/*(5-1-8) path_history->initial_position->time_confidence, op*/
			encode_item->path->initial_position->mask_time_confidence = 1;
			encode_item->path->initial_position->time_confidence = 39;
	
			/*(5-1-9) path_history->initial_position->pos_confidence, op*/
			encode_item->path->initial_position->mask_pos_confidence = 1;
			encode_item->path->initial_position->pos_confidence.pos = 15;
			encode_item->path->initial_position->pos_confidence.elevation = 15;
	
			/*(5-1-10) path_history->initial_position->speed_confidence, op*/
			encode_item->path->initial_position->mask_speed_confidence = 1;
			encode_item->path->initial_position->speed_confidence.heading = 7;
			encode_item->path->initial_position->speed_confidence.speed = 7;
			encode_item->path->initial_position->speed_confidence.throttle = 3;
		}
		/*(5-2) path_history->curr_gps_status, op*/
		encode_item->path->mask_curr_gps_status = 0;
		encode_item->path->curr_gps_status = 64;
	
		/*(5-3) path_history->crumb_data*/
		encode_item->path->crumb_data_num = 1;
		encode_item->path->crumb_data = (path_history_point_item *)j2735_ica_calloc(encode_item->path->crumb_data_num, sizeof(path_history_point_item));
		if(!(encode_item->path->crumb_data))
		{
			printf("[ICA] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		
		for(j = 0; j < encode_item->path->crumb_data_num; j++)
		{
			/*(5-3-1) path_history->crumb_data.lat_offset*/
			encode_item->path->crumb_data[j].lat_offset = 131071;
	
			/*(5-3-2) path_history->crumb_data.lon_offset*/
			encode_item->path->crumb_data[j].lon_offset = 131071;
	
			/*(5-3-3) path_history->crumb_data.elevation_offset*/
			encode_item->path->crumb_data[j].elevation_offset = 2047;
	
			/*(5-3-4) path_history->crumb_data.time_offset*/
			encode_item->path->crumb_data[j].time_offset = 65535;
	
			/*(5-3-5) path_history->crumb_data.speed, op*/
			encode_item->path->crumb_data[j].mask_speed = 1;
			encode_item->path->crumb_data[j].speed = 8191;
	
			/*(5-3-6) path_history->crumb_data.pos_accuracy, op*/
			encode_item->path->crumb_data[j].mask_pos_accuracy = 1;
			encode_item->path->crumb_data[j].pos_accuracy.semi_major = 251;
			encode_item->path->crumb_data[j].pos_accuracy.semi_minor = 250;
			encode_item->path->crumb_data[j].pos_accuracy.orientation = 65533;
	
			/*(5-3-7) path_history->crumb_data.heading, op*/
			encode_item->path->crumb_data[j].mask_heading = 1;
			encode_item->path->crumb_data[j].heading = 240;
		}
	}
	
	/*(6) path_prediction, op*/
	encode_item->mask_pathPrediction = 1;
	encode_item->path_prediction.radius_of_curve = 32767;
	encode_item->path_prediction.confidence = 200;

	/*(7) intersectionID*/
	encode_item->intersectionID.mask_region = 1;
	encode_item->intersectionID.region = 65535;		
	encode_item->intersectionID.id = 65535;

	/*(8) laneNumber*/
	encode_item->laneNumber.approach_or_lane_choice = APPROACHORLANE_LANE;
	encode_item->laneNumber.lane = 255;

	/*(9) eventFlag*/
	encode_item->eventFlag= 1024;
	
	encode_item->mask_regional = 0;
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nICA Encode Data:\n");
		_show_ica_standard_item(encode_item);
	}
	err_ica = j2735r63_ica_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_ica < 0)
	{				
		printf("ica encode fail...\n");				
		return -1;		
	}			
	_cmd_ica_free_struct(encode_item);		

	return 0;
}

int cmd_txIca(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txIca <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txIca 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_ica_calloc(1, MAX_ICA_BUFF_SIZE);
	_encode_txIca(encodeBuf, MAX_ICA_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_ica_free(encodeBuf);
	
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

	printf("%s", "cmd_txIca command executed successfully.\n");
	return CLI_OK;
}

