#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_bsm_api.h"
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

static int stop_bsm;

/***************************************************************/
/***************************************************************/
void *j2735_bsm_calloc(size_t nmemb, size_t size)
{
	unsigned char *ptr = NULL;

	ptr = linux_malloc(nmemb * size);
	(void) memset(ptr, 0, nmemb * size);

	return ptr;
}

void j2735_bsm_free(void *ptr)
{
	linux_free(ptr);
}
/***************************************************************/
/***************************************************************/

void stop_bsm_func(ULONG input)
{
	(void)input;
	stop_bsm = 1;
	return;
}

void _cmd_bsm_free_struct(bsm_standard_item *free_bsm_item)
{	
	int i, j;

	if(free_bsm_item == NULL){
		printf("[BSM] Error: free_bsm_struct() input is NULL\n");
	}else
	{
		/*(1) core_data */
		/*(2) PartII*/
		if(free_bsm_item->partII != NULL)
		{
			for(i = 0; i < free_bsm_item->partII_num; i++){
				if(free_bsm_item->partII[i].vse != NULL){
					if(free_bsm_item->partII[i].vse->path_history != NULL){
						if(free_bsm_item->partII[i].vse->path_history->initial_position != NULL){
							if(free_bsm_item->partII[i].vse->path_history->initial_position->utc_time != NULL){
								j2735_bsm_free(free_bsm_item->partII[i].vse->path_history->initial_position->utc_time);
							}
							j2735_bsm_free(free_bsm_item->partII[i].vse->path_history->initial_position);
						}					

						if(free_bsm_item->partII[i].vse->path_history->crumb_data != NULL){							
							j2735_bsm_free(free_bsm_item->partII[i].vse->path_history->crumb_data);
						}
						j2735_bsm_free(free_bsm_item->partII[i].vse->path_history);
					}
					j2735_bsm_free(free_bsm_item->partII[i].vse);
				}

				if(free_bsm_item->partII[i].spve != NULL){
					if(free_bsm_item->partII[i].spve->trailers.units != NULL){
						for(j = 0; j < free_bsm_item->partII[i].spve->trailers.units_num; j++){
							if(free_bsm_item->partII[i].spve->trailers.units[j].crumb_data != NULL){
								j2735_bsm_free(free_bsm_item->partII[i].spve->trailers.units[j].crumb_data);
							}
						}
						j2735_bsm_free(free_bsm_item->partII[i].spve->trailers.units);
					}
					j2735_bsm_free(free_bsm_item->partII[i].spve);
				}

				if(free_bsm_item->partII[i].suve != NULL){
					if(free_bsm_item->partII[i].suve->the_rtcm.msgs != NULL){
						j2735_bsm_free(free_bsm_item->partII[i].suve->the_rtcm.msgs);
					}
					j2735_bsm_free(free_bsm_item->partII[i].suve);
				}
			}
			j2735_bsm_free(free_bsm_item->partII);
		}
		j2735_bsm_free(free_bsm_item);
	}
}

