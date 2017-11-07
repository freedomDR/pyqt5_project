#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_spat_api.h"
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

static int stop_spat;

/***************************************************************/
/***************************************************************/
void *j2735_spat_calloc(size_t nmemb, size_t size)
{
	unsigned char *ptr = NULL;

	ptr = linux_malloc(nmemb * size);
	(void) memset(ptr, 0, nmemb * size);

	return ptr;
}

void j2735_spat_free(void *ptr)
{
	linux_free(ptr);
}
/***************************************************************/
/***************************************************************/

void stop_spat_func(ULONG input)
{
	(void)input;
	stop_spat = 1;
	return;
}

void _cmd_spat_free_struct(spat_standard_item *free_spat_item)
{
	int i, j, k;
	
	if(free_spat_item == NULL){
		printf("[SPAT] Error: free_spat_struct() input is NULL\n");
	}else{
		if(free_spat_item->intersections != NULL){
			for(i = 0; i < free_spat_item->intersections_num; i++){
				if(free_spat_item->intersections[i].enabled_lanes != NULL){
					j2735_spat_free(free_spat_item->intersections[i].enabled_lanes);
				}

				if(free_spat_item->intersections[i].states != NULL){
					for(j = 0; j < free_spat_item->intersections[i].states_num; j++){
						if(free_spat_item->intersections[i].states[j].state_time_speed != NULL){
							for(k = 0; k < free_spat_item->intersections[i].states[j].state_time_speed_num; k++){
								if(free_spat_item->intersections[i].states[j].state_time_speed[k].speeds != NULL){
									j2735_spat_free(free_spat_item->intersections[i].states[j].state_time_speed[k].speeds);
								}
							}
							
							j2735_spat_free(free_spat_item->intersections[i].states[j].state_time_speed);
						}					

						if(free_spat_item->intersections[i].states[j].maneuver_assist_list != NULL){
							for(k = 0; k < free_spat_item->intersections[i].states[j].maneuver_assist_list_num; k++){
								if(free_spat_item->intersections[i].states[j].maneuver_assist_list[k].regional != NULL){									
									j2735_spat_free(free_spat_item->intersections[i].states[j].maneuver_assist_list[k].regional);
								}
							}						
							
							j2735_spat_free(free_spat_item->intersections[i].states[j].maneuver_assist_list);
						}						
					}
					
					j2735_spat_free(free_spat_item->intersections[i].states);
				}				

				if(free_spat_item->intersections[i].maneuver_assist_list != NULL){
					for(j = 0; j < free_spat_item->intersections[i].maneuver_assist_list_num; j++){
						if(free_spat_item->intersections[i].maneuver_assist_list[j].regional != NULL){							
							j2735_spat_free(free_spat_item->intersections[i].maneuver_assist_list[j].regional);
						}
					}
					
					j2735_spat_free(free_spat_item->intersections[i].maneuver_assist_list);
				}

				if(free_spat_item->intersections[i].regional != NULL){
					for(j = 0; j < free_spat_item->intersections[i].regional_num; j++){
						if(free_spat_item->intersections[i].regional[j].active_prioritizations != NULL){
							j2735_spat_free(free_spat_item->intersections[i].regional[j].active_prioritizations);
						}
					}
					
					j2735_spat_free(free_spat_item->intersections[i].regional);
				}
			}			

			j2735_spat_free(free_spat_item->intersections);
		}
		
		j2735_spat_free(free_spat_item);
	}
}

