#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_srm_api.h"
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

static int stop_srm = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_srm_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_srm_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_srm_func(ULONG input)
{
	(void)input;
	stop_srm = 1;
	return;
}

void _cmd_srm_free_struct(srm_standard_item *free_item)
{
	if(free_item == NULL){
		printf("[SRM] Error: free_srm_struct() input is NULL\n");
	}else{
		if(free_item->requests != NULL){ 		
			j2735_srm_free(free_item->requests);
		}
		j2735_srm_free(free_item);
	}
}

void _show_srm_standard_item(srm_standard_item *decoded_item)
{
	int i;

	printf("==========SRM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(2) second*/
	printf("second = %d\n", decoded_item->second);
	/*(3) sequenceNumber, OP*/
	if(decoded_item->mask_sequence_number == 1){
		printf("sequence_number = %d\n", decoded_item->sequence_number);
	}

	/*(4) requests*/
	if(decoded_item->mask_requests == 1){
		printf("requests_num = %d\n", decoded_item->requests_num);
		
		for(i = 0; i < decoded_item->requests_num; i++)
		{
			/*(4-1) request*/
			/*(4-1-1) id*/
			if(decoded_item->requests[i].request.id.mask_region == 1)
				printf("requests[%d].request.id.region = %d\n", i, decoded_item->requests[i].request.id.region);
			printf("requests[%d].request.id.id = %d\n", i, decoded_item->requests[i].request.id.id);
			/*(4-1-2)*/
			printf("requests[%d].request.request_ID = %d\n", i, decoded_item->requests[i].request.request_ID);
			printf("requests[%d].request.request_type = %d\n", i, decoded_item->requests[i].request.request_type);

			/*(4-1-4) inBoundLane*/
			if(decoded_item->requests[i].request.inBoundLane.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_LANE)
				printf("requests[%d].request.inBoundLane.lane = %d\n", i, decoded_item->requests[i].request.inBoundLane.lane);
			else if(decoded_item->requests[i].request.inBoundLane.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_APPROACH)
				printf("requests[%d].request.inBoundLane.approach = %d\n", i, decoded_item->requests[i].request.inBoundLane.approach);
			else if(decoded_item->requests[i].request.inBoundLane.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_CONNECTION)
				printf("requests[%d].request.inBoundLane.connection = %d\n", i, decoded_item->requests[i].request.inBoundLane.connection);

			/*(4-1-5) outBoundLane*/
			if(decoded_item->requests[i].request.mask_outBoundLane == 1)
			{
				if(decoded_item->requests[i].request.outBoundLane.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_LANE)
					printf("requests[%d].request.outBoundLane.lane = %d\n", i, decoded_item->requests[i].request.outBoundLane.lane);
				else if(decoded_item->requests[i].request.outBoundLane.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_APPROACH)
					printf("requests[%d].request.outBoundLane.approach = %d\n", i, decoded_item->requests[i].request.outBoundLane.approach);
				else if(decoded_item->requests[i].request.outBoundLane.intersection_access_point_choice == INTERSECTION_ACCESS_POINT_CONNECTION)
					printf("requests[%d].request.outBoundLane.connection = %d\n", i, decoded_item->requests[i].request.outBoundLane.connection);
			}
			/*(4-2) minute, OP*/
			if(decoded_item->requests[i].mask_minute == 1)
				printf("requests[%d].minute = %d\n", i, decoded_item->requests[i].minute);
			if(decoded_item->requests[i].mask_second == 1)
				printf("requests[%d].second = %d\n", i, decoded_item->requests[i].second);
			if(decoded_item->requests[i].mask_duration == 1)
				printf("requests[%d].duration = %d\n", i, decoded_item->requests[i].duration);
		}	
	}
	/*(5) requester*/
	/*(5-1) id*/
	if(decoded_item->requestor.id.vehicle_id_choice == VEHICLE_ID_ENTITY_ID){
	for(i = 0; i < 4; i++)
		printf("requestor.id.entity_id[%d] = %d\n", i, decoded_item->requestor.id.entity_id[i]);
	}
	else if(decoded_item->requestor.id.vehicle_id_choice == VEHICLE_ID_STATION_ID)
		printf("requestor.id.station_id = %ld\n", decoded_item->requestor.id.station_id);

	/*(5-2) type*/
	if(decoded_item->requestor.mask_type == 1){
		printf("requestor.type.role = %d\n", decoded_item->requestor.type.role);

		if(decoded_item->requestor.type.mask_subrole == 1)
			printf("requestor.type.subrole = %d\n", decoded_item->requestor.type.subrole);
		if(decoded_item->requestor.type.mask_request == 1)
			printf("requestor.type.request = %d\n", decoded_item->requestor.type.request);
		if(decoded_item->requestor.type.mask_iso3883 == 1)
			printf("requestor.type.iso3883 = %d\n", decoded_item->requestor.type.iso3883);
		if(decoded_item->requestor.type.mask_hpmsType == 1)
			printf("requestor.type.hpmsType = %d\n", decoded_item->requestor.type.hpmsType);
	}
	/*(5-3) position*/
	if(decoded_item->requestor.mask_position == 1){
		/*(5-3-1) position*/
		printf("requestor.position.position.lat = %ld\n", decoded_item->requestor.position.position.lat);
		printf("requestor.position.position.longitude = %ld\n", decoded_item->requestor.position.position.longitude);
		if(decoded_item->requestor.position.position.mask_elevation == 1)
			printf("requestor.position.position.elevation = %ld\n", decoded_item->requestor.position.position.elevation);
		/*(5-3-2) heading*/
		if(decoded_item->requestor.position.mask_heading == 1)
			printf("requestor.position.heading = %d\n", decoded_item->requestor.position.heading);
		/*(5-3-3) speed*/
		if(decoded_item->requestor.position.mask_speed == 1){
			printf("requestor.position.speed.transmission_state = %d\n", decoded_item->requestor.position.speed.transmission_state);
			printf("requestor.position.speed.speed = %d\n", decoded_item->requestor.position.speed.speed);
		}
	}
	
	/*(5-4) name*/
	if(decoded_item->requestor.mask_name == 1)
		printf("requestor.name = %s\n", decoded_item->requestor.name);

	/*(5-5) routeName*/
	if(decoded_item->requestor.mask_routeName == 1)
		printf("requestor.routeName = %s\n", decoded_item->requestor.routeName);

	/*(5-6) transitStatus*/
	if(decoded_item->requestor.mask_transitStatus == 1)
		printf("requestor.transitStatus = %d\n", decoded_item->requestor.transitStatus);
	if(decoded_item->requestor.mask_transitOccupancy == 1)
		printf("requestor.transitOccupancy = %d\n", decoded_item->requestor.transitOccupancy);
	if(decoded_item->requestor.mask_transitSchedule == 1)
		printf("requestor.transitSchedule = %d\n", decoded_item->requestor.transitSchedule);

	printf("===============================\n");
}

int cmd_rxSrm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	srm_standard_item *decoded_item = NULL;
	int err_srm;

	int err, ret;
	TX_TIMER stop_srm_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxSrm <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxSrm 123\n");
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
	stop_srm = 0;
	tx_timer_create(&stop_srm_timer, NULL, stop_srm_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_srm == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (srm_standard_item *)j2735_srm_calloc(1, sizeof(srm_standard_item));
		err_srm = j2735r63_srm_decode(decoded_item, in_wsm.data, MAX_SRM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_srm < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_srm_standard_item(decoded_item);}
    
		j2735r63_srm_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_srm_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_srm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	srm_standard_item *encode_item = NULL;
	srm_standard_item *decoded_item = NULL;
	int err_srm, i, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 SRM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_srm_calloc(1, MAX_SRM_BUFF_SIZE);
		encode_item = (srm_standard_item *)j2735_srm_calloc(1, sizeof(srm_standard_item));

		/*(1) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;	
		/*(2) second*/
		encode_item->second = 65535;
		/*(3) sequenceNumber*/
		encode_item->mask_sequence_number = 1;
		encode_item->sequence_number = 127;

		/*(4) requests*/
		encode_item->mask_requests = 1;
		encode_item->requests_num= 1;
		encode_item->requests = (signal_request_package_item *)j2735_srm_calloc(encode_item->requests_num, sizeof(signal_request_package_item));
		if(!(encode_item->requests))
		{
			printf("[SRM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else
		{
			for(i = 0; i < encode_item->requests_num; i++)
			{
				/*(4-1) request*/
				/*(4-1-1) id*/
				encode_item->requests[i].request.id.mask_region = 1;
				encode_item->requests[i].request.id.region= 65535; 
				encode_item->requests[i].request.id.id = 65535;
				/*(4-1-2)*/
				encode_item->requests[i].request.request_ID = 255;
				encode_item->requests[i].request.request_type = 3;
				/*(4-1-4) inBoundLane*/
				encode_item->requests[i].request.inBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_LANE;
				encode_item->requests[i].request.inBoundLane.lane = 255;
				/*
				encode_item->requests[i].request.inBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_APPROACH;
				encode_item->requests[i].request.inBoundLane.approach = 15;
				encode_item->requests[i].request.inBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_CONNECTION;
				encode_item->requests[i].request.inBoundLane.connection = 255;*/
				/*(4-1-5) outBoundLane*/
				encode_item->requests[i].request.mask_outBoundLane = 1;
				/*
				encode_item->requests[i].request.outBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_LANE;
				encode_item->requests[i].request.outBoundLane.lane = 255;
				encode_item->requests[i].request.outBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_APPROACH;
				encode_item->requests[i].request.outBoundLane.approach = 15;*/
				encode_item->requests[i].request.outBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_CONNECTION;
				encode_item->requests[i].request.outBoundLane.connection = 255;
				encode_item->requests[i].request.mask_regional = 0;

				/*(4-2) minute, OP*/
				encode_item->requests[i].mask_minute = 1;
				encode_item->requests[i].minute = 527040;
				encode_item->requests[i].mask_second = 1;
				encode_item->requests[i].second = 65535;
				encode_item->requests[i].mask_duration = 1;
				encode_item->requests[i].duration = 65535;
				encode_item->requests[i].mask_regional = 0;
				
			}			
		}
		/*(5) requester*/
		/*(5-1) id*/
		encode_item->requestor.id.vehicle_id_choice = VEHICLE_ID_ENTITY_ID;
		for(i = 0; i < 4; i++)
			encode_item->requestor.id.entity_id[i] = 0x31+i;
		/*
		encode_item->requestor.id.vehicle_id_choice = VEHICLE_ID_STATION_ID;
		encode_item->requestor.id.station_id = 4294967295;*/	
		/*(5-2) type*/
		encode_item->requestor.mask_type = 1;
		encode_item->requestor.type.role = 22;
		encode_item->requestor.type.mask_subrole = 1;
		encode_item->requestor.type.subrole = 15;
		encode_item->requestor.type.mask_request = 1;
		encode_item->requestor.type.request = 15;
		encode_item->requestor.type.mask_iso3883 = 1;
		encode_item->requestor.type.iso3883 = 100;
		encode_item->requestor.type.mask_hpmsType = 1;
		encode_item->requestor.type.hpmsType = 15;
		encode_item->requestor.type.mask_regional = 0;
		/*(5-3) position*/
		encode_item->requestor.mask_position = 1;
		/*(5-3-1) position*/
		encode_item->requestor.position.position.lat = 900000001;
		encode_item->requestor.position.position.longitude = 1800000001;
		encode_item->requestor.position.position.mask_elevation = 1;
		encode_item->requestor.position.position.elevation = 61439;
		encode_item->requestor.position.position.mask_regional = 0;
		encode_item->requestor.position.position.regional_num = 0;
		encode_item->requestor.position.position.regional = NULL;
		/*(5-3-2) heading*/
		encode_item->requestor.position.mask_heading = 1;
		encode_item->requestor.position.heading = 28800;
		/*(5-3-3) speed*/
		encode_item->requestor.position.mask_speed = 1;
		encode_item->requestor.position.speed.transmission_state = 7;
		encode_item->requestor.position.speed.speed = 8191;
		/*(5-4) name*/
		encode_item->requestor.mask_name = 1;
		memset(encode_item->requestor.name, 0, 64);
		memcpy(encode_item->requestor.name, "SRM_RequestorDescription_name", strlen("SRM_RequestorDescription_name"));
		encode_item->requestor.name[strlen("SRM_RequestorDescription_name")+1] = '\0';
		/*(5-5) routeName*/
		encode_item->requestor.mask_routeName = 1;
		memset(encode_item->requestor.routeName, 0, 64);
		memcpy(encode_item->requestor.routeName, "SRM_RequestorDescription_routeName", strlen("SRM_RequestorDescription_routeName"));
		encode_item->requestor.routeName[strlen("SRM_RequestorDescription_routeName")+1] = '\0';
		/*(5-6) transitStatus*/
		encode_item->requestor.mask_transitStatus = 1;
		encode_item->requestor.transitStatus = 255;
		encode_item->requestor.mask_transitOccupancy = 1;
		encode_item->requestor.transitOccupancy = 7;
		encode_item->requestor.mask_transitSchedule = 1;
		encode_item->requestor.transitSchedule = 121;
		encode_item->requestor.mask_regional = 0;

		
		encode_item->mask_regional = 0;		
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("SRM loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_srm_standard_item(&encode_item);}

		err_srm = j2735r63_srm_encode(encode_item, encodeBuf, MAX_SRM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_srm < 0)
		{
			printf("srm encode fail...\n");		
			return -1;	
		}
		_cmd_srm_free_struct(encode_item);

		decoded_item = (srm_standard_item *)j2735_srm_calloc(1, sizeof(srm_standard_item));
		err_srm = j2735r63_srm_decode(decoded_item, encodeBuf, MAX_SRM_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_srm < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_srm_standard_item(decoded_item);}
		
		j2735r63_srm_free_struct(decoded_item);
		j2735_srm_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 SRM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txSrm(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, k, err_srm;
	srm_standard_item *encode_item = NULL;

	encode_item = (srm_standard_item *)j2735_srm_calloc(1, sizeof(srm_standard_item));
	/*(1) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;	
	/*(2) second*/
	encode_item->second = 65535;
	/*(3) sequenceNumber*/
	encode_item->mask_sequence_number = 1;
	encode_item->sequence_number = 127;

	/*(4) requests*/
	encode_item->mask_requests = 1;
	encode_item->requests_num= 1;
	encode_item->requests = (signal_request_package_item *)j2735_srm_calloc(encode_item->requests_num, sizeof(signal_request_package_item));
	if(!(encode_item->requests))
	{
		printf("[SRM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}else
	{
		for(i = 0; i < encode_item->requests_num; i++)
		{
			/*(4-1) request*/
			/*(4-1-1) id*/
			encode_item->requests[i].request.id.mask_region = 1;
			encode_item->requests[i].request.id.region= 65535; 
			encode_item->requests[i].request.id.id = 65535;
			/*(4-1-2)*/
			encode_item->requests[i].request.request_ID = 255;
			encode_item->requests[i].request.request_type = 3;
			/*(4-1-4) inBoundLane*/
			encode_item->requests[i].request.inBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_LANE;
			encode_item->requests[i].request.inBoundLane.lane = 255;
			/*
			encode_item->requests[i].request.inBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_APPROACH;
			encode_item->requests[i].request.inBoundLane.approach = 15;
			encode_item->requests[i].request.inBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_CONNECTION;
			encode_item->requests[i].request.inBoundLane.connection = 255;*/
			/*(4-1-5) outBoundLane*/
			encode_item->requests[i].request.mask_outBoundLane = 1;
			/*
			encode_item->requests[i].request.outBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_LANE;
			encode_item->requests[i].request.outBoundLane.lane = 255;
			encode_item->requests[i].request.outBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_APPROACH;
			encode_item->requests[i].request.outBoundLane.approach = 15;*/
			encode_item->requests[i].request.outBoundLane.intersection_access_point_choice = INTERSECTION_ACCESS_POINT_CONNECTION;
			encode_item->requests[i].request.outBoundLane.connection = 255;
			encode_item->requests[i].request.mask_regional = 0;

			/*(4-2) minute, OP*/
			encode_item->requests[i].mask_minute = 1;
			encode_item->requests[i].minute = 527040;
			encode_item->requests[i].mask_second = 1;
			encode_item->requests[i].second = 65535;
			encode_item->requests[i].mask_duration = 1;
			encode_item->requests[i].duration = 65535;
			encode_item->requests[i].mask_regional = 0;
			
		}			
	}
	/*(5) requester*/
	/*(5-1) id*/
	encode_item->requestor.id.vehicle_id_choice = VEHICLE_ID_ENTITY_ID;
	for(k = 0; k < 4; k++)
		encode_item->requestor.id.entity_id[k] = 0x31+k;
	/*
	encode_item->requestor.id.vehicle_id_choice = VEHICLE_ID_STATION_ID;
	encode_item->requestor.id.station_id = 4294967295;*/	
	/*(5-2) type*/
	encode_item->requestor.mask_type = 1;
	encode_item->requestor.type.role = 22;
	encode_item->requestor.type.mask_subrole = 1;
	encode_item->requestor.type.subrole = 15;
	encode_item->requestor.type.mask_request = 1;
	encode_item->requestor.type.request = 15;
	encode_item->requestor.type.mask_iso3883 = 1;
	encode_item->requestor.type.iso3883 = 100;
	encode_item->requestor.type.mask_hpmsType = 1;
	encode_item->requestor.type.hpmsType = 15;
	encode_item->requestor.type.mask_regional = 0;
	/*(5-3) position*/
	encode_item->requestor.mask_position = 1;
	/*(5-3-1) position*/
	encode_item->requestor.position.position.lat = 900000001;
	encode_item->requestor.position.position.longitude = 1800000001;
	encode_item->requestor.position.position.mask_elevation = 1;
	encode_item->requestor.position.position.elevation = 61439;
	encode_item->requestor.position.position.mask_regional = 0;
	encode_item->requestor.position.position.regional_num = 0;
	encode_item->requestor.position.position.regional = NULL;
	/*(5-3-2) heading*/
	encode_item->requestor.position.mask_heading = 1;
	encode_item->requestor.position.heading = 28800;
	/*(5-3-3) speed*/
	encode_item->requestor.position.mask_speed = 1;
	encode_item->requestor.position.speed.transmission_state = 7;
	encode_item->requestor.position.speed.speed = 8191;
	/*(5-4) name*/
	encode_item->requestor.mask_name = 1;
	memset(encode_item->requestor.name, 0, 64);
	memcpy(encode_item->requestor.name, "SRM_RequestorDescription_name", strlen("SRM_RequestorDescription_name"));
	encode_item->requestor.name[strlen("SRM_RequestorDescription_name")+1] = '\0';
	/*(5-5) routeName*/
	encode_item->requestor.mask_routeName = 1;
	memset(encode_item->requestor.routeName, 0, 64);
	memcpy(encode_item->requestor.routeName, "SRM_RequestorDescription_routeName", strlen("SRM_RequestorDescription_routeName"));
	encode_item->requestor.routeName[strlen("SRM_RequestorDescription_routeName")+1] = '\0';
	/*(5-6) transitStatus*/
	encode_item->requestor.mask_transitStatus = 1;
	encode_item->requestor.transitStatus = 16;
	encode_item->requestor.mask_transitOccupancy = 1;
	encode_item->requestor.transitOccupancy = 7;
	encode_item->requestor.mask_transitSchedule = 1;
	encode_item->requestor.transitSchedule = 121;
	encode_item->requestor.mask_regional = 0;

	
	encode_item->mask_regional = 0;
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nSRM Encode Data:\n");
		_show_srm_standard_item(encode_item);
	}
	err_srm = j2735r63_srm_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_srm < 0)
	{				
		printf("srm encode fail...\n");				
		return -1;		
	}			
	_cmd_srm_free_struct(encode_item);		

	return 0;
}

int cmd_txSrm(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txSrm <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txSrm 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_srm_calloc(1, MAX_SRM_BUFF_SIZE);
	_encode_txSrm(encodeBuf, MAX_SRM_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_srm_free(encodeBuf);
	
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

	printf("%s", "cmd_txSrm command executed successfully.\n");
	return CLI_OK;
}