void _show_bsm_standard_item(bsm_standard_item *decoded_item)
{
	int i, j, k;

	printf("==========BSM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*coreData*/
	/*(1-1) msgCnt*/
	printf("core_data.msg_cnt = %d\n", decoded_item->core_data.msg_cnt);
	/*(1-2) id*/
	printf("core_data.id = %x %x %x %x\n", decoded_item->core_data.id[0], decoded_item->core_data.id[1], decoded_item->core_data.id[2], decoded_item->core_data.id[3]);
	/*(1-3) secMark*/
	printf("core_data.sec_mark = %hu\n", decoded_item->core_data.sec_mark);
	/*(1-4) lat*/
	printf("core_data.lat = %d\n", decoded_item->core_data.lat);
	/*(1-5) Long*/
	printf("core_data.longitude = %d\n", decoded_item->core_data.longitude);
	/*(1-6) elev*/
	printf("core_data.elev = %d\n", decoded_item->core_data.elev);
	/*(1-7) accuracy*/
	printf("core_data.accuracy.semi_major = %d\n", decoded_item->core_data.accuracy.semi_major);
	printf("core_data.accuracy.semi_minor = %d\n", decoded_item->core_data.accuracy.semi_minor);
	printf("core_data.accuracy.orientation = %hu\n", decoded_item->core_data.accuracy.orientation);	
	/*(1-8) transmission*/
	printf("core_data.transmission_state = %d\n", decoded_item->core_data.transmission_state);
	/*(1-9) speed*/
	printf("core_data.speed = %hu\n", decoded_item->core_data.speed);
	/*(1-10) heading*/
	printf("core_data.heading = %hu\n", decoded_item->core_data.heading);
	/*(1-11) angle*/
	printf("core_data.angle = %d\n", decoded_item->core_data.angle);
	/*(1-12) accelSet*/
	printf("core_data.accel_set.longitude = %hu\n", decoded_item->core_data.accel_set.longitude);
	printf("core_data.accel_set.lat = %hu\n", decoded_item->core_data.accel_set.lat);
	printf("core_data.accel_set.vert = %d\n", decoded_item->core_data.accel_set.vert);
	printf("core_data.accel_set.yaw = %hi\n", decoded_item->core_data.accel_set.yaw);
	/*(1-13) brakes*/
	printf("core_data.brakes.wheel_brakes = %d\n", decoded_item->core_data.brakes.wheel_brakes);
	printf("core_data.brakes.traction = %d\n", decoded_item->core_data.brakes.traction);
	printf("core_data.brakes.abs = %d\n", decoded_item->core_data.brakes.abs);
	printf("core_data.brakes.scs = %d\n", decoded_item->core_data.brakes.scs);
	printf("core_data.brakes.brake_boost = %d\n", decoded_item->core_data.brakes.brake_boost);
	printf("core_data.brakes.aux_brakes = %d\n", decoded_item->core_data.brakes.aux_brakes);
	/*(1-14) size*/
	printf("core_data.size.width = %hu\n", decoded_item->core_data.size.width);
	printf("core_data.size.length = %hu\n", decoded_item->core_data.size.length);
	/*partII*/
	if(decoded_item->mask_partII == 1){
		for(i = 0; i < decoded_item->partII_num; i++)
		{			
			if(decoded_item->partII[i].partII_id == VEHICLE_SAFETY_EXT){
				/*(1) vse->events, OPTIONAL*/
				if(decoded_item->partII[i].vse->mask_events == 1){
					printf("partII[%d].vse->events = %hu\n", i, decoded_item->partII[i].vse->events);
				}				
				/*(2) vse->path_history, OPTIONAL*/
				if(decoded_item->partII[i].vse->mask_path_history == 1){
					/*(2-1) initialPosition*/
					if(decoded_item->partII[i].vse->path_history->mask_initial_position == 1){
						/*(2-1-1) utcTime, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_utc_time == 1){
							/*(2-1-1-1) year*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_year == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->year = %hu\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->year);
							}
							/*(2-1-1-2) month*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_month == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->month = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->month);
							}
							/*(2-1-1-3) day*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_day == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->day = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->day);
							}
							/*(2-1-1-4) hour*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_hour == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->hour = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->hour);
							}
							/*(2-1-1-5) minute*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_minute == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->minute = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->minute);
							}
							/*(2-1-1-6) second*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_second == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->second = %hu\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->second);
							}
							/*(2-1-1-7) offset*/
							if(decoded_item->partII[i].vse->path_history->initial_position->utc_time->mask_offset == 1){
								printf("partII[%d].vse->path_history->initial_position->utc_time->offset = %hi\n", i, decoded_item->partII[i].vse->path_history->initial_position->utc_time->offset);
							}
						}							
						/*(2-1-2) Long*/
						printf("partII[%d].vse->path_history->initial_position->longitude = %ld\n", i, decoded_item->partII[i].vse->path_history->initial_position->longitude);
						/*(2-1-3) lat*/
						printf("partII[%d].vse->path_history->initial_position->lat = %ld\n", i, decoded_item->partII[i].vse->path_history->initial_position->lat);
						/*(2-1-4) elevation, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_elevation == 1){
							printf("partII[%d].vse->path_history->initial_position->elevation = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->elevation);
						}						
						/*(2-1-5) heading, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_heading == 1){
							printf("partII[%d].vse->path_history->initial_position->heading = %hu\n", i, decoded_item->partII[i].vse->path_history->initial_position->heading);
						}						
						/*(2-1-6) speed, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_speed == 1){
							printf("partII[%d].vse->path_history->initial_position->speed.transmission_state = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->speed.transmission_state);
							printf("partII[%d].vse->path_history->initial_position->speed.speed = %hu\n", i, decoded_item->partII[i].vse->path_history->initial_position->speed.speed);
						}							
						/*(2-1-7) posAccuracy, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_pos_accuracy == 1){
							printf("partII[%d].vse->path_history->initial_position->pos_accuracy.semi_major = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->pos_accuracy.semi_major);
							printf("partII[%d].vse->path_history->initial_position->pos_accuracy.semi_minor = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->pos_accuracy.semi_minor);
							printf("partII[%d].vse->path_history->initial_position->pos_accuracy.orientation = %hu\n", i, decoded_item->partII[i].vse->path_history->initial_position->pos_accuracy.orientation);
						}							
						/*(2-1-8) timeConfidence, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_time_confidence == 1){
							printf("partII[%d].vse->path_history->initial_position->time_confidence = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->time_confidence);
						}						
						/*(2-1-9) posConfidence, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_pos_confidence == 1){
							printf("partII[%d].vse->path_history->initial_position->pos_confidence.pos = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->pos_confidence.pos);
							printf("partII[%d].vse->path_history->initial_position->pos_confidence.elevation = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->pos_confidence.elevation);
						}						
						/*(2-1-10) speedCondifence, OPTIONAL*/
						if(decoded_item->partII[i].vse->path_history->initial_position->mask_speed_confidence == 1){
							printf("partII[%d].vse->path_history->initial_position->speed_confidence.heading = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->speed_confidence.heading);
							printf("partII[%d].vse->path_history->initial_position->speed_confidence.speed = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->speed_confidence.speed);
							printf("partII[%d].vse->path_history->initial_position->speed_confidence.throttle = %d\n", i, decoded_item->partII[i].vse->path_history->initial_position->speed_confidence.throttle);
						}						
					}						
					/*(2-2) currGNSSstatus, OPTIONAL*/
					if(decoded_item->partII[i].vse->path_history->mask_curr_gps_status == 1){
						printf("partII[%d].vse->path_history->curr_gps_status = %d\n", i, decoded_item->partII[i].vse->path_history->curr_gps_status);
					}					
					/*(2-3) crumbData*/					
					for(j = 0; j < decoded_item->partII[i].vse->path_history->crumb_data_num; j++)
					{
						/*(2-3-1) latOffset */
						printf("partII[%d].vse->path_history->crumb_data[%d].lat_offset = %d\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].lat_offset);
						/*(2-3-2) lonOffset */
						printf("partII[%d].vse->path_history->crumb_data[%d].lon_offset = %d\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].lon_offset);
						/*(2-3-3) elevationOffset */
						printf("partII[%d].vse->path_history->crumb_data[%d].elevation_offset = %d\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].elevation_offset);
						/*(2-3-4) timeOffset */
						printf("partII[%d].vse->path_history->crumb_data[%d].time_offset = %hu\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].time_offset);
						/*(2-3-5) speed, OPTIONAL */
						if(decoded_item->partII[i].vse->path_history->crumb_data[j].mask_speed == 1){
							printf("partII[%d].vse->path_history->crumb_data[%d].speed = %hu\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].speed);						
						}
						/*(2-3-6) posAccuracy, OPTIONAL */
						if(decoded_item->partII[i].vse->path_history->crumb_data[j].mask_pos_accuracy == 1){
							printf("partII[%d].vse->path_history->crumb_data[%d].pos_accuracy.semi_major = %d\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.semi_major);
							printf("partII[%d].vse->path_history->crumb_data[%d].pos_accuracy.semi_minor = %d\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.semi_minor);
							printf("partII[%d].vse->path_history->crumb_data[%d].pos_accuracy.orientation = %hu\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.orientation);
						}
						/*(2-3-7) heading, OPTIONAL */
						if(decoded_item->partII[i].vse->path_history->crumb_data[j].mask_heading == 1){
							printf("partII[%d].vse->path_history->crumb_data[%d].heading = %d\n", i, j, decoded_item->partII[i].vse->path_history->crumb_data[j].heading);
						}					
					}					
				}					
				/*(3) vse->path_prediction, OPTIONAL*/
				if(decoded_item->partII[i].vse->mask_path_prediction == 1){
					printf("partII[%d].vse->path_prediction.radius_of_curve = %d\n", i, decoded_item->partII[i].vse->path_prediction.radius_of_curve);
					printf("partII[%d].vse->path_prediction.confidence = %d\n", i, decoded_item->partII[i].vse->path_prediction.confidence);
				}					
				/*(4) vse->lights, OPTIONAL*/
				if(decoded_item->partII[i].vse->mask_lights == 1){
					printf("partII[%d].vse->lights = %hu\n", i, decoded_item->partII[i].vse->lights);
				}
			}else if(decoded_item->partII[i].partII_id == SPECIAL_VEHICLE_EXT){
				/*(1) spve->vehicleAlerts, OPTIONAL*/
				if(decoded_item->partII[i].spve->mask_vehicle_alerts == 1){
					/*(1-1) spve->vehicleAlerts->sspRights*/
					printf("partII[%d].spve->vehicle_alerts.ssp_rights = %d\n", i, decoded_item->partII[i].spve->vehicle_alerts.ssp_rights);
					/*(1-2) spve->vehicleAlerts->sirenUse*/
					printf("partII[%d].spve->vehicle_alerts.siren_use = %d\n", i, decoded_item->partII[i].spve->vehicle_alerts.siren_use);
					/*(1-3) spve->vehicleAlerts->lightsUse*/
					printf("partII[%d].spve->vehicle_alerts.lights_use = %d\n", i, decoded_item->partII[i].spve->vehicle_alerts.lights_use);
					/*(1-4) spve->vehicleAlerts->multi*/
					printf("partII[%d].spve->vehicle_alerts.multi = %d\n", i, decoded_item->partII[i].spve->vehicle_alerts.multi);
					/*(1-5) spve->vehicleAlerts->events, OPTIONAL*/
					if(decoded_item->partII[i].spve->vehicle_alerts.mask_events == 1){
						printf("partII[%d].spve->vehicle_alerts.events.ssp_rights = %d\n", i, decoded_item->partII[i].spve->vehicle_alerts.events.ssp_rights);
						printf("partII[%d].spve->vehicle_alerts.events.event = %hu\n", i, decoded_item->partII[i].spve->vehicle_alerts.events.event);
					}						
					/*(1-6) spve->vehicleAlerts->responseType, OPTIONAL*/
					if(decoded_item->partII[i].spve->vehicle_alerts.mask_response_type == 1){
						printf("partII[%d].spve->vehicle_alerts.response_type = %d\n", i, decoded_item->partII[i].spve->vehicle_alerts.response_type);
					}
				}					
				/*(2) spve->description, OPTIONAL*/
				if(decoded_item->partII[i].spve->mask_description == 1){
					/*(2-1) spve->description->typeEvent*/
					printf("partII[%d].spve->description.type_event = %hu\n", i, decoded_item->partII[i].spve->description.type_event);
					/*(2-2) spve->description->description, OPTIONAL*/
					if(decoded_item->partII[i].spve->description.mask_description == 1){
						for(j = 0; j < decoded_item->partII[i].spve->description.description_num; j++)
						{
							printf("partII[%d].spve->description.description[%d] = %hu\n", i, j, decoded_item->partII[i].spve->description.description[j]);
						}						
					}					
					/*(2-3) spve->description->priority, OPTIONAL*/
					if(decoded_item->partII[i].spve->description.mask_priority == 1){
						printf("partII[%d].spve->description.priority = %d\n", i, decoded_item->partII[i].spve->description.priority);
					}					
					/*(2-4) spve->description->heading, OPTIONAL*/
					if(decoded_item->partII[i].spve->description.mask_heading == 1){
						printf("partII[%d].spve->description.heading = %hu\n", i, decoded_item->partII[i].spve->description.heading);
					}					
					/*(2-5) spve->description->extent, OPTIONAL*/
					if(decoded_item->partII[i].spve->description.mask_extent == 1){
						printf("partII[%d].spve->description.extent = %d\n", i, decoded_item->partII[i].spve->description.extent);
					}					
				}					
				/*(3) spve->trailers, OPTIONAL*/
				if(decoded_item->partII[i].spve->mask_trailers == 1){					
					/*(3-1) spve->trailers->sspRights*/
					printf("partII[%d].spve->trailers.ssp_rights = %d\n", i, decoded_item->partII[i].spve->trailers.ssp_rights);
					/*(3-2) spve->trailers->connection*/
					printf("partII[%d].spve->trailers.connection.pivot_offset = %hi\n", i, decoded_item->partII[i].spve->trailers.connection.pivot_offset);
					printf("partII[%d].spve->trailers.connection.pivot_angle = %hu\n", i, decoded_item->partII[i].spve->trailers.connection.pivot_angle);
					printf("partII[%d].spve->trailers.connection.pivots = %hi\n", i, decoded_item->partII[i].spve->trailers.connection.pivots);
					/*(3-3) spve->trailers->units*/
					for(j = 0; j < decoded_item->partII[i].spve->trailers.units_num; j++)
					{
						/*(3-3-1) isDolly */
						printf("partII[%d].spve->trailers.units[%d].is_dolly = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].is_dolly);
						/*(3-3-2) width */
						printf("partII[%d].spve->trailers.units[%d].width = %hu\n", i, j, decoded_item->partII[i].spve->trailers.units[j].width);
						/*(3-3-3) length */
						printf("partII[%d].spve->trailers.units[%d].length = %hu\n", i, j, decoded_item->partII[i].spve->trailers.units[j].length);
						/*(3-3-4) height, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_height == 1){
							printf("partII[%d].spve->trailers.units[%d].height = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].height);
						}
						/*(3-3-5) mass, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_mass == 1){
							printf("partII[%d].spve->trailers.units[%d].mass = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].mass);
						}
						/*(3-3-6) bumperHeights, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_bumper_heights == 1){
							printf("partII[%d].spve->trailers.units[%d].bumper_heights.front = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].bumper_heights.front);
							printf("partII[%d].spve->trailers.units[%d].bumper_heights.rear = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].bumper_heights.rear);
						}
						/*(3-3-7) centerOfGravity, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_center_of_gravity == 1){
							printf("partII[%d].spve->trailers.units[%d].center_of_gravity = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].center_of_gravity);
						}
						/*(3-3-8) frontPivot */
						printf("partII[%d].spve->trailers.units[%d].front_pivot.pivot_offset = %hi\n", i, j, decoded_item->partII[i].spve->trailers.units[j].front_pivot.pivot_offset);
						printf("partII[%d].spve->trailers.units[%d].front_pivot.pivot_angle = %hu\n", i, j, decoded_item->partII[i].spve->trailers.units[j].front_pivot.pivot_angle);
						printf("partII[%d].spve->trailers.units[%d].front_pivot.pivots = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].front_pivot.pivots);
						/*(3-3-9) rearPivot, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_rear_pivot == 1){
							printf("partII[%d].spve->trailers.units[%d].rear_pivot.pivot_offset = %hi\n", i, j, decoded_item->partII[i].spve->trailers.units[j].rear_pivot.pivot_offset);
							printf("partII[%d].spve->trailers.units[%d].rear_pivot.pivot_angle = %hu\n", i, j, decoded_item->partII[i].spve->trailers.units[j].rear_pivot.pivot_angle);
							printf("partII[%d].spve->trailers.units[%d].rear_pivot.pivots = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].rear_pivot.pivots);
						}
						/*(3-3-10) rearWheelOffset, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_rear_wheel_offset == 1){
							printf("partII[%d].spve->trailers.units[%d].rear_wheel_offset = %hi\n", i, j, decoded_item->partII[i].spve->trailers.units[j].rear_wheel_offset);
						}
						/*(3-3-11) positionOffset */
						printf("partII[%d].spve->trailers.units[%d].position_offset.x = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].position_offset.x);
						printf("partII[%d].spve->trailers.units[%d].position_offset.y = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].position_offset.y);
						/*(3-3-12) elevationOffset, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_elevation_offset == 1){
							printf("partII[%d].spve->trailers.units[%d].elevation_offset = %d\n", i, j, decoded_item->partII[i].spve->trailers.units[j].elevation_offset);
						}
						/*(3-3-13) crumbData, OPTIONAL */
						if(decoded_item->partII[i].spve->trailers.units[j].mask_crumb_data == 1){							
							for(k = 0; k < decoded_item->partII[i].spve->trailers.units[j].crumb_data_num; k++)
							{
								/*(3-3-13-1) pivotAngle */
								printf("partII[%d].spve->trailers.units[%d].crumb_data[%d].pivot_angle = %hu\n", i, j, k, decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].pivot_angle);
								/*(3-3-13-2) timeOffset */
								printf("partII[%d].spve->trailers.units[%d].crumb_data[%d].time_offset = %hu\n", i, j, k, decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].time_offset);
								/*(3-3-13-3) positionOffset */
								printf("partII[%d].spve->trailers.units[%d].crumb_data[%d].position_offset.x = %d\n", i, j, k, decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].position_offset.x);
								printf("partII[%d].spve->trailers.units[%d].crumb_data[%d].position_offset.y = %d\n", i, j, k, decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].position_offset.y);
								/*(3-3-13-4) elevationOffset, OPTIONAL */
								if(decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].mask_elevation_offset == 1){
									printf("partII[%d].spve->trailers.units[%d].crumb_data[%d].elevation_offset = %d\n", i, j, k, decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].elevation_offset);
								}
								/*(3-3-13-5) heading, OPTIONAL */
								if(decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].mask_heading == 1){
									printf("partII[%d].spve->trailers.units[%d].crumb_data[%d].heading = %d\n", i, j, k, decoded_item->partII[i].spve->trailers.units[j].crumb_data[k].heading);
								}
							}		
						}
					}					
				}
			}else if(decoded_item->partII[i].partII_id == SUPPLEMENTAL_VEHICLE_EXT){
				/*(1) suve->classification, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_classification == 1){
					printf("partII[%d].suve->classification = %d\n", i, decoded_item->partII[i].suve->classification);
				}				
				/*(2) suve->classDetails, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_class_details == 1){
					/*(2-1) suve->classDetails->keyType, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_key_type == 1){
						printf("partII[%d].suve->class_details.key_type = %d\n", i, decoded_item->partII[i].suve->class_details.key_type);
					}					
					/*(2-2) suve->classDetails->role, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_role == 1){
						printf("partII[%d].suve->class_details.role = %d\n", i, decoded_item->partII[i].suve->class_details.role);
					}					
					/*(2-3) suve->classDetails->iso3883, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_iso3883 == 1){
						printf("partII[%d].suve->class_details.iso3883 = %d\n", i, decoded_item->partII[i].suve->class_details.iso3883);
					}					
					/*(2-4) suve->classDetails->hpmsType, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_hpms_type == 1){
						printf("partII[%d].suve->class_details.hpms_type = %d\n", i, decoded_item->partII[i].suve->class_details.hpms_type);
					}					
					/*(2-5) suve->classDetails->vehicleType, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_vehicle_type == 1){
						printf("partII[%d].suve->class_details.vehicle_type = %hu\n", i, decoded_item->partII[i].suve->class_details.vehicle_type);
					}					
					/*(2-6) suve->classDetails->responseEquip, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_response_equip == 1){
						printf("partII[%d].suve->class_details.response_equip = %hu\n", i, decoded_item->partII[i].suve->class_details.response_equip);
					}					
					/*(2-7) suve->classDetails->responderType, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_responder_type == 1){
						printf("partII[%d].suve->class_details.responder_type = %hu\n", i, decoded_item->partII[i].suve->class_details.responder_type);
					}					
					/*(2-8) suve->classDetails->fuelType, OPTIONAL*/
					if(decoded_item->partII[i].suve->class_details.mask_fuel_type == 1){
						printf("partII[%d].suve->class_details.fuel_type = %d\n", i, decoded_item->partII[i].suve->class_details.fuel_type);
					}					
				}
				/*(3) suve->vehicleData, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_vehicle_data == 1){
					/*(3-1) suve->vehicleData->height, OPTIONAL*/
					if(decoded_item->partII[i].suve->vehicle_data.mask_height == 1){
						printf("partII[%d].suve->vehicle_data.height = %d\n", i, decoded_item->partII[i].suve->vehicle_data.height);
					}					
					/*(3-2) suve->vehicleData->bumpers, OPTIONAL*/
					if(decoded_item->partII[i].suve->vehicle_data.mask_bumpers == 1){
						printf("partII[%d].suve->vehicle_data.bumpers.front = %d\n", i, decoded_item->partII[i].suve->vehicle_data.bumpers.front);
						printf("partII[%d].suve->vehicle_data.bumpers.rear = %d\n", i, decoded_item->partII[i].suve->vehicle_data.bumpers.rear);
					}					
					/*(3-3) suve->vehicleData->mass, OPTIONAL*/
					if(decoded_item->partII[i].suve->vehicle_data.mask_mass == 1){
						printf("partII[%d].suve->vehicle_data.mass = %d\n", i, decoded_item->partII[i].suve->vehicle_data.mass);
					}					
					/*(3-4) suve->vehicleData->trailerWeight, OPTIONAL*/
					if(decoded_item->partII[i].suve->vehicle_data.mask_trailer_weight == 1){
						printf("partII[%d].suve->vehicle_data.trailer_weight = %hu\n", i, decoded_item->partII[i].suve->vehicle_data.trailer_weight);
					}
				}					
				/*(4) suve->weatherReport, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_weather_report == 1){
					/*(4-1) suve->weatherReport->isRaining*/
					printf("partII[%d].suve->weather_report.is_raining = %d\n", i, decoded_item->partII[i].suve->weather_report.is_raining);
					/*(4-2) suve->weatherReport->rainRate, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_report.mask_rain_rate == 1){
						printf("partII[%d].suve->weather_report.rain_rate = %hu\n", i, decoded_item->partII[i].suve->weather_report.rain_rate);
					}					
					/*(4-3) suve->weatherReport->precipSituation, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_report.mask_precip_situation == 1){
						printf("partII[%d].suve->weather_report.precip_situation = %d\n", i, decoded_item->partII[i].suve->weather_report.precip_situation);
					}					
					/*(4-4) suve->weatherReport->solarRadiation, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_report.mask_solar_radiation == 1){
						printf("partII[%d].suve->weather_report.solar_radiation = %hu\n", i, decoded_item->partII[i].suve->weather_report.solar_radiation);
					}					
					/*(4-5) suve->weatherReport->friction, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_report.mask_friction == 1){
						printf("partII[%d].suve->weather_report.friction = %d\n", i, decoded_item->partII[i].suve->weather_report.friction);
					}					
					/*(4-6) suve->weatherReport->roadFriction, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_report.mask_road_friction == 1){
						printf("partII[%d].suve->weather_report.road_friction = %d\n", i, decoded_item->partII[i].suve->weather_report.road_friction);
					}
				}					
				/*(5) suve->weatherProbe, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_weather_probe == 1){
					/*(5-1) suve->weatherProbe->airTemp, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_probe.mask_air_temp == 1){
						printf("partII[%d].suve->weather_probe.air_temp = %d\n", i, decoded_item->partII[i].suve->weather_probe.air_temp);
					}					
					/*(5-2) suve->weatherProbe->airPressure, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_probe.mask_air_pressure == 1){
						printf("partII[%d].suve->weather_probe.air_pressure = %d\n", i, decoded_item->partII[i].suve->weather_probe.air_pressure);
					}					
					/*(5-3) suve->weatherProbe->rainRates, OPTIONAL*/
					if(decoded_item->partII[i].suve->weather_probe.mask_rain_rates == 1){
						/*(5-3-1) suve->weatherProbe->rainRates->statusFront*/
						printf("partII[%d].suve->weather_probe.rain_rates.status_front = %d\n", i, decoded_item->partII[i].suve->weather_probe.rain_rates.status_front);
						/*(5-3-2) suve->weatherProbe->rainRates->rateFront*/
						printf("partII[%d].suve->weather_probe.rain_rates.rate_front = %d\n", i, decoded_item->partII[i].suve->weather_probe.rain_rates.rate_front);
						/*(5-3-3) suve->weatherProbe->rainRates->statusRear, OPTIONAL*/
						if(decoded_item->partII[i].suve->weather_probe.rain_rates.mask_status_rear == 1){
							printf("partII[%d].suve->weather_probe.rain_rates.status_rear = %d\n", i, decoded_item->partII[i].suve->weather_probe.rain_rates.status_rear);
						}						
						/*(5-3-4) suve->weatherProbe->rainRates->rateRear, OPTIONAL*/
						if(decoded_item->partII[i].suve->weather_probe.rain_rates.mask_rate_rear == 1){
							printf("partII[%d].suve->weather_probe.rain_rates.rate_rear = %d\n", i, decoded_item->partII[i].suve->weather_probe.rain_rates.rate_rear);
						}
					}
				}
					
				/*(6) suve->obstacle, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_obstacle == 1){
					/*(6-1) suve->obstacle->obDist*/
					printf("partII[%d].suve->obstacle.ob_dist = %hu\n", i, decoded_item->partII[i].suve->obstacle.ob_dist);
					/*(6-2) suve->obstacle->obDirect*/
					printf("partII[%d].suve->obstacle.ob_direct = %hu\n", i, decoded_item->partII[i].suve->obstacle.ob_direct);
					/*(6-3) suve->obstacle->description, OPTIONAL*/
					if(decoded_item->partII[i].suve->obstacle.mask_description == 1){
						printf("partII[%d].suve->obstacle.description = %hu\n", i, decoded_item->partII[i].suve->obstacle.description);
					}					
					/*(6-4) suve->obstacle->locationDetails, OPTIONAL*/
					if(decoded_item->partII[i].suve->obstacle.mask_location_details == 1){
						printf("partII[%d].suve->obstacle.location_details = %hu\n", i, decoded_item->partII[i].suve->obstacle.location_details);
					}					
					/*(6-5) suve->obstacle->dateTime*/
						/*(6-5-1) year, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_year == 1){
							printf("partII[%d].suve->obstacle.date_time.year = %hu\n", i, decoded_item->partII[i].suve->obstacle.date_time.year);
						}						
						/*(6-5-2) month, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_month == 1){
							printf("partII[%d].suve->obstacle.date_time.month = %d\n", i, decoded_item->partII[i].suve->obstacle.date_time.month);
						}						
						/*(6-5-3) day, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_day == 1){
							printf("partII[%d].suve->obstacle.date_time.day = %d\n", i, decoded_item->partII[i].suve->obstacle.date_time.day);
						}						
						/*(6-5-4) hour, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_hour == 1){
							printf("partII[%d].suve->obstacle.date_time.hour = %d\n", i, decoded_item->partII[i].suve->obstacle.date_time.hour);
						}						
						/*(6-5-5) minute, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_minute == 1){
							printf("partII[%d].suve->obstacle.date_time.minute = %d\n", i, decoded_item->partII[i].suve->obstacle.date_time.minute);
						}						
						/*(6-5-6) second, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_second == 1){
							printf("partII[%d].suve->obstacle.date_time.second = %hu\n", i, decoded_item->partII[i].suve->obstacle.date_time.second);
						}						
						/*(6-5-7) offset, OPTIONAL*/
						if(decoded_item->partII[i].suve->obstacle.date_time.mask_offset == 1){
							printf("partII[%d].suve->obstacle.date_time.offset = %hi\n", i, decoded_item->partII[i].suve->obstacle.date_time.offset);
						}
					/*(6-6) suve->obstacle->vertEvent, OPTIONAL*/
					if(decoded_item->partII[i].suve->obstacle.mask_vert_event == 1){
						printf("partII[%d].suve->obstacle.vert_event = %d\n", i, decoded_item->partII[i].suve->obstacle.vert_event);
					}
				}					
				/*(7) suve->status, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_status == 1){
					/*(7-1) suve->status->statusDetails*/
					printf("partII[%d].suve->status.status_details = %hu\n", i, decoded_item->partII[i].suve->status.status_details);
					/*(7-2) suve->status->locationDetails, OPTIONAL*/
					if(decoded_item->partII[i].suve->status.mask_location_details == 1){
						printf("partII[%d].suve->status.location_details = %hu\n", i, decoded_item->partII[i].suve->status.location_details);
					}
				}					
				/*(8) suve->speedProfile, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_speed_profile == 1){
					/*(8-1) suve->speedProfile->speedReports*/
					for(j = 0; j < decoded_item->partII[i].suve->speed_profile_num; j++)
					{
						printf("partII[%d].suve->speed_profile[%d] = %d\n", i, j, decoded_item->partII[i].suve->speed_profile[j]);
					}
				}					
				/*(9) suve->theRTCM, OPTIONAL*/
				if(decoded_item->partII[i].suve->mask_the_rtcm == 1){
					/*(9-1) suve->theRTCM->rtcmHeader, OPTIONAL*/
					if(decoded_item->partII[i].suve->the_rtcm.mask_rtcm_header == 1){						
						/*(9-1-1) suve->theRTCM->rtcmHeader->status*/
						printf("partII[%d].suve->the_rtcm.rtcm_header.status = %d\n", i, decoded_item->partII[i].suve->the_rtcm.rtcm_header.status);
						/*(9-1-2) suve->theRTCM->rtcmHeader->offsetSet*/
						printf("partII[%d].suve->the_rtcm.rtcm_header.offset_set.ant_offset_x = %hi\n", i, decoded_item->partII[i].suve->the_rtcm.rtcm_header.offset_set.ant_offset_x);
						printf("partII[%d].suve->the_rtcm.rtcm_header.offset_set.ant_offset_y = %hi\n", i, decoded_item->partII[i].suve->the_rtcm.rtcm_header.offset_set.ant_offset_y);
						printf("partII[%d].suve->the_rtcm.rtcm_header.offset_set.ant_offset_z = %hi\n", i, decoded_item->partII[i].suve->the_rtcm.rtcm_header.offset_set.ant_offset_z);
					}						
					/*(9-2) suve->theRTCM->msgs*/
					for(j = 0; j < decoded_item->partII[i].suve->the_rtcm.msgs_num; j++)
					{
						printf("partII[%d].suve->the_rtcm.msgs[j].rtcm_message = %s\n", i, decoded_item->partII[i].suve->the_rtcm.msgs[j].rtcm_message);
					}
				}					
			}
		}
	}

	printf("===============================\n");
}

int cmd_rxBsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	bsm_standard_item *decoded_item = NULL;
	int err_bsm;

	int err, ret;
	TX_TIMER stop_bsm_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxBsm <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxBsm 123\n");
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
	stop_bsm = 0;
	tx_timer_create(&stop_bsm_timer, NULL, stop_bsm_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_bsm == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (bsm_standard_item *)j2735_bsm_calloc(1, sizeof(bsm_standard_item));
		err_bsm = j2735r63_bsm_decode(decoded_item, in_wsm.data, MAX_BSM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_bsm < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_bsm_standard_item(decoded_item);}
    
		j2735r63_bsm_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_bsm_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_bsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	bsm_standard_item *encode_item = NULL;
	bsm_standard_item *decoded_item = NULL;
	int err_bsm, i, j, k, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 BSM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_bsm_calloc(1, MAX_BSM_BUFF_SIZE);
		encode_item = (bsm_standard_item *)j2735_bsm_calloc(1, sizeof(bsm_standard_item));
		/*(1) core_data */
		encode_item->core_data.msg_cnt = 127;
		memset(encode_item->core_data.id, 0xFF, 4);
		encode_item->core_data.sec_mark = 65535;
		encode_item->core_data.lat = 900000001;
		encode_item->core_data.longitude = 1800000001;
		encode_item->core_data.elev = 61439;
		encode_item->core_data.accuracy.semi_major = 255;
		encode_item->core_data.accuracy.semi_minor = 254;
		encode_item->core_data.accuracy.orientation = 65535;
		encode_item->core_data.transmission_state = 7;
		encode_item->core_data.speed = 8191;
		encode_item->core_data.heading = 28800;
		encode_item->core_data.angle = 127;
		encode_item->core_data.accel_set.longitude = 2001;
		encode_item->core_data.accel_set.lat = 2001;
		encode_item->core_data.accel_set.vert = 127;
		encode_item->core_data.accel_set.yaw = 32767;
		encode_item->core_data.brakes.wheel_brakes = 2;
		encode_item->core_data.brakes.traction = 3;
		encode_item->core_data.brakes.abs = 3;
		encode_item->core_data.brakes.scs = 2;
		encode_item->core_data.brakes.brake_boost = 2;
		encode_item->core_data.brakes.aux_brakes = 3;
		encode_item->core_data.size.width = 1023;
		encode_item->core_data.size.length = 4095;
		
#if 0
		/*(2) PartII, SEQUENCE (SIZE(1..8))*/
		encode_item->mask_partII = 1;
		encode_item->partII_num = 1;
		encode_item->partII = (partII_item *)j2735_bsm_calloc(encode_item->partII_num, sizeof(partII_item));
		if(!(encode_item->partII))
		{
			printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		
		for(i = 0; i < encode_item->partII_num; i++)
		{
			encode_item->partII[i].partII_id = VEHICLE_SAFETY_EXT;
			encode_item->partII[i].vse = (vehicle_safety_extensions_item *)j2735_bsm_calloc(1, sizeof(vehicle_safety_extensions_item));
			if(!(encode_item->partII[i].vse))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}

			/*(2-1) PartII.vse->events, op*/
			encode_item->partII[i].vse->mask_events = 1;
			encode_item->partII[i].vse->events = 1024;

			/*(2-2) PartII.vse->path_history, op*/
			encode_item->partII[i].vse->mask_path_history = 1;
			encode_item->partII[i].vse->path_history = (path_history_item *)j2735_bsm_calloc(1, sizeof(path_history_item));
			if(!(encode_item->partII[i].vse->path_history))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				/*(2-2-1) PartII.vse->path_history->initial_position, op*/
				encode_item->partII[i].vse->path_history->mask_initial_position = 1;
				encode_item->partII[i].vse->path_history->initial_position = (full_position_vector_item *)j2735_bsm_calloc(1, sizeof(full_position_vector_item));
				if(!(encode_item->partII[i].vse->path_history->initial_position))
				{
					printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}else{
					/*(2-2-1-1) PartII.vse->path_history->initial_position->utc_time, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_utc_time = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time = (ddate_time_item *)j2735_bsm_calloc(1, sizeof(ddate_time_item));
					if(!(encode_item->partII[i].vse->path_history->initial_position->utc_time))
					{
						printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
						return -MEMORY_ALLOCATE_FAIL;		 
					}else{
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_year = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->year = 2016;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_month = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->month = 12;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_day = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->day = 31;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_hour = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->hour = 23;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_minute = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->minute = 59;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_second = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->second = 59;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_offset = 1;
						encode_item->partII[i].vse->path_history->initial_position->utc_time->offset = 840;
					}					

					/*(2-2-1-2) PartII.vse->path_history->initial_position->longitude*/
					encode_item->partII[i].vse->path_history->initial_position->longitude = 1800000001;

					/*(2-2-1-3) PartII.vse->path_history->initial_position->lat*/
					encode_item->partII[i].vse->path_history->initial_position->lat = 900000001;

					/*(2-2-1-4) PartII.vse->path_history->initial_position->elevation*/
					encode_item->partII[i].vse->path_history->initial_position->mask_elevation = 1;
					encode_item->partII[i].vse->path_history->initial_position->elevation = -4095;

					/*(2-2-1-5) PartII.vse->path_history->initial_position->heading, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_heading = 1;
					encode_item->partII[i].vse->path_history->initial_position->heading = 27800;

					/*(2-2-1-6) PartII.vse->path_history->initial_position->speed, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_speed = 1;
					encode_item->partII[i].vse->path_history->initial_position->speed.speed = 8191;
					encode_item->partII[i].vse->path_history->initial_position->speed.transmission_state = 7;

					/*(2-2-1-7) PartII.vse->path_history->initial_position->pos_accuracy, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_pos_accuracy = 1;
					encode_item->partII[i].vse->path_history->initial_position->pos_accuracy.semi_major = 253;
					encode_item->partII[i].vse->path_history->initial_position->pos_accuracy.semi_minor = 252;
					encode_item->partII[i].vse->path_history->initial_position->pos_accuracy.orientation = 65534;

					/*(2-2-1-8) PartII.vse->path_history->initial_position->time_confidence, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_time_confidence = 1;
					encode_item->partII[i].vse->path_history->initial_position->time_confidence = 39;

					/*(2-2-1-9) PartII.vse->path_history->initial_position->pos_confidence, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_pos_confidence = 1;
					encode_item->partII[i].vse->path_history->initial_position->pos_confidence.pos = 15;
					encode_item->partII[i].vse->path_history->initial_position->pos_confidence.elevation = 15;

					/*(2-2-1-10) PartII.vse->path_history->initial_position->speed_confidence, op*/
					encode_item->partII[i].vse->path_history->initial_position->mask_speed_confidence = 1;
					encode_item->partII[i].vse->path_history->initial_position->speed_confidence.heading = 7;
					encode_item->partII[i].vse->path_history->initial_position->speed_confidence.speed = 7;
					encode_item->partII[i].vse->path_history->initial_position->speed_confidence.throttle = 3;
				}

				/*(2-2-2) PartII.vse->path_history->curr_gps_status, op*/
				encode_item->partII[i].vse->path_history->mask_curr_gps_status = 0;
				encode_item->partII[i].vse->path_history->curr_gps_status = 64;

				/*(2-2-3) PartII.vse->path_history->crumb_data*/
				encode_item->partII[i].vse->path_history->crumb_data_num = 1;
				encode_item->partII[i].vse->path_history->crumb_data = (path_history_point_item *)j2735_bsm_calloc(encode_item->partII[i].vse->path_history->crumb_data_num, sizeof(path_history_point_item));
				if(!(encode_item->partII[i].vse->path_history->crumb_data))
				{
					printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}
				
				for(j = 0; j < encode_item->partII[i].vse->path_history->crumb_data_num; j++)
				{
					/*(2-2-3-1) PartII.vse->path_history->crumb_data.lat_offset*/
					encode_item->partII[i].vse->path_history->crumb_data[j].lat_offset = 131071;

					/*(2-2-3-2) PartII.vse->path_history->crumb_data.lon_offset*/
					encode_item->partII[i].vse->path_history->crumb_data[j].lon_offset = 131071;

					/*(2-2-3-3) PartII.vse->path_history->crumb_data.elevation_offset*/
					encode_item->partII[i].vse->path_history->crumb_data[j].elevation_offset = 2047;

					/*(2-2-3-4) PartII.vse->path_history->crumb_data.time_offset*/
					encode_item->partII[i].vse->path_history->crumb_data[j].time_offset = 65535;

					/*(2-2-3-5) PartII.vse->path_history->crumb_data.speed, op*/
					encode_item->partII[i].vse->path_history->crumb_data[j].mask_speed = 1;
					encode_item->partII[i].vse->path_history->crumb_data[j].speed = 8191;

					/*(2-2-3-6) PartII.vse->path_history->crumb_data.pos_accuracy, op*/
					encode_item->partII[i].vse->path_history->crumb_data[j].mask_pos_accuracy = 1;
					encode_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.semi_major = 251;
					encode_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.semi_minor = 250;
					encode_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.orientation = 65533;

					/*(2-2-3-7) PartII.vse->path_history->crumb_data.heading, op*/
					encode_item->partII[i].vse->path_history->crumb_data[j].mask_heading = 1;
					encode_item->partII[i].vse->path_history->crumb_data[j].heading = 240;
				}
			}
			
			/*(2-3) PartII.vse->path_prediction, op*/
			encode_item->partII[i].vse->mask_path_prediction = 1;
			encode_item->partII[i].vse->path_prediction.radius_of_curve = 32767;
			encode_item->partII[i].vse->path_prediction.confidence = 200;

			/*(2-4) PartII.vse->lights, op*/
			encode_item->partII[i].vse->mask_lights = 1;
			encode_item->partII[i].vse->lights = 256;			
		}	
#endif

#if 1
		/*(2) PartII, SEQUENCE (SIZE(1..8))*/
		encode_item->mask_partII = 1;
		encode_item->partII_num = 1;
		encode_item->partII = (partII_item *)j2735_bsm_calloc(encode_item->partII_num, sizeof(partII_item));
		if(!(encode_item->partII))
		{
			printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		
		for(i = 0; i < encode_item->partII_num; i++)
		{
			encode_item->partII[i].partII_id = SPECIAL_VEHICLE_EXT;
			encode_item->partII[i].spve = (special_vehicle_extensions_item *)j2735_bsm_calloc(1, sizeof(special_vehicle_extensions_item));
			if(!(encode_item->partII[i].spve))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}

			/*(2-1) PartII.spve->vehicle_alerts, op*/
			encode_item->partII[i].spve->mask_vehicle_alerts = 1;
			encode_item->partII[i].spve->vehicle_alerts.ssp_rights = 31;
			encode_item->partII[i].spve->vehicle_alerts.siren_use = 3;
			encode_item->partII[i].spve->vehicle_alerts.lights_use = 7;
			encode_item->partII[i].spve->vehicle_alerts.multi = 3;
			encode_item->partII[i].spve->vehicle_alerts.mask_events = 1;
			encode_item->partII[i].spve->vehicle_alerts.events.ssp_rights = 31;
			encode_item->partII[i].spve->vehicle_alerts.events.event = 1024;
			encode_item->partII[i].spve->vehicle_alerts.mask_response_type = 1;
			encode_item->partII[i].spve->vehicle_alerts.response_type = 6;

			/*(2-2) PartII.spve->description, op*/
			encode_item->partII[i].spve->mask_description = 1;
			encode_item->partII[i].spve->description.type_event = 0;
			encode_item->partII[i].spve->description.mask_description = 1;
			encode_item->partII[i].spve->description.description_num = 1;
			encode_item->partII[i].spve->description.description[0] = 0;
			encode_item->partII[i].spve->description.mask_priority = 1;
			encode_item->partII[i].spve->description.priority = 97;
			encode_item->partII[i].spve->description.mask_heading = 1;
			encode_item->partII[i].spve->description.heading = 8;
			encode_item->partII[i].spve->description.mask_extent = 1;
			encode_item->partII[i].spve->description.extent = 15;
			
			/*(2-3) PartII.spve->trailers, op*/
			encode_item->partII[i].spve->mask_trailers = 1;
			encode_item->partII[i].spve->trailers.ssp_rights = 30;
			encode_item->partII[i].spve->trailers.connection.pivot_offset = -1024;
			encode_item->partII[i].spve->trailers.connection.pivot_angle = 28800;
			encode_item->partII[i].spve->trailers.connection.pivots = 1;
			encode_item->partII[i].spve->trailers.units_num = 1;
			encode_item->partII[i].spve->trailers.units = (trailer_unit_description_item *)j2735_bsm_calloc(encode_item->partII[i].spve->trailers.units_num, sizeof(trailer_unit_description_item));
			if(!(encode_item->partII[i].spve->trailers.units))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			
			for(j = 0; j < encode_item->partII[i].spve->trailers.units_num; j++)
			{
				encode_item->partII[i].spve->trailers.units[j].is_dolly = 1;
				encode_item->partII[i].spve->trailers.units[j].width = 1023;
				encode_item->partII[i].spve->trailers.units[j].length = 1024;
				encode_item->partII[i].spve->trailers.units[j].mask_height = 1;
				encode_item->partII[i].spve->trailers.units[j].height = 127;
				encode_item->partII[i].spve->trailers.units[j].mask_mass = 1;
				encode_item->partII[i].spve->trailers.units[j].mass = 255;
				encode_item->partII[i].spve->trailers.units[j].mask_bumper_heights = 1;
				encode_item->partII[i].spve->trailers.units[j].bumper_heights.front = 0;
				encode_item->partII[i].spve->trailers.units[j].bumper_heights.rear = 127;
				encode_item->partII[i].spve->trailers.units[j].mask_center_of_gravity = 1;
				encode_item->partII[i].spve->trailers.units[j].center_of_gravity = 127;
				encode_item->partII[i].spve->trailers.units[j].front_pivot.pivot_offset = -1024;
				encode_item->partII[i].spve->trailers.units[j].front_pivot.pivot_angle = 0;
				encode_item->partII[i].spve->trailers.units[j].front_pivot.pivots = 1;
				encode_item->partII[i].spve->trailers.units[j].mask_rear_pivot = 1;
				encode_item->partII[i].spve->trailers.units[j].rear_pivot.pivot_offset = 1023;
				encode_item->partII[i].spve->trailers.units[j].rear_pivot.pivot_angle = 28800;
				encode_item->partII[i].spve->trailers.units[j].rear_pivot.pivots = 1;
				encode_item->partII[i].spve->trailers.units[j].mask_rear_wheel_offset = 1;
				encode_item->partII[i].spve->trailers.units[j].rear_wheel_offset = -2048;
				encode_item->partII[i].spve->trailers.units[j].position_offset.x = -2048;
				encode_item->partII[i].spve->trailers.units[j].position_offset.y = 2047;
				encode_item->partII[i].spve->trailers.units[j].mask_elevation_offset = 1;
				encode_item->partII[i].spve->trailers.units[j].elevation_offset = 192;
				encode_item->partII[i].spve->trailers.units[j].mask_crumb_data = 1;
				encode_item->partII[i].spve->trailers.units[j].crumb_data_num = 1;
				encode_item->partII[i].spve->trailers.units[j].crumb_data = (trailer_history_point_item *)j2735_bsm_calloc(encode_item->partII[i].spve->trailers.units[j].crumb_data_num, sizeof(trailer_history_point_item));
				if(!(encode_item->partII[i].spve->trailers.units[j].crumb_data))
				{
					printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}
				
				for(k = 0; k < encode_item->partII[i].spve->trailers.units[j].crumb_data_num; k++)
				{
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].pivot_angle = 28800;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].time_offset = 65535;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].position_offset.x = 2047;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].position_offset.y = -2048;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].mask_elevation_offset = 1;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].elevation_offset = 192;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].mask_heading = 1;
					encode_item->partII[i].spve->trailers.units[j].crumb_data[k].heading = 240;
				}
			}
		}
