#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_pvd_api.h"
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

static int stop_pvd = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_pvd_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_pvd_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
/***************************************************************/

void stop_pvd_func(ULONG input)
{
	(void)input;
	stop_pvd = 1;
	return;
}

void _cmd_pvd_free_struct(pvd_standard_item *free_pvd_item)
{
	int i;

	if(free_pvd_item == NULL){
		printf("[PVD] Error: free_pvd_struct() input is NULL\n");
	}
	else
	{
		if(free_pvd_item->start_vector.utc_time != NULL){
			j2735_pvd_free(free_pvd_item->start_vector.utc_time);
		}

		if(free_pvd_item->snapshots != NULL)
		{
			for(i = 0; i < free_pvd_item->snapshots_num; i++)
			{
				if(free_pvd_item->snapshots[i].the_position.utc_time != NULL)
				{
					j2735_pvd_free(free_pvd_item->snapshots[i].the_position.utc_time);
				}
				if(free_pvd_item->snapshots[i].safety_ext.path_history != NULL){
					if(free_pvd_item->snapshots[i].safety_ext.path_history->initial_position != NULL){
						if(free_pvd_item->snapshots[i].safety_ext.path_history->initial_position->utc_time != NULL){
							j2735_pvd_free(free_pvd_item->snapshots[i].safety_ext.path_history->initial_position->utc_time);
						}
						j2735_pvd_free(free_pvd_item->snapshots[i].safety_ext.path_history->initial_position);
					}
					if(free_pvd_item->snapshots[i].safety_ext.path_history->crumb_data != NULL){
						j2735_pvd_free(free_pvd_item->snapshots[i].safety_ext.path_history->crumb_data);
					}
					j2735_pvd_free(free_pvd_item->snapshots[i].safety_ext.path_history);
				}				
				if(free_pvd_item->snapshots[i].data_set.accel_sets != NULL){
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.accel_sets);
				}
				if(free_pvd_item->snapshots[i].data_set.object != NULL){
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.object);
				}
				if(free_pvd_item->snapshots[i].data_set.full_pos != NULL){
					if(free_pvd_item->snapshots[i].data_set.full_pos->utc_time != NULL){
						j2735_pvd_free(free_pvd_item->snapshots[i].data_set.full_pos->utc_time);
					}
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.full_pos);
				}
				if(free_pvd_item->snapshots[i].data_set.vehicle_data != NULL){
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.vehicle_data);
				}

				if(free_pvd_item->snapshots[i].data_set.vehicle_ident != NULL){
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.vehicle_ident);
				}

				if(free_pvd_item->snapshots[i].data_set.j1939_data != NULL)
				{
					if(free_pvd_item->snapshots[i].data_set.j1939_data->tires != NULL)
					{
						j2735_pvd_free(free_pvd_item->snapshots[i].data_set.j1939_data->tires);
					}
					if(free_pvd_item->snapshots[i].data_set.j1939_data->axle != NULL)
					{
						j2735_pvd_free(free_pvd_item->snapshots[i].data_set.j1939_data->axle);
					}
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.j1939_data);
				}

				if(free_pvd_item->snapshots[i].data_set.weather_report != NULL){
					j2735_pvd_free(free_pvd_item->snapshots[i].data_set.weather_report);
				}
			}			
			j2735_pvd_free(free_pvd_item->snapshots);
		}
	}
}