void _show_spat_standard_item(spat_standard_item *decoded_item)
{
	int i, j, k, m, n;

	printf("==========SPAT Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) timeStamp, OPTIONAL*/
	if(decoded_item->mask_time_stamp == 1){
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	}
	/*(2) name **OP**/
	if(decoded_item->mask_name == 1)
	{
		printf("name = %s\n", decoded_item->name);
	}
	/*(3) intersections */
	printf("intersections_num = %d\n", decoded_item->intersections_num);
	for(i = 0; i < decoded_item->intersections_num; i++)
	{
		/* (3-1) name **OP**/
		if(decoded_item->intersections[i].mask_name == 1)
		{
			printf("intersections[%d].name = %s\n", i, decoded_item->intersections[i].name);
		}
		/* (3-2) id */
		if(decoded_item->intersections[i].id.mask_region == 1)
		{
			printf("intersections[%d].id.region = %d\n", i, decoded_item->intersections[i].id.region);
		}
		printf("intersections[%d].id.id = %d\n", i, decoded_item->intersections[i].id.id);
		/* (3-3) revision */
		printf("intersections[%d].revision = %d\n", i, decoded_item->intersections[i].revision);
		/* (3-4) status */
		printf("intersections[%d].status = %hu\n", i, decoded_item->intersections[i].status);
		/* (3-5) moy **OP**/
		if(decoded_item->intersections[i].mask_moy == 1)
		{
			printf("intersections[%d].moy = %lu\n", i, decoded_item->intersections[i].moy);
		}
		/* (3-6) timeStamp **OP**/
		if(decoded_item->intersections[i].mask_time_stamp == 1)
		{
			printf("intersections[%d].time_stamp = %d\n", i, decoded_item->intersections[i].time_stamp);
		}
		/* (3-7) enabledLanes **OP**/
		if(decoded_item->intersections[i].mask_enabled_lanes == 1)
		{
			printf("intersections[%d].enabled_lanes_num = %d\n", i, decoded_item->intersections[i].enabled_lanes_num);
			for(j = 0; j < decoded_item->intersections[i].enabled_lanes_num; j++)
			{
				printf("intersections[%d].enabled_lanes[%d] = %d\n", i, j, decoded_item->intersections[i].enabled_lanes[j]);
			}		
		}
		/* (3-8) states */
		printf("intersections[%d].states_num = %d\n", i, decoded_item->intersections[i].states_num);
		for(j = 0; j < decoded_item->intersections[i].states_num; j++)
		{
			/* (3-8-1) movementName **OP**/
			if(decoded_item->intersections[i].states[j].mask_movement_name == 1)
			{
				printf("intersections[%d].states[%d].movement_name = %s\n", i, j, decoded_item->intersections[i].states[j].movement_name);
			}
			/* (3-8-2) signalGroup */
			printf("intersections[%d].states[%d].signal_group = %d\n", i, j, decoded_item->intersections[i].states[j].signal_group);
			/* (3-8-3) state_time_speed */
			printf("intersections[%d].states[%d].state_time_speed_num = %d\n", i, j, decoded_item->intersections[i].states[j].state_time_speed_num);
			for(k = 0; k < decoded_item->intersections[i].states[j].state_time_speed_num; k++)
			{
				/* (3-8-3-1) eventState */
				printf("intersections[%d].states[%d].state_time_speed[%d].event_state = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].event_state);
				/* (3-8-3-2) timing **OP**/
				if(decoded_item->intersections[i].states[j].state_time_speed[k].mask_timing == 1)
				{
					/* (3-8-3-2-1) startTime **OP**/
					if(decoded_item->intersections[i].states[j].state_time_speed[k].timing.mask_start_time == 1)
					{
						printf("intersections[%d].states[%d].state_time_speed[%d].timing.start_time = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].timing.start_time);
					}
					/* (3-8-3-2-2) minEndTime */
					printf("intersections[%d].states[%d].state_time_speed[%d].timing.min_end_time = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].timing.min_end_time);
					/* (3-8-3-2-3) maxEndTime **OP**/
					if(decoded_item->intersections[i].states[j].state_time_speed[k].timing.mask_max_end_time == 1)
					{
						printf("intersections[%d].states[%d].state_time_speed[%d].timing.max_end_time = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].timing.max_end_time);
					}
					/* (3-8-3-2-4) likelyTime **OP**/
					if(decoded_item->intersections[i].states[j].state_time_speed[k].timing.mask_likely_time == 1)
					{
						printf("intersections[%d].states[%d].state_time_speed[%d].timing.likely_time = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].timing.likely_time);
					}
					/* (3-8-3-2-5) confidence **OP**/
					if(decoded_item->intersections[i].states[j].state_time_speed[k].timing.mask_confidence == 1)
					{
						printf("intersections[%d].states[%d].state_time_speed[%d].timing.confidence = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].timing.confidence);
					}
					/* (3-8-3-2-6) nextTime **OP**/
					if(decoded_item->intersections[i].states[j].state_time_speed[k].timing.mask_next_time == 1)
					{
						printf("intersections[%d].states[%d].state_time_speed[%d].timing.next_time = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].timing.next_time);
					}
				}
				/* (3-8-3-3) speeds **OP**/
				if(decoded_item->intersections[i].states[j].state_time_speed[k].mask_speeds == 1)
				{
					printf("intersections[%d].states[%d].state_time_speed[%d].speeds_num = %d\n", i, j, k, decoded_item->intersections[i].states[j].state_time_speed[k].speeds_num);
					for(m = 0; m < decoded_item->intersections[i].states[j].state_time_speed[k].speeds_num; m++)
					{
						/* (3-8-3-3-1) type */
						printf("intersections[%d].states[%d].state_time_speed[%d].speeds[%d].type = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].type);
						/* (3-8-3-3-2) speed **OP**/
						if(decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_speed == 1)
						{
							printf("intersections[%d].states[%d].state_time_speed[%d].speeds[%d].speed = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].speed);
						}
						/* (3-8-3-3-3) confidence **OP**/
						if(decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_confidence == 1)
						{
							printf("intersections[%d].states[%d].state_time_speed[%d].speeds[%d].confidence = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].confidence);
						}
						/* (3-8-3-3-4) distance **OP**/
						if(decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_distance == 1)
						{
							printf("intersections[%d].states[%d].state_time_speed[%d].speeds[%d].distance = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].distance);
						}
						/* (3-8-3-3-5) Class **OP**/
						if(decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_as_class == 1)
						{
							printf("intersections[%d].states[%d].state_time_speed[%d].speeds[%d].as_class = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].state_time_speed[k].speeds[m].as_class);
						}
					}
				}
			}
			/* (3-8-4) maneuverAssistList **OP**/
			if(decoded_item->intersections[i].states[j].mask_maneuver_assist_list == 1)
			{
				printf("intersections[%d].states[%d].maneuver_assist_list_num = %d\n", i, j, decoded_item->intersections[i].states[j].maneuver_assist_list_num);
				for(k = 0; k < decoded_item->intersections[i].states[j].maneuver_assist_list_num; k++)
				{
					/* (3-8-4-1) connectionID */
					printf("intersections[%d].states[%d].maneuver_assist_list[%d].connection_id = %d\n", i, j, k, decoded_item->intersections[i].states[j].maneuver_assist_list[k].connection_id);
					/* (3-8-4-2) queueLength **OP**/
					if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].mask_queue_length == 1)
					{
						printf("intersections[%d].states[%d].maneuver_assist_list[%d].queue_length = %d\n", i, j, k, decoded_item->intersections[i].states[j].maneuver_assist_list[k].queue_length);
					}
					/* (3-8-4-3) availableStorageLength **OP**/
					if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].mask_available_storage_length == 1)
					{
						printf("intersections[%d].states[%d].maneuver_assist_list[%d].available_storage_length = %d\n", i, j, k, decoded_item->intersections[i].states[j].maneuver_assist_list[k].available_storage_length);
					}						
					/* (3-8-4-4) waitOnStop **OP**/
					if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].mask_wait_on_stop == 1)
					{
						printf("intersections[%d].states[%d].maneuver_assist_list[%d].wait_on_stop = %d\n", i, j, k, decoded_item->intersections[i].states[j].maneuver_assist_list[k].wait_on_stop);
					}
					/* (3-8-4-5) pedBicycleDetect **OP**/
					if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].mask_ped_bicycle_detect == 1)
					{
						printf("intersections[%d].states[%d].maneuver_assist_list[%d].ped_bicycle_detect = %d\n", i, j, k, decoded_item->intersections[i].states[j].maneuver_assist_list[k].ped_bicycle_detect);
					}
					/* (3-8-4-6) regional **OP**/
					if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].mask_regional == 1){
						printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional_num = %d\n", i, j, k, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional_num);
						for(m = 0; m < decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional_num; m++)
						{						
							printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].region_id = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].region_id);
							if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].region_id == ADD_GRP_C){
								printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].vehicle_to_lane_positions_num = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions_num);
								for(n = 0; n < decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions_num; n++)
								{
									printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].vehicle_to_lane_positions[%d].station_id = %lu\n", i, j, k, m, n, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions[n].station_id);
									printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].vehicle_to_lane_positions[%d].lane_id = %d\n", i, j, k, m, n, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions[n].lane_id);
								}

								if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].mask_rsu_distance_from_anchor == 1){
									printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.choice = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice);
									if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY1)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy1.x = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy1.x);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy1.y = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy1.y);
									}
									else if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY2)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy2.x = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy2.x);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy2.y = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy2.y);
									}
									else if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY3)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy3.x = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy3.x);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy3.y = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy3.y);
									}
									else if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY4)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy4.x = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy4.x);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy4.y = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy4.y);
									}
									else if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY5)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy5.x = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy5.x);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy5.y = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy5.y);
									}
									else if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY6)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy6.x = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy6.x);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy6.y = %d\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy6.y);
									}
									else if(decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_LATLON)
									{
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_lat_lon.lon = %li\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_lat_lon.lon);
										printf("intersections[%d].states[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_lat_lon.lat = %li\n", i, j, k, m, decoded_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_lat_lon.lat);
									}
								}
							}
						}
					}
				}
			}
		}
		/* (3-9) maneuverAssistList **OP**/
		if(decoded_item->intersections[i].mask_maneuver_assist_list == 1)
		{
			printf("intersections[%d].maneuver_assist_list_num = %d\n", i, decoded_item->intersections[i].maneuver_assist_list_num);
			for(j = 0; j < decoded_item->intersections[i].maneuver_assist_list_num; j++)
			{
				/* (3-9-1) connectionID */
				printf("intersections[%d].maneuver_assist_list[%d].connection_id = %d\n", i, j, decoded_item->intersections[i].maneuver_assist_list[j].connection_id);
				/* (3-9-2) queueLength **OP**/
				if(decoded_item->intersections[i].maneuver_assist_list[j].mask_queue_length == 1)
				{
					printf("intersections[%d].maneuver_assist_list[%d].queue_length = %d\n", i, j, decoded_item->intersections[i].maneuver_assist_list[j].queue_length);
				}
				/* (3-9-3) availableStorageLength **OP**/
				if(decoded_item->intersections[i].maneuver_assist_list[j].mask_available_storage_length == 1)
				{
					printf("intersections[%d].maneuver_assist_list[%d].available_storage_length = %d\n", i, j, decoded_item->intersections[i].maneuver_assist_list[j].available_storage_length);
				}						
				/* (3-9-4) waitOnStop **OP**/
				if(decoded_item->intersections[i].maneuver_assist_list[j].mask_wait_on_stop == 1)
				{
					printf("intersections[%d].maneuver_assist_list[%d].wait_on_stop = %d\n", i, j, decoded_item->intersections[i].maneuver_assist_list[j].wait_on_stop);
				}
				/* (3-9-5) pedBicycleDetect **OP**/
				if(decoded_item->intersections[i].maneuver_assist_list[j].mask_ped_bicycle_detect == 1)
				{
					printf("intersections[%d].maneuver_assist_list[%d].ped_bicycle_detect = %d\n", i, j, decoded_item->intersections[i].maneuver_assist_list[j].ped_bicycle_detect);
				}
				/* (3-9-6) regional **OP**/
				if(decoded_item->intersections[i].maneuver_assist_list[j].mask_regional == 1){
					printf("intersections[%d].maneuver_assist_list[%d].regional_num = %d\n", i, j, decoded_item->intersections[i].maneuver_assist_list[j].regional_num);
					for(k = 0; k < decoded_item->intersections[i].maneuver_assist_list[j].regional_num; k++)
					{						
						printf("intersections[%d].maneuver_assist_list[%d].regional[%d].region_id = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].region_id);
						if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].region_id == ADD_GRP_C){
							printf("intersections[%d].maneuver_assist_list[%d].regional[%d].vehicle_to_lane_positions_num = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions_num);
							for(m = 0; m < decoded_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions_num; m++)
							{
								printf("intersections[%d].maneuver_assist_list[%d].regional[%d].vehicle_to_lane_positions[%d].station_id = %lu\n", i, j, k, m, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions[m].station_id);
								printf("intersections[%d].maneuver_assist_list[%d].regional[%d].vehicle_to_lane_positions[%d].lane_id = %d\n", i, j, k, m, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions[m].lane_id);
							}

							if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].mask_rsu_distance_from_anchor == 1){
								printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.choice = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice);
								if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY1)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy1.x = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy1.x);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy1.y = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy1.y);
								}
								else if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY2)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy2.x = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy2.x);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy2.y = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy2.y);
								}
								else if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY3)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy3.x = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy3.x);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy3.y = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy3.y);
								}
								else if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY4)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy4.x = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy4.x);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy4.y = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy4.y);
								}
								else if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY5)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy5.x = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy5.x);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy5.y = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy5.y);
								}
								else if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_XY6)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy6.x = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy6.x);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_xy6.y = %d\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy6.y);
								}
								else if(decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice == NODE_OFFSET_POINTXY_LATLON)
								{
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_lat_lon.lon = %li\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_lat_lon.lon);
									printf("intersections[%d].maneuver_assist_list[%d].regional[%d].rsu_distance_from_anchor.node_lat_lon.lat = %li\n", i, j, k, decoded_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_lat_lon.lat);
								}
							}
						}
					}
				}
			}
		}
		/*(3-10) regional **OP**/
		if(decoded_item->intersections[i].mask_regional == 1)
		{
			printf("intersections[%d].regional_num = %d\n", i, decoded_item->intersections[i].regional_num);
			for(j = 0; j < decoded_item->intersections[i].regional_num; j++)
			{
				printf("intersections[%d].regional[%d].region_id = %d\n", i, j, decoded_item->intersections[i].regional[j].region_id);
				if(decoded_item->intersections[i].regional[j].mask_active_prioritizations == 1)
				{
					printf("intersections[%d].regional[%d].active_prioritizations_num = %d\n", i, j, decoded_item->intersections[i].regional[j].active_prioritizations_num);
					for(k = 0; k < decoded_item->intersections[i].regional[j].active_prioritizations_num; k++)
					{						
						printf("intersections[%d].regional[%d].active_prioritizations[%d].station_id = %lu\n", i, j, k, decoded_item->intersections[i].regional[j].active_prioritizations[k].station_id);
						printf("intersections[%d].regional[%d].active_prioritizations[%d].prior_state = %d\n", i, j, k, decoded_item->intersections[i].regional[j].active_prioritizations[k].prior_state);
						printf("intersections[%d].regional[%d].active_prioritizations[%d].signal_group = %d\n", i, j, k, decoded_item->intersections[i].regional[j].active_prioritizations[k].signal_group);
					}				
				}			
			}
		}
	}
	
	printf("===============================\n");
}