#endif

#if 0
		/*(2) PartII, SEQUENCE (SIZE(1..8))*/
		encode_item->mask_partII = 1;
		encode_item->partII_num = 1;
		encode_item->partII = (partII_item *)j2735_bsm_calloc(encode_item->partII_num, sizeof(partII_item));
		if(!(encode_item->partII))
		{
			printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		
		for(i = 0; i < encode_item->partII_num; i++)
		{
			encode_item->partII[i].partII_id = SUPPLEMENTAL_VEHICLE_EXT;
			encode_item->partII[i].suve = (supplemental_vehicle_extensions_item *)j2735_bsm_calloc(1, sizeof(supplemental_vehicle_extensions_item));
			if(!(encode_item->partII[i].suve))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}

			/*(2-1) PartII.suve->classification, op*/
			encode_item->partII[i].suve->mask_classification = 1;
			encode_item->partII[i].suve->classification = 255;
			
			/*(2-2) PartII.suve->class_details, op*/
			encode_item->partII[i].suve->mask_class_details = 1;
			encode_item->partII[i].suve->class_details.mask_key_type = 1;			
			encode_item->partII[i].suve->class_details.key_type = 255;
			encode_item->partII[i].suve->class_details.mask_role = 1;
			encode_item->partII[i].suve->class_details.role = 22;
			encode_item->partII[i].suve->class_details.mask_iso3883 = 1;
			encode_item->partII[i].suve->class_details.iso3883 = 100;
			encode_item->partII[i].suve->class_details.mask_hpms_type = 1;
			encode_item->partII[i].suve->class_details.hpms_type = 15;
			encode_item->partII[i].suve->class_details.mask_vehicle_type = 1;
			encode_item->partII[i].suve->class_details.vehicle_type = 9217;
			encode_item->partII[i].suve->class_details.mask_response_equip = 1;
			encode_item->partII[i].suve->class_details.response_equip = 10113;
			encode_item->partII[i].suve->class_details.mask_responder_type = 1;
			encode_item->partII[i].suve->class_details.responder_type = 9742;
			encode_item->partII[i].suve->class_details.mask_fuel_type = 1;
			encode_item->partII[i].suve->class_details.fuel_type = 9;
			
			/*(2-3) PartII.suve->vehicle_data, op*/
			encode_item->partII[i].suve->mask_vehicle_data = 1;
			encode_item->partII[i].suve->vehicle_data.mask_height = 1;
			encode_item->partII[i].suve->vehicle_data.height = 1;
			encode_item->partII[i].suve->vehicle_data.mask_bumpers = 1;
			encode_item->partII[i].suve->vehicle_data.bumpers.front = 0;
			encode_item->partII[i].suve->vehicle_data.bumpers.rear = 127;
			encode_item->partII[i].suve->vehicle_data.mask_mass = 1;
			encode_item->partII[i].suve->vehicle_data.mass = 255;
			encode_item->partII[i].suve->vehicle_data.mask_trailer_weight = 1;
			encode_item->partII[i].suve->vehicle_data.trailer_weight = 64255;
			
			/*(2-4) PartII.suve->weather_report, op*/
			encode_item->partII[i].suve->mask_weather_report = 1;
			encode_item->partII[i].suve->weather_report.is_raining = 3;
			encode_item->partII[i].suve->weather_report.mask_rain_rate = 1;
			encode_item->partII[i].suve->weather_report.rain_rate = 65535;
			encode_item->partII[i].suve->weather_report.mask_precip_situation = 1;
			encode_item->partII[i].suve->weather_report.precip_situation = 15;
			encode_item->partII[i].suve->weather_report.mask_solar_radiation = 1;
			encode_item->partII[i].suve->weather_report.solar_radiation = 65535;
			encode_item->partII[i].suve->weather_report.mask_friction = 1;
			encode_item->partII[i].suve->weather_report.friction = 101;

			/*(2-5) PartII.suve->weather_probe, op*/
			encode_item->partII[i].suve->mask_weather_probe = 1;
			encode_item->partII[i].suve->weather_probe.mask_air_temp = 1;
			encode_item->partII[i].suve->weather_probe.air_temp = 191;
			encode_item->partII[i].suve->weather_probe.mask_air_pressure = 1;
			encode_item->partII[i].suve->weather_probe.air_pressure = 255;
			encode_item->partII[i].suve->weather_probe.mask_rain_rates = 1;
			encode_item->partII[i].suve->weather_probe.rain_rates.status_front = 6;
			encode_item->partII[i].suve->weather_probe.rain_rates.rate_front = 127;
			encode_item->partII[i].suve->weather_probe.rain_rates.mask_status_rear = 1;
			encode_item->partII[i].suve->weather_probe.rain_rates.status_rear = 0;
			encode_item->partII[i].suve->weather_probe.rain_rates.mask_rate_rear = 1;
			encode_item->partII[i].suve->weather_probe.rain_rates.rate_rear = 0;
			
			/*(2-6) PartII.suve->obstacle, op*/
			encode_item->partII[i].suve->mask_obstacle = 1;
			encode_item->partII[i].suve->obstacle.ob_dist = 32767;
			encode_item->partII[i].suve->obstacle.ob_direct = 28800;
			encode_item->partII[i].suve->obstacle.mask_description = 1;
			encode_item->partII[i].suve->obstacle.description = 541;
			encode_item->partII[i].suve->obstacle.mask_location_details = 1;
			encode_item->partII[i].suve->obstacle.location_details = 8005;
			encode_item->partII[i].suve->obstacle.date_time.mask_year = 1;
			encode_item->partII[i].suve->obstacle.date_time.year = 2017;
			encode_item->partII[i].suve->obstacle.date_time.mask_month = 1;
			encode_item->partII[i].suve->obstacle.date_time.month = 1;
			encode_item->partII[i].suve->obstacle.date_time.mask_day = 1;
			encode_item->partII[i].suve->obstacle.date_time.day = 10;
			encode_item->partII[i].suve->obstacle.date_time.mask_hour = 1;
			encode_item->partII[i].suve->obstacle.date_time.hour = 18;
			encode_item->partII[i].suve->obstacle.date_time.mask_minute = 1;
			encode_item->partII[i].suve->obstacle.date_time.minute = 23;
			encode_item->partII[i].suve->obstacle.date_time.mask_second = 1;
			encode_item->partII[i].suve->obstacle.date_time.second = 45;
			encode_item->partII[i].suve->obstacle.date_time.mask_offset = 1;
			encode_item->partII[i].suve->obstacle.date_time.offset = -540;
			encode_item->partII[i].suve->obstacle.mask_vert_event = 1;
			encode_item->partII[i].suve->obstacle.vert_event = 16;
			
			/*(2-7) PartII.suve->status, op*/
			encode_item->partII[i].suve->mask_status = 1;
			encode_item->partII[i].suve->status.status_details = 523;
			encode_item->partII[i].suve->status.mask_location_details = 1;
			encode_item->partII[i].suve->status.location_details = 7937;

			/*(2-8) PartII.suve->speed_profile, op*/
			encode_item->partII[i].suve->mask_speed_profile = 1;
			encode_item->partII[i].suve->speed_profile_num = 1;		
			for(j = 0; j < encode_item->partII[i].suve->speed_profile_num; j++)
			{
				encode_item->partII[i].suve->speed_profile[j] = 31;
			}			

			/*(2-9) PartII.suve->the_rtcm, op*/
			encode_item->partII[i].suve->mask_the_rtcm = 1;
			encode_item->partII[i].suve->the_rtcm.mask_rtcm_header = 1;
			encode_item->partII[i].suve->the_rtcm.rtcm_header.status = 8;
			encode_item->partII[i].suve->the_rtcm.rtcm_header.offset_set.ant_offset_x = -2048;
			encode_item->partII[i].suve->the_rtcm.rtcm_header.offset_set.ant_offset_y = -256;
			encode_item->partII[i].suve->the_rtcm.rtcm_header.offset_set.ant_offset_z = -512;
			encode_item->partII[i].suve->the_rtcm.msgs_num = 1;
			encode_item->partII[i].suve->the_rtcm.msgs = (rtcm_message_item *)j2735_bsm_calloc(encode_item->partII[i].suve->the_rtcm.msgs_num, sizeof(rtcm_message_item));
			if(!(encode_item->partII[i].suve->the_rtcm.msgs))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			
			for(j = 0; j < encode_item->partII[i].suve->the_rtcm.msgs_num; j++)
			{
				memset(encode_item->partII[i].suve->the_rtcm.msgs[j].rtcm_message, 0, 1023);
				memcpy(encode_item->partII[i].suve->the_rtcm.msgs[j].rtcm_message, "rtcm_message_1", strlen("rtcm_message_1"));
				encode_item->partII[i].suve->the_rtcm.msgs[j].rtcm_message[strlen("rtcm_message_1")+1] = '\0';
			}
		}