void _fill_pvd_standard_item(pvd_standard_item *encode_item)
{
	int i, j;
	/*(1-1)***segNum****/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;
	/*(1-2)***segNum****/
	encode_item->mask_seg_num = 1;
	encode_item->seg_num = 32767;
	/*(1-3)***probe_ID****/
	encode_item->mask_probe_ID = 1;
	/*(1-3-1)***name****/
	encode_item->probe_ID.mask_name = 1;
	memcpy(encode_item->probe_ID.name, "ProbeVehicleData.probe_ID.name", strlen("ProbeVehicleData.probe_ID.name"));
	/*(1-3-2)***vin****/
	encode_item->probe_ID.mask_vin = 1;
	memset(encode_item->probe_ID.vin, 0x61, 17);
	encode_item->probe_ID.vin[17] = '\0';
	/*(1-3-3)***ownerCode****/
	encode_item->probe_ID.mask_owner_code = 1;
	memcpy(encode_item->probe_ID.owner_code, "kyho123123", strlen("kyho123123"));
	/*(1-3-4)***id****/
	encode_item->probe_ID.mask_id = 1;
	encode_item->probe_ID.id.vehicle_id_choice  = VEHICLE_ID_ENTITY_ID;
	memset(encode_item->probe_ID.id.entity_id, 0x62, 4);
	/*(1-3-5)***vehicleType****/
	encode_item->probe_ID.mask_vehicle_type = 1;
	encode_item->probe_ID.vehicle_type = 15;
	/*(1-3-6)***vehicleClass****/
	encode_item->probe_ID.mask_vehicle_class = 1;
	encode_item->probe_ID.vehicle_class_choice = VEHICLE_CLASS_VGROUP;
	encode_item->probe_ID.vehicle_class = 9250;
	/*(1-4)***start_vector****/
	/*(1-4-1)***utc_time****/
	encode_item->start_vector.mask_utc_time = 1;
	encode_item->start_vector.utc_time = (ddate_time_item *)j2735_pvd_calloc(1, sizeof(ddate_time_item));	
	/*(1-4-1-1)***year****/
	encode_item->start_vector.utc_time->mask_year = 1;
	encode_item->start_vector.utc_time->year = 2016; 
	/*(1-4-1-2)***month****/
	encode_item->start_vector.utc_time->mask_month = 1;
	encode_item->start_vector.utc_time->month = 11;	
	/*(1-4-1-3)***day****/
	encode_item->start_vector.utc_time->mask_day = 1;
	encode_item->start_vector.utc_time->day = 19;	
	/*(1-4-1-4)***hour****/
	encode_item->start_vector.utc_time->mask_hour = 1;
	encode_item->start_vector.utc_time->hour = 10;	
	/*(1-4-1-5)***minute****/
	encode_item->start_vector.utc_time->mask_minute = 1;
	encode_item->start_vector.utc_time->minute = 18;
	/*(1-4-1-6)***second****/
	encode_item->start_vector.utc_time->mask_second = 1;
	encode_item->start_vector.utc_time->second = 55; 
	/*(1-4-1-6)***offset****/
	encode_item->start_vector.utc_time->mask_offset = 1;
	encode_item->start_vector.utc_time->offset = 840; 
	/*(1-4-2)***longitude****/
	encode_item->start_vector.longitude = 1800000001; 
	/*(1-4-2)***lat****/
	encode_item->start_vector.lat = 900000001;	
	/*(1-4-4)***elevation****/
	encode_item->start_vector.mask_elevation = 1;
	encode_item->start_vector.elevation = 61439;
	/*(1-4-5)***heading****/
	encode_item->start_vector.mask_heading = 1;
	encode_item->start_vector.heading = 28800;
	/*(1-4-6)***speed****/
	encode_item->start_vector.mask_speed = 1;
	/*(1-4-6-1)***speed****/
	encode_item->start_vector.speed.speed = 8191;	
	/*(1-4-6)***transmission****/
	encode_item->start_vector.speed.transmission_state = 7; 
	/*(1-4-7)***pos_accuracy****/
	encode_item->start_vector.mask_pos_accuracy = 1;
	/*(1-4-7-1)***semi_major****/
	encode_item->start_vector.pos_accuracy.semi_major = 254;
	/*(1-4-7-2)***semi_minor****/
	encode_item->start_vector.pos_accuracy.semi_minor = 254;	
	/*(1-4-7-3)***semi_major_orient****/
	encode_item->start_vector.pos_accuracy.orientation = 65535; 
	/*(1-4-8)***time_confidence****/
	encode_item->start_vector.mask_time_confidence = 1;
	encode_item->start_vector.time_confidence = 39;	
	/*(1-4-9)***pos_confidence****/
	encode_item->start_vector.mask_pos_confidence = 1;
	/*(1-4-9-1)***pos****/
	encode_item->start_vector.pos_confidence.pos = 15;	
	/*(1-4-9-2)***elevation****/
	encode_item->start_vector.pos_confidence.elevation = 15;	
	/*(1-4-10)***speed_confidence****/
	encode_item->start_vector.mask_speed_confidence = 1;
	/*(1-4-10-1)***heading****/
	encode_item->start_vector.speed_confidence.heading = 7; 
	/*(1-4-10-2)***speed****/
	encode_item->start_vector.speed_confidence.speed = 7;	
	/*(1-4-10-3)***throttle****/
	encode_item->start_vector.speed_confidence.throttle = 3;
	/*(1-5)***vehicleType****/
	encode_item->vehicle_type.mask_key_type = 1;
	encode_item->vehicle_type.key_type = 255;
	encode_item->vehicle_type.mask_role = 1;
	encode_item->vehicle_type.role = 22;
	encode_item->vehicle_type.mask_iso3883 = 1;
	encode_item->vehicle_type.iso3883 = 100;
	encode_item->vehicle_type.mask_hpms_type = 1;
	encode_item->vehicle_type.hpms_type = 15;
	encode_item->vehicle_type.mask_vehicle_type = 1;
	encode_item->vehicle_type.vehicle_type = 9251;
	encode_item->vehicle_type.mask_response_equip = 1;
	encode_item->vehicle_type.response_equip = 10114;
	encode_item->vehicle_type.mask_responder_type = 1;
	encode_item->vehicle_type.responder_type = 9742;
	encode_item->vehicle_type.mask_fuel_type = 1;
	encode_item->vehicle_type.fuel_type = 9;
	encode_item->vehicle_type.mask_regional = 0;
	/*(1-6)***snapshots****/
	encode_item->snapshots_num = 1;
	encode_item->snapshots = (snapshots_item *)j2735_pvd_calloc(encode_item->snapshots_num, sizeof(snapshots_item));
	
	for(i = 0; i < encode_item->snapshots_num; i++)
	{
		/*(1-7-1)**the_position*****/
		/*(1-7-1-1)***utc_time****/
		encode_item->snapshots[i].the_position.mask_utc_time = 1;
		encode_item->snapshots[i].the_position.utc_time = (ddate_time_item *)j2735_pvd_calloc(1, sizeof(ddate_time_item));	
		/*(1-7-1-1-1)***year****/
		encode_item->snapshots[i].the_position.utc_time->mask_year = 1;
		encode_item->snapshots[i].the_position.utc_time->year = 2017;	
		/*(1-7-1-1-2)***month****/
		encode_item->snapshots[i].the_position.utc_time->mask_month = 1;
		encode_item->snapshots[i].the_position.utc_time->month = 12;	
		/*(1-7-1-1-3)***day****/
		encode_item->snapshots[i].the_position.utc_time->mask_day = 1;
		encode_item->snapshots[i].the_position.utc_time->day = 31;	
		/*(1-7-1-1-4)***hour****/
		encode_item->snapshots[i].the_position.utc_time->mask_hour = 1;
		encode_item->snapshots[i].the_position.utc_time->hour = 20; 
		/*(1-7-1-1-5)***minute****/
		encode_item->snapshots[i].the_position.utc_time->mask_minute = 1;
		encode_item->snapshots[i].the_position.utc_time->minute = 8;
		/*(1-7-1-1-6)***second****/
		encode_item->snapshots[i].the_position.utc_time->mask_second = 1;
		encode_item->snapshots[i].the_position.utc_time->second = 59;	
		/*(1-7-1-1-6)***offset****/
		encode_item->snapshots[i].the_position.utc_time->mask_offset = 1;
		encode_item->snapshots[i].the_position.utc_time->offset = 59;	
		/*(1-7-1-2)***longitude****/
		encode_item->snapshots[i].the_position.longitude = -1799999999;	
		/*(1-7-1-2)***lat****/
		encode_item->snapshots[i].the_position.lat = -900000000; 
		/*(1-7-1-4)***elevation****/
		encode_item->snapshots[i].the_position.mask_elevation = 1;
		encode_item->snapshots[i].the_position.elevation = -4095;
		/*(1-7-1-5)***heading****/
		encode_item->snapshots[i].the_position.mask_heading = 1;
		encode_item->snapshots[i].the_position.heading = 28800;
		/*(1-7-1-6)***speed****/
		encode_item->snapshots[i].the_position.mask_speed = 1;
		/*(1-7-1-6-1)***speed****/
		encode_item->snapshots[i].the_position.speed.speed = 8191;	
		/*(1-7-1-6)***transmission****/
		encode_item->snapshots[i].the_position.speed.transmission_state = 5;	
		/*(1-7-1-7)***pos_accuracy****/
		encode_item->snapshots[i].the_position.mask_pos_accuracy = 1;
		/*(1-7-1-7-1)***semi_major****/
		encode_item->snapshots[i].the_position.pos_accuracy.semi_major = 254;	
		/*(1-7-1-7-2)***semi_minor****/
		encode_item->snapshots[i].the_position.pos_accuracy.semi_minor = 254;	
		/*(1-7-1-7-3)***semi_major_orient****/
		encode_item->snapshots[i].the_position.pos_accuracy.orientation = 65535;
		/*(1-7-1-8)***time_confidence****/
		encode_item->snapshots[i].the_position.mask_time_confidence = 1;
		encode_item->snapshots[i].the_position.time_confidence = 20;	
		/*(1-7-1-9)***pos_confidence****/
		encode_item->snapshots[i].the_position.mask_pos_confidence = 1;
		/*(1-7-1-9-1)***pos****/
		encode_item->snapshots[i].the_position.pos_confidence.pos = 10;	
		/*(1-7-1-9-2)***elevation****/
		encode_item->snapshots[i].the_position.pos_confidence.elevation = 15;	
		/*(1-7-1-10)***speed_confidence****/
		encode_item->snapshots[i].the_position.mask_speed_confidence = 1;
		/*(1-7-1-10-1)***heading****/
		encode_item->snapshots[i].the_position.speed_confidence.heading = 5;	
		/*(1-7-1-10-2)***speed****/
		encode_item->snapshots[i].the_position.speed_confidence.speed = 4; 
		/*(1-7-1-10-3)***throttle****/
		encode_item->snapshots[i].the_position.speed_confidence.throttle = 2;		
		/*(1-7-2)***safety_ext****/
		encode_item->snapshots[i].mask_safety_ext = 1;
		encode_item->snapshots[i].safety_ext.mask_events = 1;
		encode_item->snapshots[i].safety_ext.events = 1024;
		encode_item->snapshots[i].safety_ext.mask_path_history = 1;
		encode_item->snapshots[i].safety_ext.path_history = (path_history_item *)j2735_pvd_calloc(1, sizeof(path_history_item));		
		/*(1-7-2-1)***initial_position****/
		encode_item->snapshots[i].safety_ext.path_history->mask_initial_position = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position = (full_position_vector_item *)j2735_pvd_calloc(1, sizeof(full_position_vector_item));
		
		/*(1-7-2-1-1)***initial_position****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_utc_time = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time = (ddate_time_item *)j2735_pvd_calloc(1, sizeof(ddate_time_item));
		
		/*(1-7-2-1-1-1)***year****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_year = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->year = 2010;
		
		/*(1-7-2-1-1-2)***month****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_month = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->month = 10;
		
		/*(1-7-2-1-1-3)***day****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_day = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->day = 19;
		
		/*(1-7-2-1-1-4)***hour****/

		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_hour = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->hour = 10;
		
		/*(1-7-2-1-1-5)***minute****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_minute = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->minute = 18;
		
		/*(1-7-2-1-1-6)***second****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_second = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->second = 55;
		
		/*(1-7-2-1-1-6)***offset****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_offset = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->offset = -55;

		/*(1-7-2-1-2)***longitude****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->longitude = 1800000001;
		
		/*(1-7-2-1-2)***lat****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->lat = 900000001;
		
		/*(1-7-2-1-4)***elevation****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_elevation = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->elevation = 61439;
		
		/*(1-7-2-1-5)***heading****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_heading = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->heading = 28800;
		
		/*(1-7-2-1-6)***speed****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_speed = 1;
		
		/*(1-7-2-1-6-1)***speed****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->speed.speed = 8191;
		
		/*(1-7-2-1-6)***transmission****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->speed.transmission_state = 3;
		
		/*(1-7-2-1-7)***pos_accuracy****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_pos_accuracy = 1;
		
		/*(1-7-2-1-7-1)***semi_major****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->pos_accuracy.semi_major = 254;
		
		/*(1-7-2-1-7-2)***semi_minor****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->pos_accuracy.semi_minor = 254;
		
		/*(1-7-2-1-7-3)***semi_major_orient****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->pos_accuracy.orientation = 65535;
	
		/*(1-7-2-1-8)***time_confidence****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_time_confidence = 1;
		encode_item->snapshots[i].safety_ext.path_history->initial_position->time_confidence = 30;
		
		/*(1-7-2-1-9)***pos_confidence****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_pos_confidence = 1;
		
		/*(1-7-2-1-9-1)***pos****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->pos_confidence.pos = 8;
		
		/*(1-7-2-1-9-2)***elevation****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->pos_confidence.elevation = 7;
		
		/*(1-7-2-1-10)***speed_confidence****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->mask_speed_confidence = 1;
		
		/*(1-7-2-1-10-1)***heading****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->speed_confidence.heading = 3;
		
		/*(1-7-2-1-10-2)***speed****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->speed_confidence.speed = 2;
		
		/*(1-7-2-1-10-3)***throttle****/
		encode_item->snapshots[i].safety_ext.path_history->initial_position->speed_confidence.throttle = 1;
		
		/*(1-7-2-2)***currGPSstatus****/
		encode_item->snapshots[i].safety_ext.path_history->mask_curr_gps_status = 1;
		encode_item->snapshots[i].safety_ext.path_history->curr_gps_status = 255;
		
		/*(1-7-2-4)***crumbData****/
		encode_item->snapshots[i].safety_ext.path_history->crumb_data_num = 1;
		encode_item->snapshots[i].safety_ext.path_history->crumb_data = (path_history_point_item *)j2735_pvd_calloc(encode_item->snapshots[i].safety_ext.path_history->crumb_data_num, sizeof(path_history_point_item));
		
		for(j = 0; j < encode_item->snapshots[i].safety_ext.path_history->crumb_data_num; j++)
		{
			/*(2-2-3-1) PartII.vse->path_history->crumb_data.lat_offset*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].lat_offset = 131071;

			/*(2-2-3-2) PartII.vse->path_history->crumb_data.lon_offset*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].lon_offset = 131071;

			/*(2-2-3-3) PartII.vse->path_history->crumb_data.elevation_offset*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].elevation_offset = 2047;

			/*(2-2-3-4) PartII.vse->path_history->crumb_data.time_offset*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].time_offset = 65535;

			/*(2-2-3-5) PartII.vse->path_history->crumb_data.speed, op*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].mask_speed = 1;
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].speed = 8191;

			/*(2-2-3-6) PartII.vse->path_history->crumb_data.pos_accuracy, op*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].mask_pos_accuracy = 1;
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].pos_accuracy.semi_major = 251;
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].pos_accuracy.semi_minor = 250;
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].pos_accuracy.orientation = 65533;

			/*(2-2-3-7) PartII.vse->path_history->crumb_data.heading, op*/
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].mask_heading = 1;
			encode_item->snapshots[i].safety_ext.path_history->crumb_data[j].heading = 240;
		}
		/*(1-7-2-3)***path_prediction****/
		encode_item->snapshots[i].safety_ext.mask_path_prediction = 1;
		encode_item->snapshots[i].safety_ext.path_prediction.radius_of_curve = 32767;
		encode_item->snapshots[i].safety_ext.path_prediction.confidence = 200;
		/*lights, op*/
		encode_item->snapshots[i].safety_ext.mask_lights = 1;
		encode_item->snapshots[i].safety_ext.lights = 256;			
		
		/*(1-7-3)***data_set****/
		encode_item->snapshots[i].mask_data_set = 1;
		encode_item->snapshots[i].data_set.mask_lights = 1;
		encode_item->snapshots[i].data_set.lights = 128;
		encode_item->snapshots[i].data_set.mask_light_bar = 1;
		encode_item->snapshots[i].data_set.light_bar = 7;
		encode_item->snapshots[i].data_set.mask_wipers = 1;
		encode_item->snapshots[i].data_set.wipers.status_front = 6;
		encode_item->snapshots[i].data_set.wipers.rate_front = 127;
		encode_item->snapshots[i].data_set.wipers.mask_status_rear = 1;
		encode_item->snapshots[i].data_set.wipers.status_rear = 0;
		encode_item->snapshots[i].data_set.wipers.mask_rate_rear = 1;
		encode_item->snapshots[i].data_set.wipers.rate_rear = 127;
		encode_item->snapshots[i].data_set.mask_brake_status = 1;
		encode_item->snapshots[i].data_set.brake_status.wheel_brakes = (char)8;
		encode_item->snapshots[i].data_set.brake_status.traction = 3;
		encode_item->snapshots[i].data_set.brake_status.abs = 3;
		encode_item->snapshots[i].data_set.brake_status.scs = 3;
		encode_item->snapshots[i].data_set.brake_status.brake_boost = 2;
		encode_item->snapshots[i].data_set.brake_status.aux_brakes = 3;
		encode_item->snapshots[i].data_set.mask_brake_pressure = 1;
		encode_item->snapshots[i].data_set.brake_pressure = 15;
		encode_item->snapshots[i].data_set.mask_road_friction = 1;
		encode_item->snapshots[i].data_set.road_friction = 50;
		encode_item->snapshots[i].data_set.mask_sun_data = 1;
		encode_item->snapshots[i].data_set.sun_data = 1000;
		encode_item->snapshots[i].data_set.mask_rain_data = 1;
		encode_item->snapshots[i].data_set.rain_data = 7;
		encode_item->snapshots[i].data_set.mask_air_temp = 1;
		encode_item->snapshots[i].data_set.air_temp = 191;
		encode_item->snapshots[i].data_set.mask_air_pres = 1;
		encode_item->snapshots[i].data_set.air_pres = 255;
		encode_item->snapshots[i].data_set.mask_steering = 1;
		encode_item->snapshots[i].data_set.steering.angle = 127;
		encode_item->snapshots[i].data_set.steering.mask_confidence =1;
		encode_item->snapshots[i].data_set.steering.confidence = 3;
		encode_item->snapshots[i].data_set.steering.mask_rate =1;
		encode_item->snapshots[i].data_set.steering.rate = 127;
		encode_item->snapshots[i].data_set.steering.mask_wheels = 1;
		encode_item->snapshots[i].data_set.steering.wheels = 127;
		encode_item->snapshots[i].data_set.mask_accel_sets = 1;
		encode_item->snapshots[i].data_set.accel_sets = (accel_sets_item *)j2735_pvd_calloc(1,sizeof(accel_sets_item));
		encode_item->snapshots[i].data_set.accel_sets->mask_accel_4way = 1;
		encode_item->snapshots[i].data_set.accel_sets->accel_4way.longitude = 2001;
		encode_item->snapshots[i].data_set.accel_sets->accel_4way.lat = 2001;
		encode_item->snapshots[i].data_set.accel_sets->accel_4way.vert = 127;
		encode_item->snapshots[i].data_set.accel_sets->accel_4way.yaw = 32767 ;
		encode_item->snapshots[i].data_set.accel_sets->mask_vert_accel_thres = 1;
		encode_item->snapshots[i].data_set.accel_sets->vert_accel_thres = (char)4;
		encode_item->snapshots[i].data_set.accel_sets->mask_yaw_rate_con = 1;
		encode_item->snapshots[i].data_set.accel_sets->yaw_rate_con = 7; 
		encode_item->snapshots[i].data_set.accel_sets->mask_hoz_accel_con = 1;
		encode_item->snapshots[i].data_set.accel_sets->hoz_accel_con = 7;
		encode_item->snapshots[i].data_set.accel_sets->mask_confidence_set = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_accel_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.accel_confidence.yaw_rate = 7;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.accel_confidence.acceleration = 7;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.accel_confidence.steering_wheel_angle = 3;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_speed_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.speed_confidence.heading = 7;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.speed_confidence.speed = 7;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.speed_confidence.throttle = 3;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_time_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.time_confidence = 39;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_pos_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.pos_confidence.pos = 15;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.pos_confidence.elevation = 15;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_steer_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.steer_confidence = 3;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_heading_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.heading_confidence = 7;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.mask_throttle_confidence = 1;
		encode_item->snapshots[i].data_set.accel_sets->confidence_set.throttle_confidence = 3;
		encode_item->snapshots[i].data_set.mask_object = 1;
		encode_item->snapshots[i].data_set.object = (object_item *)j2735_pvd_calloc(1,sizeof(object_item));
		encode_item->snapshots[i].data_set.object->ob_dist = 32767;
		encode_item->snapshots[i].data_set.object->ob_direct = 28800;
		encode_item->snapshots[i].data_set.object->date_time.mask_year = 1;
		encode_item->snapshots[i].data_set.object->date_time.year = 2010;	
		encode_item->snapshots[i].data_set.object->date_time.mask_month = 1;
		encode_item->snapshots[i].data_set.object->date_time.month = 10;	
		encode_item->snapshots[i].data_set.object->date_time.mask_day = 1;
		encode_item->snapshots[i].data_set.object->date_time.day = 19;	
		encode_item->snapshots[i].data_set.object->date_time.mask_hour = 1;
		encode_item->snapshots[i].data_set.object->date_time.hour = 10; 
		encode_item->snapshots[i].data_set.object->date_time.mask_minute = 1;
		encode_item->snapshots[i].data_set.object->date_time.minute = 18;
		encode_item->snapshots[i].data_set.object->date_time.mask_second = 1;
		encode_item->snapshots[i].data_set.object->date_time.second = 65535;	
		encode_item->snapshots[i].data_set.object->date_time.mask_offset = 1;
		encode_item->snapshots[i].data_set.object->date_time.offset = 840;	
		encode_item->snapshots[i].data_set.mask_full_pos = 1;
		encode_item->snapshots[i].data_set.full_pos = (full_position_vector_item *)j2735_pvd_calloc(1, sizeof(full_position_vector_item));
		encode_item->snapshots[i].data_set.full_pos->mask_utc_time = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time = (ddate_time_item *)j2735_pvd_calloc(1, sizeof(ddate_time_item));	
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_year = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->year = 2222;	
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_month = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->month = 10;	
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_day = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->day = 19; 
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_hour = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->hour = 31;	
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_minute = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->minute = 60;
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_second = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->second = 55;	
		encode_item->snapshots[i].data_set.full_pos->utc_time->mask_offset = 1;
		encode_item->snapshots[i].data_set.full_pos->utc_time->offset = 840;	
		encode_item->snapshots[i].data_set.full_pos->longitude = 1800000001;	
		encode_item->snapshots[i].data_set.full_pos->lat = 900000001;	
		encode_item->snapshots[i].data_set.full_pos->mask_elevation = 1;
		encode_item->snapshots[i].data_set.full_pos->elevation = 61439;
		encode_item->snapshots[i].data_set.full_pos->mask_heading = 1;
		encode_item->snapshots[i].data_set.full_pos->heading = 28800;
		encode_item->snapshots[i].data_set.full_pos->mask_speed = 1;
		encode_item->snapshots[i].data_set.full_pos->speed.speed = 8191; 
		encode_item->snapshots[i].data_set.full_pos->speed.transmission_state = 3;	
		encode_item->snapshots[i].data_set.full_pos->mask_pos_accuracy = 1;
		encode_item->snapshots[i].data_set.full_pos->pos_accuracy.semi_major = 254;	
		encode_item->snapshots[i].data_set.full_pos->pos_accuracy.semi_minor = 254;	
		encode_item->snapshots[i].data_set.full_pos->pos_accuracy.orientation = 65535;	
		encode_item->snapshots[i].data_set.full_pos->mask_time_confidence = 1;
		encode_item->snapshots[i].data_set.full_pos->time_confidence = 39;	
		encode_item->snapshots[i].data_set.full_pos->mask_pos_confidence = 1;
		encode_item->snapshots[i].data_set.full_pos->pos_confidence.pos = 15;	
		encode_item->snapshots[i].data_set.full_pos->pos_confidence.elevation = 15; 
		encode_item->snapshots[i].data_set.full_pos->mask_speed_confidence = 1;
		encode_item->snapshots[i].data_set.full_pos->speed_confidence.heading = 5;	
		encode_item->snapshots[i].data_set.full_pos->speed_confidence.speed = 4;	
		encode_item->snapshots[i].data_set.full_pos->speed_confidence.throttle = 3;

		encode_item->snapshots[i].data_set.mask_throttle_pos = 1;
		encode_item->snapshots[i].data_set.throttle_pos = 200;
		encode_item->snapshots[i].data_set.mask_speed_head_c = 1;
		encode_item->snapshots[i].data_set.speed_head_c.heading = 7;
		encode_item->snapshots[i].data_set.speed_head_c.speed = 5;
		encode_item->snapshots[i].data_set.speed_head_c.throttle = 3;
		encode_item->snapshots[i].data_set.mask_speed_c = 1;
		encode_item->snapshots[i].data_set.speed_c = 7;
		
		encode_item->snapshots[i].data_set.mask_vehicle_data = 1;
		encode_item->snapshots[i].data_set.vehicle_data = (pvd_vehicle_data_item *)j2735_pvd_calloc(1,sizeof(pvd_vehicle_data_item));
		encode_item->snapshots[i].data_set.vehicle_data->height = 127 ;
		encode_item->snapshots[i].data_set.vehicle_data->bumpers.front= 127;
		encode_item->snapshots[i].data_set.vehicle_data->bumpers.rear = 127;
		encode_item->snapshots[i].data_set.vehicle_data->mass =	255;
		encode_item->snapshots[i].data_set.vehicle_data->trailer_weight = 64255;
		encode_item->snapshots[i].data_set.vehicle_data->type = 15;
		
		encode_item->snapshots[i].data_set.mask_vehicle_ident = 1;
		encode_item->snapshots[i].data_set.vehicle_ident = (vehicle_ident_item *)j2735_pvd_calloc(1,sizeof(vehicle_ident_item));
		encode_item->snapshots[i].data_set.vehicle_ident->mask_name = 1;
		memcpy(encode_item->snapshots[i].data_set.vehicle_ident->name,"PVD_VehicleType_DescriptiveName",strlen("PVD_VehicleType_DescriptiveName"));
		encode_item->snapshots[i].data_set.vehicle_ident->mask_vin = 1;
		memset(encode_item->snapshots[i].data_set.vehicle_ident->vin, 0x31, 17);
		encode_item->snapshots[i].data_set.vehicle_ident->mask_owner_code = 1;
		memcpy(encode_item->snapshots[i].data_set.vehicle_ident->owner_code,"kyho123123",strlen("kyho123123"));
		encode_item->snapshots[i].data_set.vehicle_ident->owner_code[strlen("kyho123123")+1] = '\0';
		encode_item->snapshots[i].data_set.vehicle_ident->mask_id = 1;
		encode_item->snapshots[i].data_set.vehicle_ident->id.vehicle_id_choice = VEHICLE_ID_ENTITY_ID;
		memset(encode_item->snapshots[i].data_set.vehicle_ident->id.entity_id, 0x32, 4);
		encode_item->snapshots[i].data_set.vehicle_ident->mask_vehicle_type = 1;
		encode_item->snapshots[i].data_set.vehicle_ident->vehicle_type = 15;
		encode_item->snapshots[i].data_set.vehicle_ident->mask_vehicle_class = 1;
		encode_item->snapshots[i].data_set.vehicle_ident->vehicle_class_choice = VEHICLE_CLASS_RGROUP;
		encode_item->snapshots[i].data_set.vehicle_ident->vehicle_class = 9742;
		
		encode_item->snapshots[i].data_set.mask_j1939_data = 1;
		encode_item->snapshots[i].data_set.j1939_data = (j1939_data_item *)j2735_pvd_calloc(1,sizeof(j1939_data_item));
		encode_item->snapshots[i].data_set.j1939_data->mask_tires = 1;
		encode_item->snapshots[i].data_set.j1939_data->tires_num = 2;
		encode_item->snapshots[i].data_set.j1939_data->tires = (tires_item *)j2735_pvd_calloc(encode_item->snapshots[i].data_set.j1939_data->tires_num, sizeof(tires_item));
		for(j = 0; j < encode_item->snapshots[i].data_set.j1939_data->tires_num; j++)
		{
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_location = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].location = 255;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_pressure = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].pressure = 250;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_temp = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].temp = 55519;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_wheel_sensor_status = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].wheel_sensor_status = 3;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_wheel_end_elect_fault = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].wheel_end_elect_fault = 3;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_leakage_rate = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].leakage_rate = 64255;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].mask_detection = 1;
			encode_item->snapshots[i].data_set.j1939_data->tires[j].detection = 7;		
		}
		
		encode_item->snapshots[i].data_set.j1939_data->mask_axle = 1;
		encode_item->snapshots[i].data_set.j1939_data->axle_num = 2;
		encode_item->snapshots[i].data_set.j1939_data->axle = (axle_item *)j2735_pvd_calloc(encode_item->snapshots[i].data_set.j1939_data->axle_num, sizeof(axle_item));

		for(j = 0; j < encode_item->snapshots[i].data_set.j1939_data->axle_num; j++){
			encode_item->snapshots[i].data_set.j1939_data->axle[j].mask_location = 1;
			encode_item->snapshots[i].data_set.j1939_data->axle[j].location = 255;
			encode_item->snapshots[i].data_set.j1939_data->axle[j].mask_weight = 1;
			encode_item->snapshots[i].data_set.j1939_data->axle[j].weight = 64255;
		}
		
		encode_item->snapshots[i].data_set.j1939_data->mask_trailer_weight = 1;
		encode_item->snapshots[i].data_set.j1939_data->trailer_weight = 64255;
		encode_item->snapshots[i].data_set.j1939_data->mask_cargo_weight = 1;
		encode_item->snapshots[i].data_set.j1939_data->cargo_weight = 64255;
		encode_item->snapshots[i].data_set.j1939_data->mask_steering_axle_temperature = 1;
		encode_item->snapshots[i].data_set.j1939_data->steering_axle_temperature = 210;
		encode_item->snapshots[i].data_set.j1939_data->mask_drive_axle_location = 1;
		encode_item->snapshots[i].data_set.j1939_data->drive_axle_location = 255;
		encode_item->snapshots[i].data_set.j1939_data->mask_drive_axle_lift_air_pressure = 1;
		encode_item->snapshots[i].data_set.j1939_data->drive_axle_lift_air_pressure = 1000;
		encode_item->snapshots[i].data_set.j1939_data->mask_drive_axle_temperature = 1;
		encode_item->snapshots[i].data_set.j1939_data->drive_axle_temperature = 210;
		encode_item->snapshots[i].data_set.j1939_data->mask_drive_axle_lube_pressure = 1;
		encode_item->snapshots[i].data_set.j1939_data->drive_axle_lube_pressure = 250;
		encode_item->snapshots[i].data_set.j1939_data->mask_steering_axle_lube_pressure = 1;
		encode_item->snapshots[i].data_set.j1939_data->steering_axle_lube_pressure = 250;
		encode_item->snapshots[i].data_set.mask_weather_report = 1;
		encode_item->snapshots[i].data_set.weather_report = (pvd_weather_report_item *)j2735_pvd_calloc(1, sizeof(pvd_weather_report_item));
		encode_item->snapshots[i].data_set.weather_report->is_raining = 3; 
		encode_item->snapshots[i].data_set.weather_report->mask_rain_rate =1;
		encode_item->snapshots[i].data_set.weather_report->rain_rate = 65535; 
		encode_item->snapshots[i].data_set.weather_report->mask_precip_situation =1;
		encode_item->snapshots[i].data_set.weather_report->precip_situation = 15; 
		encode_item->snapshots[i].data_set.weather_report->mask_solar_radiation =1;
		encode_item->snapshots[i].data_set.weather_report->solar_radiation = 65535; 
		encode_item->snapshots[i].data_set.weather_report->mask_friction =1;
		encode_item->snapshots[i].data_set.weather_report->friction = 101; 
		encode_item->snapshots[i].data_set.mask_gps_status = 1;
		encode_item->snapshots[i].data_set.gps_status = (char)3;
		
	}
	
	encode_item->mask_regional = 0;

}
void _show_pvd_standard_item(pvd_standard_item *decoded_item)
{
	int i, j;

	printf("********PVD Message***********\n"); 	
	
	printf("message_id = %d\n", decoded_item->message_id);
	
	/*(1) time_stamp */
	if(decoded_item->mask_time_stamp == 1)
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	
	/*(2) segNum *OP**/
	if(decoded_item->mask_seg_num == 1)
	{
		printf("seg_num = %d\n", decoded_item->seg_num);
	}	
	/*(3) probeID *OP**/
	if(decoded_item->mask_probe_ID == 1)
	{
		/*(1-3-1)***name****/
		if(decoded_item->probe_ID.mask_name == 1)
		{
			printf("probe_ID->name = %s\n", decoded_item->probe_ID.name);
		}	
		/*(1-3-2)***vin****/
		if(decoded_item->probe_ID.mask_vin == 1)
		{
			printf("probe_ID->vin = %s\n", decoded_item->probe_ID.vin);
		}
		/*(1-3-3)***ownerCode****/
		if(decoded_item->probe_ID.mask_owner_code == 1)
		{
			printf("probe_ID->owner_code = %s\n", decoded_item->probe_ID.owner_code);
		}
		/*(1-3-4)***id****/
		if(decoded_item->probe_ID.mask_id == 1)
		{
			if(decoded_item->probe_ID.id.vehicle_id_choice == VEHICLE_ID_ENTITY_ID)
			{
				printf("probe_ID->entity_id = %x %x %x %x\n", decoded_item->probe_ID.id.entity_id[0], decoded_item->probe_ID.id.entity_id[1], decoded_item->probe_ID.id.entity_id[2], decoded_item->probe_ID.id.entity_id[3]);
			}
			else if(decoded_item->probe_ID.id.vehicle_id_choice == VEHICLE_ID_STATION_ID)
				printf("probe_ID->station_id = %ld\n", decoded_item->probe_ID.id.station_id);
		}
		/*(1-3-5)***vehicleType****/
		if(decoded_item->probe_ID.mask_vehicle_type == 1)
		{
			printf("probe_ID->vehicle_type = %d\n", decoded_item->probe_ID.vehicle_type);
		}
		/*(1-3-6)***vehicleClass****/
		if(decoded_item->probe_ID.mask_vehicle_class == 1)
		{				
			if(decoded_item->probe_ID.vehicle_class_choice == VEHICLE_CLASS_VGROUP)
			{
				printf("probe_ID->vehicle_class_choice = VEHICLE_CLASS_VGROUP\n");
				printf("probe_ID->vehicle_class = %d\n", decoded_item->probe_ID.vehicle_class);
			}else if(decoded_item->probe_ID.vehicle_class_choice == VEHICLE_CLASS_RGROUP)
			{
				printf("probe_ID->vehicle_class_choice = VEHICLE_CLASS_RGROUP\n");
				printf("probe_ID->vehicle_class = %d\n", decoded_item->probe_ID.vehicle_class);
			}else if(decoded_item->probe_ID.vehicle_class_choice == VEHICLE_CLASS_REQUIP)
			{
				printf("probe_ID->vehicle_class_choice = VEHICLE_CLASS_REQUIP\n");
				printf("probe_ID->vehicle_class = %d\n", decoded_item->probe_ID.vehicle_class);
			}else{
				printf("probe_ID->vehicle_class_choice = VEHICLE_CLASS_NOTHING\n");
			}
		}	
	}
	/*(4) startVector*/
	/*(4-1) utcTime *OP**/
	if(decoded_item->start_vector.mask_utc_time == 1)
	{
		/*(4-1-1) year *OP**/
		if(decoded_item->start_vector.utc_time->mask_year == 1)
		{
			printf("start_vector.utc_time->year = %d\n", decoded_item->start_vector.utc_time->year);
		}	
		/*(4-1-2) month *OP**/
		if(decoded_item->start_vector.utc_time->mask_month == 1)
		{
			printf("start_vector.utc_time->month = %d\n", decoded_item->start_vector.utc_time->month);
		}	
		/*(4-1-3) day *OP**/
		if(decoded_item->start_vector.utc_time->mask_day == 1)
		{
			printf("start_vector.utc_time->day = %d\n", decoded_item->start_vector.utc_time->day);
		}	
		/*(4-1-4) hour *OP**/
		if(decoded_item->start_vector.utc_time->mask_hour == 1)
		{
			printf("start_vector.utc_time->hour = %d\n", decoded_item->start_vector.utc_time->hour);
		}	
		/*(4-1-5) minute *OP**/
		if(decoded_item->start_vector.utc_time->mask_minute == 1)
		{
			printf("start_vector.utc_time->minute = %d\n", decoded_item->start_vector.utc_time->minute);
		}	
		/*(4-1-6) second *OP**/
		if(decoded_item->start_vector.utc_time->mask_second == 1)
		{
			printf("start_vector.utc_time->second = %d\n", decoded_item->start_vector.utc_time->second);
		}
		/*(4-1-7) offset *OP**/
		if(decoded_item->start_vector.utc_time->mask_offset == 1)
		{
			printf("start_vector.utc_time->offset = %d\n", decoded_item->start_vector.utc_time->offset);
		}
	}	
	/*(4-2) longitude */
	printf("start_vector.longitude = %ld\n", decoded_item->start_vector.longitude);
	/*(4-3) lat */
	printf("start_vector.lat = %ld\n", decoded_item->start_vector.lat);
	/*(4-4) elevation *OP**/
	if(decoded_item->start_vector.mask_elevation == 1)
	{
		printf("start_vector.elevation = %d\n", decoded_item->start_vector.elevation);
	}		
	/*(4-5) heading *OP**/
	if(decoded_item->start_vector.mask_heading == 1)
	{
		printf("start_vector.heading = %d\n", decoded_item->start_vector.heading);
	}		
	/*(4-6) speed *OP**/
	if(decoded_item->start_vector.mask_speed == 1)
	{
		printf("start_vector.speed->speed = %d\n", decoded_item->start_vector.speed.speed);
		printf("start_vector.speed->transmission_state = %d\n", decoded_item->start_vector.speed.transmission_state);
	}		
	/*(4-7) posAccuracy *OP**/
	if(decoded_item->start_vector.mask_pos_accuracy == 1)
	{
		printf("start_vector.pos_accuracy->semi_major = %d\n", decoded_item->start_vector.pos_accuracy.semi_major);
		printf("start_vector.pos_accuracy->semi_minor = %d\n", decoded_item->start_vector.pos_accuracy.semi_minor);
		printf("start_vector.pos_accuracy->semi_major_orient = %d\n", decoded_item->start_vector.pos_accuracy.orientation);
	}		
	/*(4-8) timeConfidence *OP**/
	if(decoded_item->start_vector.mask_time_confidence == 1)
	{
		printf("start_vector.time_confidence = %d\n", decoded_item->start_vector.time_confidence);
	}		
	/*(4-9) posConfidence *OP**/
	if(decoded_item->start_vector.mask_pos_confidence == 1)
	{
		printf("start_vector.pos_confidence->pos = %d\n", decoded_item->start_vector.pos_confidence.pos);
		printf("start_vector.pos_confidence->elevation = %d\n", decoded_item->start_vector.pos_confidence.elevation);
	}		
	/*(4-10) speedConfidence *OP**/
	if(decoded_item->start_vector.mask_speed_confidence == 1)
	{
		printf("start_vector.speed_confidence->heading = %d\n", decoded_item->start_vector.speed_confidence.heading);
		printf("start_vector.speed_confidence->speed = %d\n", decoded_item->start_vector.speed_confidence.speed);
		printf("start_vector.speed_confidence->throttle = %d\n", decoded_item->start_vector.speed_confidence.throttle);
	}		
	/*(5) vehicleType*/
	if(decoded_item->vehicle_type.mask_key_type == 1)
		printf("vehicle_type.key_type = %d\n", decoded_item->vehicle_type.key_type);
	if(decoded_item->vehicle_type.mask_role == 1)
		printf("vehicle_type.role = %d\n", decoded_item->vehicle_type.role);
	if(decoded_item->vehicle_type.mask_iso3883 == 1)
		printf("vehicle_type.iso3883 = %d\n", decoded_item->vehicle_type.iso3883);
	if(decoded_item->vehicle_type.mask_hpms_type == 1)
		printf("vehicle_type.hpms_type = %d\n", decoded_item->vehicle_type.hpms_type);
	if(decoded_item->vehicle_type.mask_vehicle_type == 1)
		printf("vehicle_type.vehicle_type = %d\n", decoded_item->vehicle_type.vehicle_type);
	if(decoded_item->vehicle_type.mask_response_equip == 1)
		printf("vehicle_type.response_equip = %d\n", decoded_item->vehicle_type.response_equip);
	if(decoded_item->vehicle_type.mask_responder_type == 1)
		printf("vehicle_type.responder_type = %d\n", decoded_item->vehicle_type.responder_type);
	if(decoded_item->vehicle_type.mask_fuel_type == 1)
		printf("vehicle_type.fuel_type = %d\n", decoded_item->vehicle_type.fuel_type);
	
	/*(6) snapshots*/
	printf("snapshots_num = %d\n", decoded_item->snapshots_num);
	for(i = 0; i < decoded_item->snapshots_num; i++)
	{
		/*(7-1)***thePosition****/
		/*(7-1-1)***utcTime****/
		if(decoded_item->snapshots[i].the_position.mask_utc_time == 1)
		{
			/*(7-1-1-1)***year****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_year == 1)
			{
				printf("snapshots[%d].the_position.utc_time->year = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->year);
			}
			/*(7-1-1-2)***month****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_month == 1)
			{
				printf("snapshots[%d].the_position.utc_time->month = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->month);
			}
			/*(7-1-1-3)***day****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_day == 1)
			{
				printf("snapshots[%d].the_position.utc_time->day = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->day);
			}
			/*(7-1-1-4)***hour****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_hour == 1)
			{
				printf("snapshots[%d].the_position.utc_time->hour = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->hour);
			}
			/*(7-1-1-5)***minute****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_minute == 1)
			{
				printf("snapshots[%d].the_position.utc_time->minute = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->minute);
			}
			/*(7-1-1-6)***second****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_second == 1)
			{
				printf("snapshots[%d].the_position.utc_time->second = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->second);
			}			
			/*(7-1-1-7)***offset****/
			if(decoded_item->snapshots[i].the_position.utc_time->mask_offset == 1)
			{
				printf("snapshots[%d].the_position.utc_time->offset = %d\n", i, decoded_item->snapshots[i].the_position.utc_time->offset);
			}			
		}
		/*(7-1-2)***longitude****/
		printf("snapshots[%d].the_position->longitude = %ld\n", i, decoded_item->snapshots[i].the_position.longitude);
		/*(7-1-3)***lat****/
		printf("snapshots[%d].the_position->lat = %ld\n", i, decoded_item->snapshots[i].the_position.lat);
		/*(7-1-4)***elevation****/
		if(decoded_item->snapshots[i].the_position.mask_elevation == 1)
		{
			printf("snapshots[%d].the_position->elevation = %d\n", i, decoded_item->snapshots[i].the_position.elevation);
		}
		/*(7-1-5)***heading****/
		if(decoded_item->snapshots[i].the_position.mask_heading == 1)
		{
			printf("snapshots[%d].the_position->heading = %d\n", i, decoded_item->snapshots[i].the_position.heading);
		}	
		/*(7-1-6)***speed****/
		if(decoded_item->snapshots[i].the_position.mask_speed == 1)
		{
			printf("snapshots[%d].the_position->speed->speed = %d\n", i, decoded_item->snapshots[i].the_position.speed.speed);
			printf("snapshots[%d].the_position->speed->transmission_state = %d\n", i, decoded_item->snapshots[i].the_position.speed.transmission_state);
		}
		/*(7-1-7)***posAccuracy****/
		if(decoded_item->snapshots[i].the_position.mask_pos_accuracy == 1)
		{
			printf("snapshots[%d].the_position->pos_accuracy->semi_major = %d\n", i, decoded_item->snapshots[i].the_position.pos_accuracy.semi_major);
			printf("snapshots[%d].the_position->pos_accuracy->semi_minor = %d\n", i, decoded_item->snapshots[i].the_position.pos_accuracy.semi_minor);
			printf("snapshots[%d].the_position->pos_accuracy->semi_major_orient = %d\n", i, decoded_item->snapshots[i].the_position.pos_accuracy.orientation);
		}
		/*(7-1-8)***timeConfidence****/
		if(decoded_item->snapshots[i].the_position.mask_time_confidence == 1)
		{
			printf("snapshots[%d].the_position->time_confidence = %d\n", i, decoded_item->snapshots[i].the_position.time_confidence);
		}			
		/*(7-1-9)***posConfidence****/
		if(decoded_item->snapshots[i].the_position.mask_pos_confidence == 1)
		{
			printf("snapshots[%d].the_position->pos_confidence->pos = %d\n", i, decoded_item->snapshots[i].the_position.pos_confidence.pos);
			printf("snapshots[%d].the_position->pos_confidence->elevation = %d\n", i, decoded_item->snapshots[i].the_position.pos_confidence.elevation);
		}
		/*(7-1-10)***speedConfidence****/
		if(decoded_item->snapshots[i].the_position.mask_speed_confidence == 1)
		{
			printf("snapshots[%d].the_position->speed_confidence->heading = %d\n", i, decoded_item->snapshots[i].the_position.speed_confidence.heading);
			printf("snapshots[%d].the_position->speed_confidence->speed = %d\n", i, decoded_item->snapshots[i].the_position.speed_confidence.speed);
			printf("snapshots[%d].the_position->speed_confidence->throttle = %d\n", i, decoded_item->snapshots[i].the_position.speed_confidence.throttle);
		}
		/*(7-2)***safetyExt****/
		if(decoded_item->snapshots[i].mask_safety_ext == 1)
		{
			/*(7-2-1)***events****/
			if(decoded_item->snapshots[i].safety_ext.mask_events == 1)
			{
				printf("snapshots[%d].safety_ext.events = %d\n", i, decoded_item->snapshots[i].safety_ext.events);
			}
			/*(7-2-2)***pathHistory****/
			if(decoded_item->snapshots[i].safety_ext.mask_path_history == 1)
			{
				/*(7-2-2-1)***initialPosition****/
				if(decoded_item->snapshots[i].safety_ext.path_history->mask_initial_position == 1)
				{						
					/*(7-2-2-1-1)***utcTime****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_utc_time == 1)
					{
						/*(7-2-2-1-1-1)***year****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_year == 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->year = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->year);
						}
						/*(7-2-2-1-1-2)***month****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_month == 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->month = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->month);
						}
						/*(7-2-2-1-1-3)***day****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_day == 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->day = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->day);
						}
						/*(7-2-2-1-1-4)***hour****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_hour == 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->hour = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->hour);
						}
						/*(7-2-2-1-1-5)***minute****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_minute == 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->minute = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->minute);
						}
						/*(7-2-2-1-1-6)***second****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_second == 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->second = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->second);
						}
						/*(7-2-2-1-1-7)***offset****/
						if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->mask_offset== 1)
						{
							printf("snapshots[%d].safety_ext.path_history->initial_position->utc_time->offset = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->utc_time->offset);
						}
					}
					/*(7-2-2-1-2)***longitude****/
					printf("snapshots[%d].safety_ext.path_history->initial_position->longitude = %ld\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->longitude);
					/*(7-2-2-1-3)***lat****/
					printf("snapshots[%d].safety_ext.path_history->initial_position->lat = %ld\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->lat);
					/*(7-2-2-1-4)***elevation****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_elevation == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->elevation = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->elevation);
					}				
					/*(7-2-2-1-5)***heading****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_heading == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->heading = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->heading);
					}					
					/*(7-2-2-1-6)***speed****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_speed == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->speed->speed = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->speed.speed);
						printf("snapshots[%d].safety_ext.path_history->initial_position->speed->transmission_state = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->speed.transmission_state);
					}				
					/*(7-2-2-1-7)***posAccuracy****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_pos_accuracy == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->pos_accuracy->semi_major = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->pos_accuracy.semi_major);
						printf("snapshots[%d].safety_ext.path_history->initial_position->pos_accuracy->semi_minor = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->pos_accuracy.semi_minor);
						printf("snapshots[%d].safety_ext.path_history->initial_position->pos_accuracy->semi_major_orient = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->pos_accuracy.orientation);
					}
					/*(7-2-2-1-8)***timeConfidence****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_time_confidence == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->time_confidence = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->time_confidence);
					}
					/*(7-2-2-1-9)***posConfidence****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_pos_confidence == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->pos_confidence->pos = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->pos_confidence.pos);
						printf("snapshots[%d].safety_ext.path_history->initial_position->pos_confidence->elevation = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->pos_confidence.elevation);
					}
					/*(7-2-2-1-10)***speedConfidence****/
					if(decoded_item->snapshots[i].safety_ext.path_history->initial_position->mask_speed_confidence == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->initial_position->speed_confidence->heading = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->speed_confidence.heading);
						printf("snapshots[%d].safety_ext.path_history->initial_position->speed_confidence->speed = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->speed_confidence.speed);
						printf("snapshots[%d].safety_ext.path_history->initial_position->speed_confidence->throttle = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->initial_position->speed_confidence.throttle);
					}
				}			
				/*(7-2-2-2)***currGPSstatus****/
				if(decoded_item->snapshots[i].safety_ext.path_history->mask_curr_gps_status == 1)
				{
					printf("snapshots[%d].safety_ext.path_history->curr_gps_status = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->curr_gps_status);
				}
				/*(7-2-2-3)***crumbData****/
				printf("snapshots[%d].safety_ext.path_history->crumb_data_num = %d\n", i, decoded_item->snapshots[i].safety_ext.path_history->crumb_data_num);
				for(j = 0; j < decoded_item->snapshots[i].safety_ext.path_history->crumb_data_num; j++)
				{
					printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].lat_offset = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].lat_offset);
					printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].lon_offset = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].lon_offset);
					printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].elevation_offset = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].elevation_offset);
					printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].time_offset = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].time_offset);

					if(decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].mask_speed == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].speed = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].speed);
					}

					if(decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].mask_pos_accuracy == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].pos_accuracy->semi_major = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].pos_accuracy.semi_major);
						printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].pos_accuracy->semi_minor = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].pos_accuracy.semi_minor);
						printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].pos_accuracy->semi_major_orient = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].pos_accuracy.orientation);
					}

					if(decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].mask_heading == 1)
					{
						printf("snapshots[%d].safety_ext.path_history->crumb_data[%d].heading = %d\n", i, j, decoded_item->snapshots[i].safety_ext.path_history->crumb_data[j].heading);
					}
				}
			}			
			/*(7-2-3)***pathPrediction****/
			if(decoded_item->snapshots[i].safety_ext.mask_path_prediction == 1)
			{
				printf("snapshots[%d].safety_ext.path_prediction->radius_of_curve = %d\n", i, decoded_item->snapshots[i].safety_ext.path_prediction.radius_of_curve);
				printf("snapshots[%d].safety_ext.path_prediction->confidence = %d\n", i, decoded_item->snapshots[i].safety_ext.path_prediction.confidence);
			}
			/*(7-2-4)***lights****/
			if(decoded_item->snapshots[i].safety_ext.mask_lights == 1)
			{
				printf("snapshots[%d].safety_ext.lights = %d\n", i, decoded_item->snapshots[i].safety_ext.lights);
			}
		}
		/*(7-3)***dataSet****/
		if(decoded_item->snapshots[i].mask_data_set == 1)
		{
			/*(7-3-1)***lights****/
			if(decoded_item->snapshots[i].data_set.mask_lights == 1)
			{
				printf("snapshots[%d].data_set.lights = %d\n", i, decoded_item->snapshots[i].data_set.lights);
			}
			/*(7-3-2)***lightBar****/
			if(decoded_item->snapshots[i].data_set.mask_light_bar == 1)
			{
				printf("snapshots[%d].data_set.light_bar = %d\n", i, decoded_item->snapshots[i].data_set.light_bar);
			}
			/*(7-3-3)***wipers****/
			if(decoded_item->snapshots[i].data_set.mask_wipers == 1)
			{
				printf("snapshots[%d].data_set.wipers->status_front = %d\n", i, decoded_item->snapshots[i].data_set.wipers.status_front);
				printf("snapshots[%d].data_set.wipers->rate_front = %d\n", i, decoded_item->snapshots[i].data_set.wipers.rate_front);
				if(decoded_item->snapshots[i].data_set.wipers.mask_status_rear == 1)
				{
					printf("snapshots[%d].data_set.wipers->status_rear = %d\n", i, decoded_item->snapshots[i].data_set.wipers.status_rear);
				}
				if(decoded_item->snapshots[i].data_set.wipers.mask_rate_rear == 1)
				{
					printf("snapshots[%d].data_set.wipers->rate_rear = %d\n", i, decoded_item->snapshots[i].data_set.wipers.rate_rear);
				}				
			}
			/*(7-3-4)***brakeStatus****/
			if(decoded_item->snapshots[i].data_set.mask_brake_status == 1)
			{
				printf("snapshots[%d].data_set.brake_status->wheel_brakes = %d\n", i, decoded_item->snapshots[i].data_set.brake_status.wheel_brakes);
				printf("snapshots[%d].data_set.brake_status->traction = %d\n", i, decoded_item->snapshots[i].data_set.brake_status.traction);
				printf("snapshots[%d].data_set.brake_status->abs = %d\n", i, decoded_item->snapshots[i].data_set.brake_status.abs);
				printf("snapshots[%d].data_set.brake_status->scs = %d\n", i, decoded_item->snapshots[i].data_set.brake_status.scs);
				printf("snapshots[%d].data_set.brake_status->brake_boost = %d\n", i, decoded_item->snapshots[i].data_set.brake_status.brake_boost);
				printf("snapshots[%d].data_set.brake_status->aux_brakes = %d\n", i, decoded_item->snapshots[i].data_set.brake_status.aux_brakes);
			}		
			/*(7-3-5)***brakePressure****/
			if(decoded_item->snapshots[i].data_set.mask_brake_pressure == 1)
			{
				printf("snapshots[%d].data_set.brake_pressure = %d\n", i, decoded_item->snapshots[i].data_set.brake_pressure);
			}
			/*(7-3-6)***roadFriction****/
			if(decoded_item->snapshots[i].data_set.mask_road_friction == 1)
			{
				printf("snapshots[%d].data_set.road_friction = %d\n", i, decoded_item->snapshots[i].data_set.road_friction);
			}
			/*(7-3-7)***sunData****/
			if(decoded_item->snapshots[i].data_set.mask_sun_data == 1)
			{
				printf("snapshots[%d].data_set.sun_data = %d\n", i, decoded_item->snapshots[i].data_set.sun_data);
			}
			/*(7-3-8)***rainData****/
			if(decoded_item->snapshots[i].data_set.mask_rain_data == 1)
			{
				printf("snapshots[%d].data_set.rain_data = %d\n", i, decoded_item->snapshots[i].data_set.rain_data);
			}
			/*(7-3-9)***airTemp****/
			if(decoded_item->snapshots[i].data_set.mask_air_temp == 1)
			{
				printf("snapshots[%d].data_set.air_temp = %d\n", i, decoded_item->snapshots[i].data_set.air_temp);
			}
			/*(7-3-10)***airPres****/
			if(decoded_item->snapshots[i].data_set.mask_air_pres == 1)
			{
				printf("snapshots[%d].data_set.air_pres = %d\n", i, decoded_item->snapshots[i].data_set.air_pres);
			}
			/*(7-3-11)***steering****/
			if(decoded_item->snapshots[i].data_set.mask_steering == 1)
			{
				printf("snapshots[%d].data_set.steering->angle = %d\n", i, decoded_item->snapshots[i].data_set.steering.angle);
				if(decoded_item->snapshots[i].data_set.steering.mask_confidence == 1)
				{	
					printf("snapshots[%d].data_set.steering->confidence = %d\n", i, decoded_item->snapshots[i].data_set.steering.confidence);
				}					
				if(decoded_item->snapshots[i].data_set.steering.mask_rate == 1)
				{
					printf("snapshots[%d].data_set.steering->rate = %d\n", i, decoded_item->snapshots[i].data_set.steering.rate);
				}					
				if(decoded_item->snapshots[i].data_set.steering.mask_wheels == 1)
				{
					printf("snapshots[%d].data_set.steering->wheels = %d\n", i, decoded_item->snapshots[i].data_set.steering.wheels);
				}				
			}			
			/*(7-3-12)***accelSets****/
			if(decoded_item->snapshots[i].data_set.mask_accel_sets == 1)
			{
				if(decoded_item->snapshots[i].data_set.accel_sets->mask_accel_4way == 1)
				{
					printf("snapshots[%d].data_set.accel_sets->accel_4way->longitude = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->accel_4way.longitude);
					printf("snapshots[%d].data_set.accel_sets->accel_4way->lat = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->accel_4way.lat);
					printf("snapshots[%d].data_set.accel_sets->accel_4way->vert = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->accel_4way.vert);
					printf("snapshots[%d].data_set.accel_sets->accel_4way->yaw = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->accel_4way.yaw);
				}
				if(decoded_item->snapshots[i].data_set.accel_sets->mask_vert_accel_thres == 1)
				{
					printf("snapshots[%d].data_set.accel_sets->vert_accel_thres = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->vert_accel_thres);
				}					
				if(decoded_item->snapshots[i].data_set.accel_sets->mask_yaw_rate_con == 1)
				{
					printf("snapshots[%d].data_set.accel_sets->yaw_rate_con = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->yaw_rate_con);
				}					
				if(decoded_item->snapshots[i].data_set.accel_sets->mask_hoz_accel_con == 1)
				{
					printf("snapshots[%d].data_set.accel_sets->hoz_accel_con = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->hoz_accel_con);
				}
				if(decoded_item->snapshots[i].data_set.accel_sets->mask_confidence_set == 1)
				{
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_accel_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.accel_confidence->yaw_rate = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.accel_confidence.yaw_rate);
						printf("snapshots[%d].data_set.accel_sets->confidence_set.accel_confidence->acceleration = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.accel_confidence.acceleration);
						printf("snapshots[%d].data_set.accel_sets->confidence_set.accel_confidence->steering_wheel_angle = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.accel_confidence.steering_wheel_angle);
					}
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_speed_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.speed_confidence->heading = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.speed_confidence.heading);
						printf("snapshots[%d].data_set.accel_sets->confidence_set.speed_confidence->speed = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.speed_confidence.speed);
						printf("snapshots[%d].data_set.accel_sets->confidence_set.speed_confidence->throttle = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.speed_confidence.throttle);
					}					
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_time_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.time_confidence = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.time_confidence);
					}
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_pos_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.pos_confidence->pos = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.pos_confidence.pos);
						printf("snapshots[%d].data_set.accel_sets->confidence_set.pos_confidence->elevation = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.pos_confidence.elevation);
					}
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_steer_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.steer_confidence = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.steer_confidence);
					}
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_heading_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.heading_confidence = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.heading_confidence);
					}
					if(decoded_item->snapshots[i].data_set.accel_sets->confidence_set.mask_throttle_confidence == 1)
					{
						printf("snapshots[%d].data_set.accel_sets->confidence_set.throttle_confidence = %d\n", i, decoded_item->snapshots[i].data_set.accel_sets->confidence_set.throttle_confidence);
					}					
				}				
			}
			/*(7-3-13)***object****/
			if(decoded_item->snapshots[i].data_set.mask_object == 1)
			{
				printf("snapshots[%d].data_set.object->ob_dist = %d\n", i, decoded_item->snapshots[i].data_set.object->ob_dist);
				printf("snapshots[%d].data_set.object->ob_direct = %d\n", i, decoded_item->snapshots[i].data_set.object->ob_direct);
				/*(7-3-13)***year****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_year == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.year = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.year);
				}
				/*(7-3-13)***month****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_month == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.month = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.month);
				}
				/*(7-3-13)***day****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_day == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.day = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.day);
				}
				/*(7-3-13)***hour****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_hour == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.hour = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.hour);
				}
				/*(7-3-13)***minute****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_minute == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.minute = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.minute);
				}
				/*(7-3-13)***second****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_second == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.second = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.second);
				}
				/*(7-3-14)***offset****/
				if(decoded_item->snapshots[i].data_set.object->date_time.mask_offset == 1)
				{
					printf("snapshots[%d].data_set.object->date_time.offset = %d\n", i, decoded_item->snapshots[i].data_set.object->date_time.offset);
				}
			}			

			/*(7-3-14)***fullPos****/
			if(decoded_item->snapshots[i].data_set.mask_full_pos == 1)
			{
				/*(7-3-14-1)***utcTime****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_utc_time == 1)
				{						
					/*(7-3-14-1-1)***year****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_year == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->year = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->year);
					}
					/*(7-3-14-1-2)***month****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_month == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->month = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->month);
					}
					/*(7-3-14-1-3)***day****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_day == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->day = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->day);
					}
					/*(7-3-14-1-4)***hour****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_hour == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->hour = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->hour);
					}
					/*(7-3-14-1-5)***minute****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_minute == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->minute = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->minute);
					}
					/*(7-3-14-1-6)***second****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_second == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->second = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->second);
					}					
					/*(7-3-14-1-7)***offset****/
					if(decoded_item->snapshots[i].data_set.full_pos->utc_time->mask_offset == 1)
					{
						printf("snapshots[%d].data_set.full_pos->utc_time->offset = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->utc_time->offset);
					}					
				}
				/*(7-3-14-2)***longitude****/
				printf("snapshots[%d].data_set.full_pos->longitude = %ld\n", i, decoded_item->snapshots[i].data_set.full_pos->longitude);
				/*(7-3-14-3)***lat****/
				printf("snapshots[%d].data_set.full_pos->lat = %ld\n", i, decoded_item->snapshots[i].data_set.full_pos->lat);
				/*(7-3-14-4)***elevation****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_elevation == 1)
				{
					printf("snapshots[%d].data_set.full_pos->elevation = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->elevation);
				}
				/*(7-3-14-5)***heading****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_heading == 1)
				{
					printf("snapshots[%d].data_set.full_pos->heading = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->heading);
				}
				/*(7-3-14-6)***speed****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_speed == 1)
				{
					printf("snapshots[%d].data_set.full_pos->speed->speed = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->speed.speed);
					printf("snapshots[%d].data_set.full_pos->speed->transmission_state = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->speed.transmission_state);
				}				
				/*(7-3-14-7)***posAccuracy****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_pos_accuracy == 1)
				{
					printf("snapshots[%d].data_set.full_pos->pos_accuracy->semi_major = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->pos_accuracy.semi_major);
					printf("snapshots[%d].data_set.full_pos->pos_accuracy->semi_minor = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->pos_accuracy.semi_minor);
					printf("snapshots[%d].data_set.full_pos->pos_accuracy->semi_major_orient = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->pos_accuracy.orientation);
				}
				/*(7-3-14-8)***timeConfidence****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_time_confidence == 1)
				{
					printf("snapshots[%d].data_set.full_pos->time_confidence = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->time_confidence);
				}
				/*(7-3-14-9)***posConfidence****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_pos_confidence == 1)
				{
					printf("snapshots[%d].data_set.full_pos->pos_confidence->pos = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->pos_confidence.pos);
					printf("snapshots[%d].data_set.full_pos->pos_confidence->elevation = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->pos_confidence.elevation);
				}				
				/*(7-3-14-10)***speedConfidence****/
				if(decoded_item->snapshots[i].data_set.full_pos->mask_speed_confidence == 1)
				{
					printf("snapshots[%d].data_set.full_pos->speed_confidence->heading = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->speed_confidence.heading);
					printf("snapshots[%d].data_set.full_pos->speed_confidence->speed = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->speed_confidence.speed);
					printf("snapshots[%d].data_set.full_pos->speed_confidence->throttle = %d\n", i, decoded_item->snapshots[i].data_set.full_pos->speed_confidence.throttle);
				}
			}			
			/*(7-3-15)***throttlePos****/
			if(decoded_item->snapshots[i].data_set.mask_throttle_pos == 1)
			{
				printf("snapshots[%d].data_set.throttle_pos = %d\n", i, decoded_item->snapshots[i].data_set.throttle_pos);
			}
			/*(7-3-16)***speedHeadC****/
			if(decoded_item->snapshots[i].data_set.mask_speed_head_c == 1)
			{
				printf("snapshots[%d].data_set.speed_head_c->heading = %d\n", i, decoded_item->snapshots[i].data_set.speed_head_c.heading);
				printf("snapshots[%d].data_set.speed_head_c->speed = %d\n", i, decoded_item->snapshots[i].data_set.speed_head_c.speed);
				printf("snapshots[%d].data_set.speed_head_c->throttle = %d\n", i, decoded_item->snapshots[i].data_set.speed_head_c.throttle);
			}
			/*(7-3-17)***speedC****/
			if(decoded_item->snapshots[i].data_set.mask_speed_c == 1)
			{
				printf("snapshots[%d].data_set.speed_c = %d\n", i, decoded_item->snapshots[i].data_set.speed_c);
			}
			/*(7-3-18)***vehicleData****/
			if(decoded_item->snapshots[i].data_set.mask_vehicle_data == 1)
			{
				printf("snapshots[%d].data_set.vehicle_data->height = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_data->height);
				printf("snapshots[%d].data_set.vehicle_data->bumpers.frnt = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_data->bumpers.front);
				printf("snapshots[%d].data_set.vehicle_data->bumpers.rear = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_data->bumpers.rear);
				printf("snapshots[%d].data_set.vehicle_data->mass = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_data->mass);
				printf("snapshots[%d].data_set.vehicle_data->trailer_weight = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_data->trailer_weight);
				printf("snapshots[%d].data_set.vehicle_data->type = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_data->type);
			}
			/*(7-3-19)***vehicleIdent****/
			if(decoded_item->snapshots[i].data_set.mask_vehicle_ident == 1)
			{
				/*(7-3-19-1)***name****/
				if(decoded_item->snapshots[i].data_set.vehicle_ident->mask_name == 1)
				{
					printf("snapshots[%d].data_set.vehicle_ident->name = %s\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->name);
				}
				/*(7-3-19-2)***vin****/
				if(decoded_item->snapshots[i].data_set.vehicle_ident->mask_vin == 1)
				{
					printf("snapshots[%d].data_set.vehicle_ident->vin = %s\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->vin);
				}
				/*(7-3-19-3)***ownerCode****/
				if(decoded_item->snapshots[i].data_set.vehicle_ident->mask_owner_code == 1)
				{
					printf("snapshots[%d].data_set.vehicle_ident->owner_code = %s\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->owner_code);
				}
				/*(7-3-19-4)***id****/
				if(decoded_item->snapshots[i].data_set.vehicle_ident->mask_id == 1)
				{
					if(decoded_item->snapshots[i].data_set.vehicle_ident->id.vehicle_id_choice == VEHICLE_ID_ENTITY_ID)
						printf("snapshots[%d].data_set.vehicle_ident->id.entity_id = %s\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->id.entity_id);
					else if(decoded_item->snapshots[i].data_set.vehicle_ident->id.vehicle_id_choice == VEHICLE_ID_STATION_ID)
						printf("snapshots[%d].data_set.vehicle_ident->id.station_id = %ld\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->id.station_id);
				}
				/*(7-3-19-5)***vehicleType****/
				if(decoded_item->snapshots[i].data_set.vehicle_ident->mask_vehicle_type == 1)
				{
					printf("snapshots[%d].data_set.vehicle_ident->vehicle_type = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_type);
				}
				/*(7-3-19-6)***vehicleClass****/
				if(decoded_item->snapshots[i].data_set.vehicle_ident->mask_vehicle_class == 1)
				{						
					if(decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_class_choice == VEHICLE_CLASS_VGROUP)
					{
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class_choice = VEHICLE_CLASS_VGROUP\n", i);
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_class);
					}else if(decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_class_choice == VEHICLE_CLASS_RGROUP)
					{
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class_choice = VEHICLE_CLASS_RGROUP\n", i);
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_class);
					}else if(decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_class_choice == VEHICLE_CLASS_REQUIP)
					{
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class_choice = VEHICLE_CLASS_REQUIP\n", i);
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class = %d\n", i, decoded_item->snapshots[i].data_set.vehicle_ident->vehicle_class);
					}else{
						printf("snapshots[%d].data_set.vehicle_ident->vehicle_class_choice = VEHICLE_IDENT__VEHICLE_CLASS_NOTHING\n", i);
					}
				}				
			}			
			/*(7-3-20)***j1939data****/
			if(decoded_item->snapshots[i].data_set.mask_j1939_data == 1)
			{
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_tires == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->tires_num = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->tires_num);
					for(j = 0; j < decoded_item->snapshots[i].data_set.j1939_data->tires_num; j++)
					{
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_location == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].location = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].location);
						}							
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_pressure == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].pressure = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].pressure);
						}
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_temp == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].temp = %ld\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].temp);
						}
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_wheel_sensor_status == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].wheel_sensor_status = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].wheel_sensor_status);
						}
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_wheel_end_elect_fault == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].wheel_end_elect_fault = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].wheel_end_elect_fault);
						}
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_leakage_rate == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].leakage_rate = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].leakage_rate);
						}
						if(decoded_item->snapshots[i].data_set.j1939_data->tires[j].mask_detection == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->tires[%d].detection = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->tires[j].detection);
						}										
					}
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_axle == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->axle_num = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->axle_num);
					for(j = 0; j < decoded_item->snapshots[i].data_set.j1939_data->axle_num; j++)
					{
						if(decoded_item->snapshots[i].data_set.j1939_data->axle[j].mask_location == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->axle[%d].location = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->axle[j].location);
						}							
						if(decoded_item->snapshots[i].data_set.j1939_data->axle[j].mask_weight == 1)
						{
							printf("snapshots[%d].data_set.j1939_data->axle[%d].weight = %d\n", i, j, decoded_item->snapshots[i].data_set.j1939_data->axle[j].weight);
						}
					}
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_trailer_weight == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->trailer_weight = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->trailer_weight);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_cargo_weight == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->cargo_weight = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->cargo_weight);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_steering_axle_temperature == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->steering_axle_temperature = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->steering_axle_temperature);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_drive_axle_location == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->drive_axle_location = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->drive_axle_location);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_drive_axle_lift_air_pressure == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->drive_axle_lift_air_pressure = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->drive_axle_lift_air_pressure);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_drive_axle_temperature == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->drive_axle_temperature = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->drive_axle_temperature);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_drive_axle_lube_pressure == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->drive_axle_lube_pressure = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->drive_axle_lube_pressure);
				}
				if(decoded_item->snapshots[i].data_set.j1939_data->mask_steering_axle_lube_pressure == 1)
				{
					printf("snapshots[%d].data_set.j1939_data->steering_axle_lube_pressure = %d\n", i, decoded_item->snapshots[i].data_set.j1939_data->steering_axle_lube_pressure);
				}
			}		
			/*(7-3-21)***weatherReport****/
			if(decoded_item->snapshots[i].data_set.mask_weather_report == 1)
			{
				printf("snapshots[%d].data_set.weather_report->is_raining = %d\n", i, decoded_item->snapshots[i].data_set.weather_report->is_raining);
				if(decoded_item->snapshots[i].data_set.weather_report->mask_rain_rate == 1)
				{
					printf("snapshots[%d].data_set.weather_report->rain_rate = %d\n", i, decoded_item->snapshots[i].data_set.weather_report->rain_rate);
				}
				if(decoded_item->snapshots[i].data_set.weather_report->mask_precip_situation == 1)
				{
					printf("snapshots[%d].data_set.weather_report->precip_situation = %d\n", i, decoded_item->snapshots[i].data_set.weather_report->precip_situation);
				}
				if(decoded_item->snapshots[i].data_set.weather_report->mask_solar_radiation == 1)
				{
					printf("snapshots[%d].data_set.weather_report->solar_radiation = %d\n", i, decoded_item->snapshots[i].data_set.weather_report->solar_radiation);
				}
				if(decoded_item->snapshots[i].data_set.weather_report->mask_friction == 1)
				{
					printf("snapshots[%d].data_set.weather_report->friction = %d\n", i, decoded_item->snapshots[i].data_set.weather_report->friction);
				}
			}
			/*(7-3-22)***gpsStatus****/
			if(decoded_item->snapshots[i].data_set.mask_gps_status == 1)
			{
				printf("snapshots[%d].data_set.gps_status = %d\n", i, decoded_item->snapshots[i].data_set.gps_status);
			}			
		}
	}
}