int cmd_rxSpat(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	spat_standard_item *decoded_item = NULL;
	int err_spat;

	int err, ret;
	TX_TIMER stop_spat_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxSpat <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxSpat 123\n");
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
	stop_spat = 0;
	tx_timer_create(&stop_spat_timer, NULL, stop_spat_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_spat == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (spat_standard_item *)j2735_spat_calloc(1, sizeof(spat_standard_item));
		err_spat = j2735r63_spat_decode(decoded_item, in_wsm.data, MAX_SPAT_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_spat < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_spat_standard_item(decoded_item);}
    
		j2735r63_spat_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_spat_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_spat(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	spat_standard_item *encode_item = NULL;
	spat_standard_item *decoded_item = NULL;	
	int err_spat, i, j, k, m, n, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 SPAT test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_spat_calloc(1, MAX_SPAT_BUFF_SIZE);
		encode_item = (spat_standard_item *)j2735_spat_calloc(1, sizeof(spat_standard_item));
		/*(1) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;
		/*(2) name **OP**/
		encode_item->mask_name = 1;
		memset(encode_item->name, 0, 64);
		memcpy(encode_item->name, "SPAT_name", strlen("SPAT_name"));
		encode_item->name[strlen("SPAT_name")+1] = '\0';
		/*(3) intersections */
		encode_item->intersections_num = 1;
		encode_item->intersections = (intersection_state_item *)j2735_spat_calloc(encode_item->intersections_num, sizeof(intersection_state_item));
		for(i = 0; i < encode_item->intersections_num; i++)
		{
			/* (3-1) name **OP**/
			encode_item->intersections[i].mask_name = 1;
			memset(encode_item->intersections[i].name, 0, 64);
			memcpy(&(encode_item->intersections[i].name), "intersections_name", strlen("intersections_name"));
			encode_item->intersections[i].name[strlen("intersections_name")+1] = '\0';
			/* (3-2) id */
			encode_item->intersections[i].id.mask_region = 1;		
			encode_item->intersections[i].id.region = 65535;		
			encode_item->intersections[i].id.id = 65534;
			/* (3-3) revision */
			encode_item->intersections[i].revision = 127;
			/* (3-4) status */
			encode_item->intersections[i].status = 1024;
			/* (3-5) moy **OP**/
			encode_item->intersections[i].mask_moy = 1;		
			encode_item->intersections[i].moy = 527040;		
			/* (3-6) timeStamp **OP**/
			encode_item->intersections[i].mask_time_stamp = 1;
			encode_item->intersections[i].time_stamp = 65533;		
			/* (3-7) enabledLanes **OP**/
			encode_item->intersections[i].mask_enabled_lanes = 1;		
			encode_item->intersections[i].enabled_lanes_num = 4;
			encode_item->intersections[i].enabled_lanes = (unsigned char *)j2735_spat_calloc(encode_item->intersections[i].enabled_lanes_num, sizeof(unsigned char));
			for(j = 0; j < encode_item->intersections[i].enabled_lanes_num; j++)
			{
				encode_item->intersections[i].enabled_lanes[j] = 255;
			}		
			/* (3-8) states */
			encode_item->intersections[i].states_num = 1;
			encode_item->intersections[i].states = (movement_state_item *)j2735_spat_calloc(encode_item->intersections[i].states_num, sizeof(movement_state_item));
			for(j = 0; j < encode_item->intersections[i].states_num; j++)
			{
				/* (3-8-1) movementName **OP**/
				encode_item->intersections[i].states[j].mask_movement_name = 1;
				memset(encode_item->intersections[i].states[j].movement_name, 0, 64);
				memcpy(&(encode_item->intersections[i].states[j].movement_name), "movement_name", strlen("movement_name"));
				encode_item->intersections[i].states[j].movement_name[strlen("movement_name")+1] = '\0';
				/* (3-8-2) signalGroup */
				encode_item->intersections[i].states[j].signal_group = 254;
				/* (3-8-3) state_time_speed */
				encode_item->intersections[i].states[j].state_time_speed_num = 1;
				encode_item->intersections[i].states[j].state_time_speed = (movement_event_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].state_time_speed_num, sizeof(movement_event_item));
				for(k = 0; k < encode_item->intersections[i].states[j].state_time_speed_num; k++)
				{
					/* (3-8-3-1) eventState */
					encode_item->intersections[i].states[j].state_time_speed[k].event_state = 9;
					/* (3-8-3-2) timing **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].mask_timing = 1;
						/* (3-8-3-2-1) startTime **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_start_time = 1;				
						encode_item->intersections[i].states[j].state_time_speed[k].timing.start_time = 36001;				
						/* (3-8-3-2-2) minEndTime */
						encode_item->intersections[i].states[j].state_time_speed[k].timing.min_end_time = 36001;
						/* (3-8-3-2-3) maxEndTime **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_max_end_time = 1;
						encode_item->intersections[i].states[j].state_time_speed[k].timing.max_end_time = 36000;				
						/* (3-8-3-2-4) likelyTime **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_likely_time = 1;
						encode_item->intersections[i].states[j].state_time_speed[k].timing.likely_time = 35999;				
						/* (3-8-3-2-5) confidence **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_confidence = 1;
						encode_item->intersections[i].states[j].state_time_speed[k].timing.confidence = 15;				
						/* (3-8-3-2-6) nextTime **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_next_time = 1;
						encode_item->intersections[i].states[j].state_time_speed[k].timing.next_time = 35998;				
					/* (3-8-3-3) speeds **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].mask_speeds = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].speeds_num = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].speeds = (advisory_speed_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].state_time_speed[k].speeds_num, sizeof(advisory_speed_item));	
					for(m = 0; m < encode_item->intersections[i].states[j].state_time_speed[k].speeds_num; m++)
					{
						/* (3-8-3-3-1) type */
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].type = 3;
						/* (3-8-3-3-2) speed **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_speed = 1;					
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].speed = 500;					
						/* (3-8-3-3-3) confidence **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_confidence = 1;					
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].confidence = 7;					
						/* (3-8-3-3-4) distance **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_distance = 1;					
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].distance = 10000;					
						/* (3-8-3-3-5) Class **OP**/
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_as_class = 1;
						encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].as_class = 255;
					}
				}
				/* (3-8-4) maneuverAssistList **OP**/
				encode_item->intersections[i].states[j].mask_maneuver_assist_list = 1;
				encode_item->intersections[i].states[j].maneuver_assist_list_num = 1;
				encode_item->intersections[i].states[j].maneuver_assist_list = (connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].maneuver_assist_list_num, sizeof(connection_maneuver_assist_item));	
				for(k = 0; k < encode_item->intersections[i].states[j].maneuver_assist_list_num; k++)
				{
					/* (3-8-4-1) connectionID */
					encode_item->intersections[i].states[j].maneuver_assist_list[k].connection_id = 255;
					/* (3-8-4-2) queueLength **OP**/
					encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_queue_length = 1;				
					encode_item->intersections[i].states[j].maneuver_assist_list[k].queue_length = 10000;				
					/* (3-8-4-3) availableStorageLength **OP**/
					encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_available_storage_length = 1;				
					encode_item->intersections[i].states[j].maneuver_assist_list[k].available_storage_length = 10000;										
					/* (3-8-4-4) waitOnStop **OP**/
					encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_wait_on_stop = 1;				
					encode_item->intersections[i].states[j].maneuver_assist_list[k].wait_on_stop = 0;
					/* (3-8-4-5) pedBicycleDetect **OP**/
					encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_ped_bicycle_detect = 1;				
					encode_item->intersections[i].states[j].maneuver_assist_list[k].ped_bicycle_detect = 0;
					/* (3-8-4-6) regional **OP**/
					encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_regional = 1;
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional_num = 1;
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional = (reg_connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].maneuver_assist_list[k].regional_num, sizeof(reg_connection_maneuver_assist_item));	
					for(m = 0; m < encode_item->intersections[i].states[j].maneuver_assist_list[k].regional_num; m++)
					{
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].region_id = ADD_GRP_C;
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions_num = 1;
						for(n = 0; n < encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions_num; n++)
						{
							encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions[n].station_id = 4294967295;
							encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions[n].lane_id = 255;
						}
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].mask_rsu_distance_from_anchor = 1;
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice = NODE_OFFSET_POINTXY_XY3;
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy3.x = 2047;
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy3.y = 2047;
					}					
				}			
			}
			/* (3-9) maneuverAssistList **OP**/
			encode_item->intersections[i].mask_maneuver_assist_list = 1;
			encode_item->intersections[i].maneuver_assist_list_num = 1;
			encode_item->intersections[i].maneuver_assist_list = (connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].maneuver_assist_list_num, sizeof(connection_maneuver_assist_item));	
			for(j = 0; j < encode_item->intersections[i].maneuver_assist_list_num; j++)
			{
				/* (3-9-1) connectionID */
				encode_item->intersections[i].maneuver_assist_list[j].connection_id = 255;
				/* (3-9-2) queueLength **OP**/
				encode_item->intersections[i].maneuver_assist_list[j].mask_queue_length = 1;			
				encode_item->intersections[i].maneuver_assist_list[j].queue_length = 10000;			
				/* (3-9-3) availableStorageLength **OP**/
				encode_item->intersections[i].maneuver_assist_list[j].mask_available_storage_length = 1;			
				encode_item->intersections[i].maneuver_assist_list[j].available_storage_length = 9999;									
				/* (3-9-4) waitOnStop **OP**/
				encode_item->intersections[i].maneuver_assist_list[j].mask_wait_on_stop = 1;			
				encode_item->intersections[i].maneuver_assist_list[j].wait_on_stop = 0;			
				/* (3-9-5) pedBicycleDetect **OP**/
				encode_item->intersections[i].maneuver_assist_list[j].mask_ped_bicycle_detect = 1;
				encode_item->intersections[i].maneuver_assist_list[j].ped_bicycle_detect = 0;
				/* (3-9-6) regional **OP**/
				encode_item->intersections[i].maneuver_assist_list[j].mask_regional = 1;
				encode_item->intersections[i].maneuver_assist_list[j].regional_num = 1;
				encode_item->intersections[i].maneuver_assist_list[j].regional = (reg_connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].maneuver_assist_list[j].regional_num, sizeof(reg_connection_maneuver_assist_item));	
				for(k = 0; k < encode_item->intersections[i].maneuver_assist_list[j].regional_num; k++)
				{
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].region_id = ADD_GRP_C;
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions_num = 1;
					for(m = 0; m < encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions_num; m++)
					{
						encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions[m].station_id = 4294967295;
						encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions[m].lane_id = 255;
					}
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].mask_rsu_distance_from_anchor = 1;
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice = NODE_OFFSET_POINTXY_XY4;
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy4.x = 4095;
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy4.y = 4095;
				}
			}		
			/* (3-10) regional **OP**/
			encode_item->intersections[i].mask_regional = 1;
			encode_item->intersections[i].regional_num = 1;
			encode_item->intersections[i].regional = (reg_intersection_state_item *)j2735_spat_calloc(encode_item->intersections[i].regional_num, sizeof(reg_intersection_state_item));	
			for(j = 0; j < encode_item->intersections[i].regional_num; j++)
			{
				encode_item->intersections[i].regional[j].region_id = ADD_GRP_C;
				encode_item->intersections[i].regional[j].mask_active_prioritizations = 1;
				encode_item->intersections[i].regional[j].active_prioritizations_num = 1;
				encode_item->intersections[i].regional[j].active_prioritizations = (prioritization_response_item *)j2735_spat_calloc(encode_item->intersections[i].regional[j].active_prioritizations_num, sizeof(prioritization_response_item));	
				for(k = 0; k < encode_item->intersections[i].regional[j].active_prioritizations_num; k++)
				{
					encode_item->intersections[i].regional[j].active_prioritizations[k].station_id = 4294967295;
					encode_item->intersections[i].regional[j].active_prioritizations[k].prior_state = 7;
					encode_item->intersections[i].regional[j].active_prioritizations[k].signal_group = 255;
				}
			}			
		}

		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("SPAT loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_spat_standard_item(&encode_item);}

		err_spat = j2735r63_spat_encode(encode_item, encodeBuf, MAX_SPAT_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_spat < 0)
		{
			printf("spat encode fail...\n");		
			return -1;	
		}
		_cmd_spat_free_struct(encode_item);

		decoded_item = (spat_standard_item *)j2735_spat_calloc(1, sizeof(spat_standard_item));
		err_spat = j2735r63_spat_decode(decoded_item, encodeBuf, MAX_SPAT_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_spat < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_spat_standard_item(decoded_item);}	
		
		j2735r63_spat_free_struct(decoded_item);
		j2735_spat_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}

	printf("%s", "J2735R63 SPAT Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txSpat(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, j, k, m, n, err_spat;
	spat_standard_item *encode_item = NULL;

	encode_item = (spat_standard_item *)j2735_spat_calloc(1, sizeof(spat_standard_item));
	/*(1) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;
	/*(2) name **OP**/
	encode_item->mask_name = 1;
	memset(encode_item->name, 0, 64);
	memcpy(encode_item->name, "SPAT_name", strlen("SPAT_name"));
	encode_item->name[strlen("SPAT_name")+1] = '\0';
	/*(3) intersections */
	encode_item->intersections_num = 1;
	encode_item->intersections = (intersection_state_item *)j2735_spat_calloc(encode_item->intersections_num, sizeof(intersection_state_item));
	for(i = 0; i < encode_item->intersections_num; i++)
	{
		/* (3-1) name **OP**/
		encode_item->intersections[i].mask_name = 1;
		memset(encode_item->intersections[i].name, 0, 64);
		memcpy(&(encode_item->intersections[i].name), "intersections_name", strlen("intersections_name"));
		encode_item->intersections[i].name[strlen("intersections_name")+1] = '\0';
		/* (3-2) id */
		encode_item->intersections[i].id.mask_region = 1;		
		encode_item->intersections[i].id.region = 65535;		
		encode_item->intersections[i].id.id = 65534;
		/* (3-3) revision */
		encode_item->intersections[i].revision = 127;
		/* (3-4) status */
		encode_item->intersections[i].status = 1024;
		/* (3-5) moy **OP**/
		encode_item->intersections[i].mask_moy = 1;		
		encode_item->intersections[i].moy = 527040;		
		/* (3-6) timeStamp **OP**/
		encode_item->intersections[i].mask_time_stamp = 1;
		encode_item->intersections[i].time_stamp = 65533;		
		/* (3-7) enabledLanes **OP**/
		encode_item->intersections[i].mask_enabled_lanes = 1;		
		encode_item->intersections[i].enabled_lanes_num = 4;
		encode_item->intersections[i].enabled_lanes = (unsigned char *)j2735_spat_calloc(encode_item->intersections[i].enabled_lanes_num, sizeof(unsigned char));
		for(j = 0; j < encode_item->intersections[i].enabled_lanes_num; j++)
		{
			encode_item->intersections[i].enabled_lanes[j] = 255;
		}		
		/* (3-8) states */
		encode_item->intersections[i].states_num = 1;
		encode_item->intersections[i].states = (movement_state_item *)j2735_spat_calloc(encode_item->intersections[i].states_num, sizeof(movement_state_item));
		for(j = 0; j < encode_item->intersections[i].states_num; j++)
		{
			/* (3-8-1) movementName **OP**/
			encode_item->intersections[i].states[j].mask_movement_name = 1;
			memset(encode_item->intersections[i].states[j].movement_name, 0, 64);
			memcpy(&(encode_item->intersections[i].states[j].movement_name), "movement_name", strlen("movement_name"));
			encode_item->intersections[i].states[j].movement_name[strlen("movement_name")+1] = '\0';
			/* (3-8-2) signalGroup */
			encode_item->intersections[i].states[j].signal_group = 254;
			/* (3-8-3) state_time_speed */
			encode_item->intersections[i].states[j].state_time_speed_num = 1;
			encode_item->intersections[i].states[j].state_time_speed = (movement_event_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].state_time_speed_num, sizeof(movement_event_item));
			for(k = 0; k < encode_item->intersections[i].states[j].state_time_speed_num; k++)
			{
				/* (3-8-3-1) eventState */
				encode_item->intersections[i].states[j].state_time_speed[k].event_state = 9;
				/* (3-8-3-2) timing **OP**/
				encode_item->intersections[i].states[j].state_time_speed[k].mask_timing = 1;
					/* (3-8-3-2-1) startTime **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_start_time = 1;				
					encode_item->intersections[i].states[j].state_time_speed[k].timing.start_time = 36001;				
					/* (3-8-3-2-2) minEndTime */
					encode_item->intersections[i].states[j].state_time_speed[k].timing.min_end_time = 36001;
					/* (3-8-3-2-3) maxEndTime **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_max_end_time = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].timing.max_end_time = 36000;				
					/* (3-8-3-2-4) likelyTime **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_likely_time = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].timing.likely_time = 35999;				
					/* (3-8-3-2-5) confidence **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_confidence = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].timing.confidence = 15;				
					/* (3-8-3-2-6) nextTime **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].timing.mask_next_time = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].timing.next_time = 35998;				
				/* (3-8-3-3) speeds **OP**/
				encode_item->intersections[i].states[j].state_time_speed[k].mask_speeds = 1;
				encode_item->intersections[i].states[j].state_time_speed[k].speeds_num = 1;
				encode_item->intersections[i].states[j].state_time_speed[k].speeds = (advisory_speed_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].state_time_speed[k].speeds_num, sizeof(advisory_speed_item));	
				for(m = 0; m < encode_item->intersections[i].states[j].state_time_speed[k].speeds_num; m++)
				{
					/* (3-8-3-3-1) type */
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].type = 3;
					/* (3-8-3-3-2) speed **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_speed = 1;					
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].speed = 500;					
					/* (3-8-3-3-3) confidence **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_confidence = 1;					
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].confidence = 7;					
					/* (3-8-3-3-4) distance **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_distance = 1;					
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].distance = 10000;					
					/* (3-8-3-3-5) Class **OP**/
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].mask_as_class = 1;
					encode_item->intersections[i].states[j].state_time_speed[k].speeds[m].as_class = 255;
				}
			}
			/* (3-8-4) maneuverAssistList **OP**/
			encode_item->intersections[i].states[j].mask_maneuver_assist_list = 1;
			encode_item->intersections[i].states[j].maneuver_assist_list_num = 1;
			encode_item->intersections[i].states[j].maneuver_assist_list = (connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].maneuver_assist_list_num, sizeof(connection_maneuver_assist_item));	
			for(k = 0; k < encode_item->intersections[i].states[j].maneuver_assist_list_num; k++)
			{
				/* (3-8-4-1) connectionID */
				encode_item->intersections[i].states[j].maneuver_assist_list[k].connection_id = 255;
				/* (3-8-4-2) queueLength **OP**/
				encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_queue_length = 1;				
				encode_item->intersections[i].states[j].maneuver_assist_list[k].queue_length = 10000;				
				/* (3-8-4-3) availableStorageLength **OP**/
				encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_available_storage_length = 1;				
				encode_item->intersections[i].states[j].maneuver_assist_list[k].available_storage_length = 10000;										
				/* (3-8-4-4) waitOnStop **OP**/
				encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_wait_on_stop = 1;				
				encode_item->intersections[i].states[j].maneuver_assist_list[k].wait_on_stop = 0;
				/* (3-8-4-5) pedBicycleDetect **OP**/
				encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_ped_bicycle_detect = 1;				
				encode_item->intersections[i].states[j].maneuver_assist_list[k].ped_bicycle_detect = 0;
				/* (3-8-4-6) regional **OP**/
				encode_item->intersections[i].states[j].maneuver_assist_list[k].mask_regional = 1;
				encode_item->intersections[i].states[j].maneuver_assist_list[k].regional_num = 1;
				encode_item->intersections[i].states[j].maneuver_assist_list[k].regional = (reg_connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].states[j].maneuver_assist_list[k].regional_num, sizeof(reg_connection_maneuver_assist_item));	
				for(m = 0; m < encode_item->intersections[i].states[j].maneuver_assist_list[k].regional_num; m++)
				{
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].region_id = ADD_GRP_C;
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions_num = 1;
					for(n = 0; n < encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions_num; n++)
					{
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions[n].station_id = 4294967295;
						encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].vehicle_to_lane_positions[n].lane_id = 255;
					}
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.choice = NODE_OFFSET_POINTXY_XY3;
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy3.x = 2047;
					encode_item->intersections[i].states[j].maneuver_assist_list[k].regional[m].rsu_distance_from_anchor.node_xy3.y = 2047;
				}					
			}			
		}
		/* (3-9) maneuverAssistList **OP**/
		encode_item->intersections[i].mask_maneuver_assist_list = 1;
		encode_item->intersections[i].maneuver_assist_list_num = 1;
		encode_item->intersections[i].maneuver_assist_list = (connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].maneuver_assist_list_num, sizeof(connection_maneuver_assist_item));	
		for(j = 0; j < encode_item->intersections[i].maneuver_assist_list_num; j++)
		{
			/* (3-9-1) connectionID */
			encode_item->intersections[i].maneuver_assist_list[j].connection_id = 255;
			/* (3-9-2) queueLength **OP**/
			encode_item->intersections[i].maneuver_assist_list[j].mask_queue_length = 1;			
			encode_item->intersections[i].maneuver_assist_list[j].queue_length = 10000;			
			/* (3-9-3) availableStorageLength **OP**/
			encode_item->intersections[i].maneuver_assist_list[j].mask_available_storage_length = 1;			
			encode_item->intersections[i].maneuver_assist_list[j].available_storage_length = 9999;									
			/* (3-9-4) waitOnStop **OP**/
			encode_item->intersections[i].maneuver_assist_list[j].mask_wait_on_stop = 1;			
			encode_item->intersections[i].maneuver_assist_list[j].wait_on_stop = 0;			
			/* (3-9-5) pedBicycleDetect **OP**/
			encode_item->intersections[i].maneuver_assist_list[j].mask_ped_bicycle_detect = 1;
			encode_item->intersections[i].maneuver_assist_list[j].ped_bicycle_detect = 0;
			/* (3-9-6) regional **OP**/
			encode_item->intersections[i].maneuver_assist_list[j].mask_regional = 1;
			encode_item->intersections[i].maneuver_assist_list[j].regional_num = 1;
			encode_item->intersections[i].maneuver_assist_list[j].regional = (reg_connection_maneuver_assist_item *)j2735_spat_calloc(encode_item->intersections[i].maneuver_assist_list[j].regional_num, sizeof(reg_connection_maneuver_assist_item));	
			for(k = 0; k < encode_item->intersections[i].maneuver_assist_list[j].regional_num; k++)
			{
				encode_item->intersections[i].maneuver_assist_list[j].regional[k].region_id = ADD_GRP_C;
				encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions_num = 1;
				for(m = 0; m < encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions_num; m++)
				{
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions[m].station_id = 4294967295;
					encode_item->intersections[i].maneuver_assist_list[j].regional[k].vehicle_to_lane_positions[m].lane_id = 255;
				}
				encode_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.choice = NODE_OFFSET_POINTXY_XY4;
				encode_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy4.x = 4095;
				encode_item->intersections[i].maneuver_assist_list[j].regional[k].rsu_distance_from_anchor.node_xy4.y = 4095;
			}
		}		
		/* (3-10) regional **OP**/
		encode_item->intersections[i].mask_regional = 1;
		encode_item->intersections[i].regional_num = 1;
		encode_item->intersections[i].regional = (reg_intersection_state_item *)j2735_spat_calloc(encode_item->intersections[i].regional_num, sizeof(reg_intersection_state_item));	
		for(j = 0; j < encode_item->intersections[i].regional_num; j++)
		{
			encode_item->intersections[i].regional[j].region_id = ADD_GRP_C;
			encode_item->intersections[i].regional[j].mask_active_prioritizations = 1;
			encode_item->intersections[i].regional[j].active_prioritizations_num = 1;
			encode_item->intersections[i].regional[j].active_prioritizations = (prioritization_response_item *)j2735_spat_calloc(encode_item->intersections[i].regional[j].active_prioritizations_num, sizeof(prioritization_response_item));	
			for(k = 0; k < encode_item->intersections[i].regional[j].active_prioritizations_num; k++)
			{
				encode_item->intersections[i].regional[j].active_prioritizations[k].station_id = 4294967295;
				encode_item->intersections[i].regional[j].active_prioritizations[k].prior_state = 7;
				encode_item->intersections[i].regional[j].active_prioritizations[k].signal_group = 255;
			}
		}			
	}

	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nSPAT Encode Data:\n");
		_show_spat_standard_item(encode_item);
	}

	err_spat = j2735r63_spat_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_spat < 0)
	{		
		printf("spat encode fail...\n");		
		return -1;	
	}		
	_cmd_spat_free_struct(encode_item);
	
	return 0;
}

int cmd_txSpat(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txSpat <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txSpat 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_spat_calloc(1, MAX_SPAT_BUFF_SIZE);
	_encode_txSpat(encodeBuf, MAX_SPAT_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_spat_free(encodeBuf);

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

	printf("%s", "cmd_txSpat command executed successfully.\n");
	return CLI_OK;
}