#endif

		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("BSM loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_bsm_standard_item(encode_item);}
		
		err_bsm = j2735r63_bsm_encode(encode_item, encodeBuf, MAX_BSM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_bsm < 0)
		{
			printf("bsm encode fail...\n");		
			return -1;	
		}
		_cmd_bsm_free_struct(encode_item);

		decoded_item = (bsm_standard_item *)j2735_bsm_calloc(1, sizeof(bsm_standard_item));
		err_bsm = j2735r63_bsm_decode(decoded_item, encodeBuf, MAX_BSM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_bsm < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_bsm_standard_item(decoded_item);}
		
		j2735r63_bsm_free_struct(decoded_item);
		j2735_bsm_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}

	printf("%s", "J2735R63 BSM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txBsm(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, j,k, err_bsm;
	bsm_standard_item *encode_item = NULL;

	encode_item = (bsm_standard_item *)j2735_bsm_calloc(1, sizeof(bsm_standard_item));
	/*(1) core_data */
	encode_item->core_data.msg_cnt = 127;
	memset(encode_item->core_data.id, 0xFF, 4);
	encode_item->core_data.sec_mark = 65535;
	encode_item->core_data.lat = 900000001;
	encode_item->core_data.longitude = 1800000001;
	encode_item->core_data.elev = 61439;
	encode_item->core_data.accuracy.semi_major = 255;
	encode_item->core_data.accuracy.semi_minor = 254;
	encode_item->core_data.accuracy.orientation = 65535;
	encode_item->core_data.transmission_state = 7;
	encode_item->core_data.speed = 8191;
	encode_item->core_data.heading = 28800;
	encode_item->core_data.angle = 127;
	encode_item->core_data.accel_set.longitude = 2001;
	encode_item->core_data.accel_set.lat = 2001;
	encode_item->core_data.accel_set.vert = 127;
	encode_item->core_data.accel_set.yaw = 32767;
	encode_item->core_data.brakes.wheel_brakes = 2;
	encode_item->core_data.brakes.traction = 3;
	encode_item->core_data.brakes.abs = 3;
	encode_item->core_data.brakes.scs = 2;
	encode_item->core_data.brakes.brake_boost = 2;
	encode_item->core_data.brakes.aux_brakes = 3;
	encode_item->core_data.size.width = 1023;
	encode_item->core_data.size.length = 4095;


	/*(2) PartII, SEQUENCE (SIZE(1..8))*/
	encode_item->mask_partII = 1;
	encode_item->partII_num = 1;
	encode_item->partII = (partII_item *)j2735_bsm_calloc(encode_item->partII_num, sizeof(partII_item));
	if(!(encode_item->partII))
	{
		printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}
	
	for(i = 0; i < encode_item->partII_num; i++)
	{
		encode_item->partII[i].partII_id = VEHICLE_SAFETY_EXT;
		encode_item->partII[i].vse = (vehicle_safety_extensions_item *)j2735_bsm_calloc(1, sizeof(vehicle_safety_extensions_item));
		if(!(encode_item->partII[i].vse))
		{
			printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}

		/*(2-1) PartII.vse->events, op*/
		encode_item->partII[i].vse->mask_events = 1;
		encode_item->partII[i].vse->events = 1024;

		/*(2-2) PartII.vse->path_history, op*/
		encode_item->partII[i].vse->mask_path_history = 1;
		encode_item->partII[i].vse->path_history = (path_history_item *)j2735_bsm_calloc(1, sizeof(path_history_item));
		if(!(encode_item->partII[i].vse->path_history))
		{
			printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}else{
			/*(2-2-1) PartII.vse->path_history->initial_position, op*/
			encode_item->partII[i].vse->path_history->mask_initial_position = 1;
			encode_item->partII[i].vse->path_history->initial_position = (full_position_vector_item *)j2735_bsm_calloc(1, sizeof(full_position_vector_item));
			if(!(encode_item->partII[i].vse->path_history->initial_position))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}else{
				/*(2-2-1-1) PartII.vse->path_history->initial_position->utc_time, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_utc_time = 1;
				encode_item->partII[i].vse->path_history->initial_position->utc_time = (ddate_time_item *)j2735_bsm_calloc(1, sizeof(ddate_time_item));
				if(!(encode_item->partII[i].vse->path_history->initial_position->utc_time))
				{
					printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}else{
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_year = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->year = 2016;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_month = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->month = 12;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_day = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->day = 31;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_hour = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->hour = 23;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_minute = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->minute = 59;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_second = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->second = 59;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->mask_offset = 1;
					encode_item->partII[i].vse->path_history->initial_position->utc_time->offset = 840;
				}					

				/*(2-2-1-2) PartII.vse->path_history->initial_position->longitude*/
				encode_item->partII[i].vse->path_history->initial_position->longitude = 1800000001;

				/*(2-2-1-3) PartII.vse->path_history->initial_position->lat*/
				encode_item->partII[i].vse->path_history->initial_position->lat = 900000001;

				/*(2-2-1-4) PartII.vse->path_history->initial_position->elevation*/
				encode_item->partII[i].vse->path_history->initial_position->mask_elevation = 1;
				encode_item->partII[i].vse->path_history->initial_position->elevation = -4095;

				/*(2-2-1-5) PartII.vse->path_history->initial_position->heading, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_heading = 1;
				encode_item->partII[i].vse->path_history->initial_position->heading = 27800;

				/*(2-2-1-6) PartII.vse->path_history->initial_position->speed, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_speed = 1;
				encode_item->partII[i].vse->path_history->initial_position->speed.speed = 8191;
				encode_item->partII[i].vse->path_history->initial_position->speed.transmission_state = 7;

				/*(2-2-1-7) PartII.vse->path_history->initial_position->pos_accuracy, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_pos_accuracy = 1;
				encode_item->partII[i].vse->path_history->initial_position->pos_accuracy.semi_major = 253;
				encode_item->partII[i].vse->path_history->initial_position->pos_accuracy.semi_minor = 252;
				encode_item->partII[i].vse->path_history->initial_position->pos_accuracy.orientation = 65534;

				/*(2-2-1-8) PartII.vse->path_history->initial_position->time_confidence, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_time_confidence = 1;
				encode_item->partII[i].vse->path_history->initial_position->time_confidence = 39;

				/*(2-2-1-9) PartII.vse->path_history->initial_position->pos_confidence, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_pos_confidence = 1;
				encode_item->partII[i].vse->path_history->initial_position->pos_confidence.pos = 15;
				encode_item->partII[i].vse->path_history->initial_position->pos_confidence.elevation = 15;

				/*(2-2-1-10) PartII.vse->path_history->initial_position->speed_confidence, op*/
				encode_item->partII[i].vse->path_history->initial_position->mask_speed_confidence = 1;
				encode_item->partII[i].vse->path_history->initial_position->speed_confidence.heading = 7;
				encode_item->partII[i].vse->path_history->initial_position->speed_confidence.speed = 7;
				encode_item->partII[i].vse->path_history->initial_position->speed_confidence.throttle = 3;
			}

			/*(2-2-2) PartII.vse->path_history->curr_gps_status, op*/
			encode_item->partII[i].vse->path_history->mask_curr_gps_status = 0;
			encode_item->partII[i].vse->path_history->curr_gps_status = 64;

			/*(2-2-3) PartII.vse->path_history->crumb_data*/
			encode_item->partII[i].vse->path_history->crumb_data_num = 1;
			encode_item->partII[i].vse->path_history->crumb_data = (path_history_point_item *)j2735_bsm_calloc(encode_item->partII[i].vse->path_history->crumb_data_num, sizeof(path_history_point_item));
			if(!(encode_item->partII[i].vse->path_history->crumb_data))
			{
				printf("[BSM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			
			for(j = 0; j < encode_item->partII[i].vse->path_history->crumb_data_num; j++)
			{
				/*(2-2-3-1) PartII.vse->path_history->crumb_data.lat_offset*/
				encode_item->partII[i].vse->path_history->crumb_data[j].lat_offset = 131071;

				/*(2-2-3-2) PartII.vse->path_history->crumb_data.lon_offset*/
				encode_item->partII[i].vse->path_history->crumb_data[j].lon_offset = 131071;

				/*(2-2-3-3) PartII.vse->path_history->crumb_data.elevation_offset*/
				encode_item->partII[i].vse->path_history->crumb_data[j].elevation_offset = 2047;

				/*(2-2-3-4) PartII.vse->path_history->crumb_data.time_offset*/
				encode_item->partII[i].vse->path_history->crumb_data[j].time_offset = 65535;

				/*(2-2-3-5) PartII.vse->path_history->crumb_data.speed, op*/
				encode_item->partII[i].vse->path_history->crumb_data[j].mask_speed = 1;
				encode_item->partII[i].vse->path_history->crumb_data[j].speed = 8191;

				/*(2-2-3-6) PartII.vse->path_history->crumb_data.pos_accuracy, op*/
				encode_item->partII[i].vse->path_history->crumb_data[j].mask_pos_accuracy = 1;
				encode_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.semi_major = 251;
				encode_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.semi_minor = 250;
				encode_item->partII[i].vse->path_history->crumb_data[j].pos_accuracy.orientation = 65533;

				/*(2-2-3-7) PartII.vse->path_history->crumb_data.heading, op*/
				encode_item->partII[i].vse->path_history->crumb_data[j].mask_heading = 1;
				encode_item->partII[i].vse->path_history->crumb_data[j].heading = 240;
			}
		}
		
		/*(2-3) PartII.vse->path_prediction, op*/
		encode_item->partII[i].vse->mask_path_prediction = 1;
		encode_item->partII[i].vse->path_prediction.radius_of_curve = 32767;
		encode_item->partII[i].vse->path_prediction.confidence = 200;

		/*(2-4) PartII.vse->lights, op*/
		encode_item->partII[i].vse->mask_lights = 1;
		encode_item->partII[i].vse->lights = 256;			
	}

	
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nBSM Encode Data:\n");
		_show_bsm_standard_item(encode_item);
	}

	err_bsm = j2735r63_bsm_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_bsm < 0)
	{		
		printf("bsm encode fail...\n");		
		return -1;	
	}		
	_cmd_bsm_free_struct(encode_item);
	
	return 0;
}

int cmd_txBsm(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int err, i,cnt=0;
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
		printf("txBsm <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txBsm 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_bsm_calloc(1, MAX_BSM_BUFF_SIZE);
	_encode_txBsm(encodeBuf, MAX_BSM_BUFF_SIZE, &len);

	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_bsm_free(encodeBuf);

	while(num != 0){
		err = wme_wsm_send(&handle, &out_wsm);
		cnt = cnt + 1;
		if(err != 0) {
			printf("wsm_send = %d\n", err);
			return 0;
		}
		printf("wsm_send success count: %d\n",cnt);
		num--;
		(void) tx_thread_sleep(interval);
	}
	
	wme_exit(&handle);

	printf("%s", "cmd_txBsmPartIIStatus command executed successfully.\n");
	return CLI_OK;
}