int cmd_rxPvd(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	pvd_standard_item *decoded_item = NULL;
	int err_pvd;

	int err, ret;
	TX_TIMER stop_pvd_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxPvd <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxPvd 123\n");
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
	stop_pvd = 0;
	tx_timer_create(&stop_pvd_timer, NULL, stop_pvd_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_pvd == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (pvd_standard_item *)j2735_pvd_calloc(1, sizeof(pvd_standard_item));
		err_pvd = j2735r63_pvd_decode(decoded_item, in_wsm.data, MAX_PVD_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_pvd < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_pvd_standard_item(decoded_item);}
    
		j2735r63_pvd_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_pvd_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_pvd(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	pvd_standard_item *encode_item = NULL;
	pvd_standard_item *decoded_item = NULL;
	int err_pvd, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 PVD test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_pvd_calloc(1, MAX_PVD_BUFF_SIZE);
		encode_item = (pvd_standard_item *)j2735_pvd_calloc(1, sizeof(pvd_standard_item));

		_fill_pvd_standard_item(encode_item);
		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("pvd loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_pvd_standard_item(&encode_item);}

		err_pvd = j2735r63_pvd_encode(encode_item, encodeBuf, MAX_PVD_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_pvd < 0)
		{
			printf("pvd encode fail...\n");		
			return -1;	
		}
		_cmd_pvd_free_struct(encode_item);

		decoded_item = (pvd_standard_item *)j2735_pvd_calloc(1, sizeof(pvd_standard_item));
		err_pvd = j2735r63_pvd_decode(decoded_item, encodeBuf, MAX_PVD_BUFF_SIZE, ENCODE_PRINT_LEVEL);
		if(err_pvd < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_pvd_standard_item(decoded_item);}
		
		j2735r63_pvd_free_struct(decoded_item);
		j2735_pvd_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}
	
	printf("%s", "J2735R63 pvd Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txPvd(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int err_pvd;
	pvd_standard_item *encode_item = NULL;

	encode_item = (pvd_standard_item *)j2735_pvd_calloc(1, sizeof(pvd_standard_item));
	_fill_pvd_standard_item(encode_item);
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nPVD Encode Data:\n");
		_show_pvd_standard_item(encode_item);
	}
	err_pvd = j2735r63_pvd_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_pvd < 0)
	{				
		printf("pvd encode fail...\n");				
		return -1;		
	}			
	_cmd_pvd_free_struct(encode_item);		

	return 0;
}

int cmd_txPvd(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txPvd <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txPvd 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_pvd_calloc(1, MAX_PVD_BUFF_SIZE);
	_encode_txPvd(encodeBuf, MAX_PVD_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_pvd_free(encodeBuf);
	
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

	printf("%s", "cmd_txPvd command executed successfully.\n");
	return CLI_OK;
}

