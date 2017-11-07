#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_tim_api.h"
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

static int stop_tim = 0;

/***************************************************************/
 /***************************************************************/
 void *j2735_tim_calloc(size_t nmemb, size_t size)
 {
	 unsigned char *ptr = NULL;
 
	 ptr = linux_malloc(nmemb * size);
	 (void) memset(ptr, 0, nmemb * size);
 
	 return ptr;
 }
 
 void j2735_tim_free(void *ptr)
 {
	 linux_free(ptr);
 }
 /***************************************************************/
 /***************************************************************/

void stop_tim_func(ULONG input)
{
	(void)input;
	stop_tim = 1;
	return;
}

void _cmd_tim_free_struct(tim_standard_item *free_tim_item)
{
	int i, j, k, m;

	if(free_tim_item == NULL){
		printf("[TIM] Error: free_tim_struct() input is NULL\n");
	}else{
		if(free_tim_item->data_frames != NULL){
			for(i = 0; i < free_tim_item->data_frames_num; i++){
				if(free_tim_item->data_frames[i].regions != NULL){
					for(j = 0; j < free_tim_item->data_frames[i].regions_num; j++){
						if(free_tim_item->data_frames[i].regions[j].path.xy.nodes != NULL){
							for(k = 0; k < free_tim_item->data_frames[i].regions[j].path.xy.nodes_num; k++){
								if(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.local_node != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.local_node);
								}
	
								if(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.disabled != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.disabled);
								}
	
								if(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.enabled != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.enabled);
								}
	
								if(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data != NULL){
									for(m = 0; m < free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data_num; m++){
										if(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].speed_limits != NULL){
											j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].speed_limits);
										}
									}
									
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data);
								}
							}
							
							j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.xy.nodes);
						}
	
						if(free_tim_item->data_frames[i].regions[j].path.ll.nodes != NULL){
							for(k = 0; k < free_tim_item->data_frames[i].regions[j].path.ll.nodes_num; k++){
								if(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.local_node != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.local_node);
								}
	
								if(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.disabled != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.disabled);
								}
	
								if(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.enabled != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.enabled);
								}
	
								if(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data != NULL){
									for(m = 0; m < free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data_num; m++){
										if(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].speed_limits != NULL){
											j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].speed_limits);
										}
									}
									
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data);
								}
							}
							
							j2735_tim_free(free_tim_item->data_frames[i].regions[j].path.ll.nodes);
						}
	
						if(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes != NULL){
							for(k = 0; k < free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes_num; k++){
								if(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.local_node != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.local_node);
								}
	
								if(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.disabled != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.disabled);
								}
	
								if(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.enabled != NULL){
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.enabled);
								}
	
								if(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data != NULL){
									for(m = 0; m < free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data_num; m++){
										if(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].speed_limits != NULL){
											j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].speed_limits);
										}
									}
									
									j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data);
								}
	
							}
							
							j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes);
						}
	
						if(free_tim_item->data_frames[i].regions[j].old_region.region_point_set.node_list != NULL){ 						
							j2735_tim_free(free_tim_item->data_frames[i].regions[j].old_region.region_point_set.node_list);
						}						
					}
					
					j2735_tim_free(free_tim_item->data_frames[i].regions);
				}
	
				if(free_tim_item->data_frames[i].advisory != NULL){
					j2735_tim_free(free_tim_item->data_frames[i].advisory);
				}
	
				if(free_tim_item->data_frames[i].work_zone != NULL){
					j2735_tim_free(free_tim_item->data_frames[i].work_zone);
				}
	
				if(free_tim_item->data_frames[i].generic_sign != NULL){
					j2735_tim_free(free_tim_item->data_frames[i].generic_sign);
				}
	
				if(free_tim_item->data_frames[i].speed_limit != NULL){
					j2735_tim_free(free_tim_item->data_frames[i].speed_limit);
				}
	
				if(free_tim_item->data_frames[i].exit_service != NULL){
					j2735_tim_free(free_tim_item->data_frames[i].exit_service);
				}
			}
			
			j2735_tim_free(free_tim_item->data_frames);
		}
		j2735_tim_free(free_tim_item);

		//j2735_tim_free(free_tim_item);
	}	
}

void _show_tim_standard_item(tim_standard_item *decoded_item)
{
	int i, j, k, m, n;

	printf("==========TIM Message==========\n");
	printf("message_id = %d\n", decoded_item->message_id);

	/*(1) msgCnt*/
	printf("msg_cnt = %d\n", decoded_item->msg_cnt);
	/*(2) timeStamp, OP*/
	if(decoded_item->mask_time_stamp == 1)
		printf("time_stamp = %d\n", decoded_item->time_stamp);
	/*(3) packetID, OP*/
	if(decoded_item->mask_packet_id == 1)
	{
		printf("packet_id = %s\n", decoded_item->packet_id);
	}	
	/*(4) urlB, OP*/
	if(decoded_item->mask_url_b == 1)
	{
		printf("url_b = %s\n", decoded_item->url_b);
	}	
	/*(5) dataFrames*/
	printf("data_frames_num = %d\n", decoded_item->data_frames_num);
	for(i = 0; i < decoded_item->data_frames_num; i++)
	{
		/* (5-1) sspTimRights */
		printf("data_frames[%d].ssp_tim_rights = %d\n", i, decoded_item->data_frames[i].ssp_tim_rights);
		/* (5-2) frameType */
		printf("data_frames[%d].frame_type = %d\n", i, decoded_item->data_frames[i].frame_type);
		/* (5-3) msgId */
		printf("data_frames[%d].msg_id_choice = %d\n", i, decoded_item->data_frames[i].msg_id_choice);
		if(decoded_item->data_frames[i].msg_id_choice == MSG_ID_FURTHER_INFO_ID)
		{
			printf("data_frames[%d].further_info_id = %x %x\n", i, decoded_item->data_frames[i].further_info_id[0], decoded_item->data_frames[i].further_info_id[1]);
		}
		else if(decoded_item->data_frames[i].msg_id_choice == MSG_ID_ROAD_SIGN_ID)
		{
			printf("data_frames[%d].road_sign_id.position.lat = %ld\n", i, decoded_item->data_frames[i].road_sign_id.position.lat);
			printf("data_frames[%d].road_sign_id.position.longitude = %ld\n", i, decoded_item->data_frames[i].road_sign_id.position.longitude);
			if(decoded_item->data_frames[i].road_sign_id.position.mask_elevation == 1)
			{
				printf("data_frames[%d].road_sign_id.position.elevation = %d\n", i, decoded_item->data_frames[i].road_sign_id.position.elevation);
			}
			printf("data_frames[%d].road_sign_id.view_angle = %hi\n", i, decoded_item->data_frames[i].road_sign_id.view_angle);
			if(decoded_item->data_frames[i].road_sign_id.mask_mutcd_code == 1)
				printf("data_frames[%d].road_sign_id.mutcd_code = %d\n", i, decoded_item->data_frames[i].road_sign_id.mutcd_code);
			if(decoded_item->data_frames[i].road_sign_id.mask_crc == 1)
				printf("data_frames[%d].road_sign_id.crc = %x %x\n", i, decoded_item->data_frames[i].road_sign_id.crc[0], decoded_item->data_frames[i].road_sign_id.crc[1]);

			if(decoded_item->data_frames[i].road_sign_id.position.mask_regional == 1)
			{
				printf("data_frames[%d].road_sign_id.position.regional_num = %d\n", i, decoded_item->data_frames[i].road_sign_id.position.regional_num);
				for(j = 0; j < decoded_item->data_frames[i].road_sign_id.position.regional_num; j++)
				{
					printf("data_frames[%d].road_sign_id.position.regional[%d].region_id = %d\n", i, j, decoded_item->data_frames[i].road_sign_id.position.regional[j].region_id);
					printf("data_frames[%d].road_sign_id.position.regional[%d].reg_ext_value = %d\n", i, j, decoded_item->data_frames[i].road_sign_id.position.regional[j].reg_ext_value);
				}
			}			
		}		
		/* (5-4) startYear, OP */
		if(decoded_item->data_frames[i].mask_start_year == 1){
			printf("data_frames[%d].start_year = %hu\n", i, decoded_item->data_frames[i].start_year);
		}
		/* (5-5) startTime */
		printf("data_frames[%d].start_time = %d\n", i, decoded_item->data_frames[i].start_time);
		/* (5-6) duratonTime */
		printf("data_frames[%d].duraton_time = %hu\n", i, decoded_item->data_frames[i].duraton_time);
		/* (5-7) priority */
		printf("data_frames[%d].priority = %d\n", i, decoded_item->data_frames[i].priority);
		/* (5-8) sspLocationRights */
		printf("data_frames[%d].ssp_location_rights = %d\n", i, decoded_item->data_frames[i].ssp_location_rights);
		/* (5-9) regions */
		printf("data_frames[%d].regions_num = %d\n", i, decoded_item->data_frames[i].regions_num);
		for(j = 0; j < decoded_item->data_frames[i].regions_num; j++)
		{
			/* (5-9-1) name, OP */
			if(decoded_item->data_frames[i].regions[j].mask_name == 1){
				printf("data_frames[%d].regions[%d].name = %s\n", i, j, decoded_item->data_frames[i].regions[j].name);
			}
			/* (5-9-2) id, OP */
			if(decoded_item->data_frames[i].regions[j].mask_id == 1){
				if(decoded_item->data_frames[i].regions[j].id.mask_region == 1)
				{
					printf("data_frames[%d].regions[%d].id.region = %d\n", i, j, decoded_item->data_frames[i].regions[j].id.region);
				}
				printf("data_frames[%d].regions[%d].id.id = %d\n", i, j, decoded_item->data_frames[i].regions[j].id.id);
			}			
			/* (5-9-3) anchor, OP */
			if(decoded_item->data_frames[i].regions[j].mask_anchor == 1){
				printf("data_frames[%d].regions[%d].anchor.lat = %ld\n", i, j, decoded_item->data_frames[i].regions[j].anchor.lat);
				printf("data_frames[%d].regions[%d].anchor.longitude = %ld\n", i, j, decoded_item->data_frames[i].regions[j].anchor.longitude);
				if(decoded_item->data_frames[i].regions[j].anchor.mask_elevation == 1){
					printf("data_frames[%d].regions[%d].anchor.elevation = %d\n", i, j, decoded_item->data_frames[i].regions[j].anchor.elevation);
				}
				if(decoded_item->data_frames[i].regions[j].anchor.mask_regional == 1){
					printf("data_frames[%d].regions[%d].anchor.regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].anchor.regional_num);
					for(k = 0; k < decoded_item->data_frames[i].regions[j].anchor.regional_num; k++)
					{
						printf("data_frames[%d].regions[%d].anchor.regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].anchor.regional[k].region_id);
						printf("data_frames[%d].regions[%d].anchor.regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].anchor.regional[k].reg_ext_value);
					}
				}
			}			
			/* (5-9-4) laneWidth, OP */
			if(decoded_item->data_frames[i].regions[j].mask_lane_width == 1){
				printf("data_frames[%d].regions[%d].lane_width = %hu\n", i, j, decoded_item->data_frames[i].regions[j].lane_width);
			}			
			/* (5-9-5) directionality, OP */
			if(decoded_item->data_frames[i].regions[j].mask_directionality == 1){
				printf("data_frames[%d].regions[%d].directionality = %d\n", i, j, decoded_item->data_frames[i].regions[j].directionality);
			}			
			/* (5-9-6) closedPath, OP */
			if(decoded_item->data_frames[i].regions[j].mask_closed_path == 1){
				printf("data_frames[%d].regions[%d].closed_path = %d\n", i, j, decoded_item->data_frames[i].regions[j].closed_path);
			}			
			/* (5-9-7) direction, OP */
			if(decoded_item->data_frames[i].regions[j].mask_direction == 1){
				printf("data_frames[%d].regions[%d].direction = %hi\n", i, j, decoded_item->data_frames[i].regions[j].direction);
			}			
			/* (5-9-8) description, OP */
			if(decoded_item->data_frames[i].regions[j].mask_description == 1){
				printf("data_frames[%d].regions[%d].description_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].description_choice);
				/* description_choice: GEOGRAPHICALPATH__DESCRIPTION_PATH */
				if(decoded_item->data_frames[i].regions[j].description_choice == GEOGRAPHICALPATH__DESCRIPTION_PATH)
				{
					/*(1) scale, OP */
					if(decoded_item->data_frames[i].regions[j].path.mask_scale == 1){
						printf("data_frames[%d].regions[%d].path.scale = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.scale);
					}
					/*(2) offset */
					printf("data_frames[%d].regions[%d].path.offset_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.offset_choice);
					/* offset_choice: OFFSETSYSTEM__OFFSET_XY */
					if(decoded_item->data_frames[i].regions[j].path.offset_choice == OFFSETSYSTEM__OFFSET_XY)
					{
						printf("data_frames[%d].regions[%d].path.xy.node_list_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.node_list_choice);
						/* node_list_choice: NODE_LISTXY_NODES */	
						if(decoded_item->data_frames[i].regions[j].path.xy.node_list_choice == NODE_LISTXY_NODES)
						{
							printf("data_frames[%d].regions[%d].path.xy.nodes_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.nodes_num);
							for(k = 0; k < decoded_item->data_frames[i].regions[j].path.xy.nodes_num; k++)
							{
								printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.choice = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice);
								/* (1) delta */
								if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY1)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy1.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy1.x);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy1.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy1.y);
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY2)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy2.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy2.x);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy2.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy2.y);
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY3)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy3.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy3.x);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy3.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy3.y);		
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY4)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy4.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy4.x);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy4.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy4.y);
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY5)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy5.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy5.x);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy5.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy5.y);
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY6)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy6.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy6.x);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_xy6.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy6.y);
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_LATLON)
								{
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_lat_lon.lon = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_lat_lon.lon);
									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.node_lat_lon.lat = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_lat_lon.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice == NODE_OFFSET_POINTXY_REGIONAL)
								{
//									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.regional.region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.regional.region_id);
//									printf("data_frames[%d].regions[%d].path.xy.nodes[%d].delta.regional.reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].delta.regional.reg_ext_value);
								}
								/* (2) attributes, OP */
								if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].mask_attributes == 1)
								{
									/* (2-1) localNode, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_local_node == 1)
									{
										printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.local_node_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.local_node_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.local_node_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.local_node[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.local_node[m]);
										}
									}									
									/* (2-2) disabled, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_disabled == 1)
									{
										printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.disabled_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.disabled_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.disabled_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.disabled[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.disabled[m]);
										}
									}									
									/* (2-3) enabled, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_enabled == 1)
									{
										printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.enabled_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.enabled_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.enabled_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.enabled[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.enabled[m]);
										}
									}									
									/* (2-4) data, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_data == 1)
									{
										printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data_num; m++)
										{
											if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_PATHENDPOINTANGLE)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].path_end_point_angle = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].path_end_point_angle);
											}
											else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTCENTER)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].lane_crown_point_center = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_crown_point_center);
											}
											else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTLEFT)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].lane_crown_point_left = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_crown_point_left);
											}
											else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTRIGHT)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].lane_crown_point_right = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_crown_point_right);
											}
											else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANEANGLE)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].lane_angle = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_angle);
											}
											else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_SPEEDLIMITS)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].speed_limits_num = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].speed_limits_num);
												for(n = 0; n < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].speed_limits_num; n++)
												{
													printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].speed_limits[%d].type = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].speed_limits[n].type);
													printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].speed_limits[%d].speed = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].speed_limits[n].speed);
												}		
											}
											else if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_REGIONAL)
											{
												printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].regional_num = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].regional_num);
												for(n = 0; n < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].regional_num; n++)
												{
													printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].regional[%d].region_id = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].regional[n].region_id);
												//	printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.data[%d].regional[%d].reg_ext_value = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.data[m].regional[n].reg_ext_value);
												}
											}
										}
									}
									/* (2-5) dWidth, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_d_width == 1)
									{
										printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.d_width = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.d_width);
									}									
									/* (2-6) dElevation, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_d_elevation == 1)
									{
										printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.d_elevation = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.d_elevation);
									}									
									/* (2-7) regional, OP */
									if(decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.mask_regional == 1)
									{
									/*	printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.regional_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.regional_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.regional_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.regional[%d].region_id = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.regional[m].region_id);
											printf("data_frames[%d].regions[%d].path.xy.nodes[%d].attributes.regional[%d].reg_ext_value = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.xy.nodes[k].attributes.regional[m].reg_ext_value);
										}*/
									}
								}
							}					
						}
						/* node_list_choice: NODE_LISTXY_COMPUTED */					
						else if(decoded_item->data_frames[i].regions[j].path.xy.node_list_choice == NODE_LISTXY_COMPUTED)
						{							
							/*(1) referenceLaneId*/
							printf("data_frames[%d].regions[%d].path.xy.computed.reference_lane_id = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.reference_lane_id);
							/*(2) offsetXaxis*/
							printf("data_frames[%d].regions[%d].path.xy.computed.offset_xaxis_item.offset_xaxis_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.offset_xaxis_item.offset_xaxis_choice);
							if(decoded_item->data_frames[i].regions[j].path.xy.computed.offset_xaxis_item.offset_xaxis_choice == COMPUTEDLANE__OFFSETXAXIS_SMALL)
							{
								printf("data_frames[%d].regions[%d].path.xy.computed.offset_xaxis_item.small = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.offset_xaxis_item.small);
							}
							else if(decoded_item->data_frames[i].regions[j].path.xy.computed.offset_xaxis_item.offset_xaxis_choice == COMPUTEDLANE__OFFSETXAXIS_LARGE)
							{
								printf("data_frames[%d].regions[%d].path.xy.computed.offset_xaxis_item.large = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.offset_xaxis_item.large);
							}												
							/*(3) offsetYaxis */
							if(decoded_item->data_frames[i].regions[j].path.xy.computed.offset_yaxis_item.offset_yaxis_choice == COMPUTEDLANE__OFFSETYAXIS_SMALL)
							{
								printf("data_frames[%d].regions[%d].path.xy.computed.offset_yaxis_item.small = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.offset_yaxis_item.small);
							}
							else if(decoded_item->data_frames[i].regions[j].path.xy.computed.offset_yaxis_item.offset_yaxis_choice == COMPUTEDLANE__OFFSETYAXIS_LARGE)
							{
								printf("data_frames[%d].regions[%d].path.xy.computed.offset_yaxis_item.large = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.offset_yaxis_item.large);
							}							
							/*(4) rotateXY, OP*/
							if(decoded_item->data_frames[i].regions[j].path.xy.computed.mask_rotate_xy == 1){
								printf("data_frames[%d].regions[%d].path.xy.computed.rotate_xy = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.rotate_xy);
							}
							/*(5) scaleXaxis, OP*/
							if(decoded_item->data_frames[i].regions[j].path.xy.computed.mask_scale_xaxis == 1){
								printf("data_frames[%d].regions[%d].path.xy.computed.scale_xaxis = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.scale_xaxis);
							}
							/*(6) scaleYaxis, OP*/
							if(decoded_item->data_frames[i].regions[j].path.xy.computed.mask_scale_yaxis == 1){
								printf("data_frames[%d].regions[%d].path.xy.computed.scale_yaxis = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.xy.computed.scale_yaxis);
							}
						}
					}
					/* offset_choice: OFFSETSYSTEM__OFFSET_LL */				
					else if(decoded_item->data_frames[i].regions[j].path.offset_choice == OFFSETSYSTEM__OFFSET_LL)
					{
						printf("data_frames[%d].regions[%d].path.ll.node_list_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.ll.node_list_choice);
						/* node_list_choice: NODE_LISTLL_NODES */	
						if(decoded_item->data_frames[i].regions[j].path.ll.node_list_choice == NODE_LISTLL_NODES)
						{
							printf("data_frames[%d].regions[%d].path.ll.nodes_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].path.ll.nodes_num);
							for(k = 0; k < decoded_item->data_frames[i].regions[j].path.ll.nodes_num; k++)
							{							
								/* (1) delta */
								if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LL1)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll1.lon = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll1.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll1.lat = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll1.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LL2)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll2.lon = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll2.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll2.lat = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll2.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LL3)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll3.lon = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll3.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll3.lat = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll3.lat);		
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LL4)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll4.lon = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll4.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll4.lat = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll4.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LL5)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll5.lon = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll5.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll5.lat = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll5.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LL6)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll6.lon = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll6.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_ll6.lat = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_ll6.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_LATLON)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_lat_lon.lon = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_lat_lon.lon);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.node_lat_lon.lat = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.node_lat_lon.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.choice == NODE_OFFSET_POINTLL_REGIONAL)
								{
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.regional.region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.regional.region_id);
									printf("data_frames[%d].regions[%d].path.ll.nodes[%d].delta.regional.reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].delta.regional.reg_ext_value);
								}

								/* (2) attributes, OP */
								if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].mask_attributes == 1)
								{
									/* (2-1) localNode, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_local_node == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.local_node_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.local_node_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.local_node_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.local_node[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.local_node[m]);
										}
									}									
									/* (2-2) disabled, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_disabled == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.disabled_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.disabled_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.disabled_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.disabled[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.disabled[m]);
										}
									}									
									/* (2-3) enabled, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_enabled == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.enabled_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.enabled_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.enabled_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.enabled[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.enabled[m]);
										}
									}									
									/* (2-4) data, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_data == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data_num; m++)
										{
											if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_PATHENDPOINTANGLE)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].path_end_point_angle = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].path_end_point_angle);
											}
											else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTCENTER)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].lane_crown_point_center = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_crown_point_center);
											}
											else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTLEFT)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].lane_crown_point_left = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_crown_point_left);
											}
											else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTRIGHT)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].lane_crown_point_right = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_crown_point_right);
											}
											else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANEANGLE)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].lane_angle = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_angle);
											}
											else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_SPEEDLIMITS)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].speed_limits_num = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].speed_limits_num);
												for(n = 0; n < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].speed_limits_num; n++)
												{
													printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].speed_limits[%d].type = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].speed_limits[n].type);
													printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].speed_limits[%d].speed = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].speed_limits[n].speed);
												}		
											}
											else if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_REGIONAL)
											{
												printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].regional_num = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].regional_num);
												for(n = 0; n < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].regional_num; n++)
												{
													printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].regional[%d].region_id = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].regional[n].region_id);
												//	printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.data[%d].regional[%d].reg_ext_value = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.data[m].regional[n].reg_ext_value);
												}
											}
										}
									}
									/* (2-5) dWidth, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_d_width == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.d_width = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.d_width);
									}									
									/* (2-6) dElevation, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_d_elevation == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.d_elevation = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.d_elevation);
									}									
									/* (2-7) regional, OP */
									if(decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.mask_regional == 1)
									{
										printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.regional_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.regional_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.regional_num; m++)
										{
											printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.regional[%d].region_id = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.regional[m].region_id);
											printf("data_frames[%d].regions[%d].path.ll.nodes[%d].attributes.regional[%d].reg_ext_value = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].path.ll.nodes[k].attributes.regional[m].reg_ext_value);
										}
									}
								}
							}
						}
					}
				}
				/* description_choice: GEOGRAPHICALPATH__DESCRIPTION_GEOMETRY */			
				else if(decoded_item->data_frames[i].regions[j].description_choice == GEOGRAPHICALPATH__DESCRIPTION_GEOMETRY)
				{
					/*(1) direction*/
					printf("data_frames[%d].regions[%d].geometry.direction = %hi\n", i, j, decoded_item->data_frames[i].regions[j].geometry.direction);
					/*(2) extent, OP*/
					if(decoded_item->data_frames[i].regions[j].geometry.mask_extent == 1){
						printf("data_frames[%d].regions[%d].geometry.extent = %d\n", i, j, decoded_item->data_frames[i].regions[j].geometry.extent);
					}
					/*(3) laneWidth, OP*/
					if(decoded_item->data_frames[i].regions[j].geometry.mask_lane_width == 1){
						printf("data_frames[%d].regions[%d].geometry.lane_width = %hu\n", i, j, decoded_item->data_frames[i].regions[j].geometry.lane_width);
					}
					/*(4) circle*/	
					printf("data_frames[%d].regions[%d].geometry.circle.center.lat = %ld\n", i, j, decoded_item->data_frames[i].regions[j].geometry.circle.center.lat);
					printf("data_frames[%d].regions[%d].geometry.circle.center.longitude = %ld\n", i, j, decoded_item->data_frames[i].regions[j].geometry.circle.center.longitude);
					if(decoded_item->data_frames[i].regions[j].geometry.circle.center.mask_elevation == 1){
						printf("data_frames[%d].regions[%d].geometry.circle.center.elevation = %d\n", i, j, decoded_item->data_frames[i].regions[j].geometry.circle.center.elevation);
					}
					if(decoded_item->data_frames[i].regions[j].geometry.circle.center.mask_regional == 1){
						printf("data_frames[%d].regions[%d].geometry.circle.center.regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].geometry.circle.center.regional_num);
						for(k = 0; k < decoded_item->data_frames[i].regions[j].geometry.circle.center.regional_num; k++)
						{
							printf("data_frames[%d].regions[%d].geometry.circle.center.regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].geometry.circle.center.regional[k].region_id);
							printf("data_frames[%d].regions[%d].geometry.circle.center.regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].geometry.circle.center.regional[k].reg_ext_value);
						}
					}
					printf("data_frames[%d].regions[%d].geometry.circle.radius = %hu\n", i, j, decoded_item->data_frames[i].regions[j].geometry.circle.radius);
					printf("data_frames[%d].regions[%d].geometry.circle.units = %d\n", i, j, decoded_item->data_frames[i].regions[j].geometry.circle.units);
					/*(5) regional, OP*/
					if(decoded_item->data_frames[i].regions[j].geometry.mask_regional == 1){
						printf("data_frames[%d].regions[%d].geometry.regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].geometry.regional_num);
						for(k = 0; k < decoded_item->data_frames[i].regions[j].geometry.regional_num; k++)
						{
							printf("data_frames[%d].regions[%d].geometry.regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].geometry.regional[k].region_id);
							printf("data_frames[%d].regions[%d].geometry.regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].geometry.regional[k].reg_ext_value);
						}
					}
				}
				/* description_choice: GEOGRAPHICALPATH__DESCRIPTION_OLDREGION */			
				else if(decoded_item->data_frames[i].regions[j].description_choice == GEOGRAPHICALPATH__DESCRIPTION_OLDREGION)
				{
					/*(1) direction*/
					printf("data_frames[%d].regions[%d].old_region.direction = %hi\n", i, j, decoded_item->data_frames[i].regions[j].old_region.direction);
					/*(2) extent, OP*/
					if(decoded_item->data_frames[i].regions[j].old_region.mask_extent == 1){
						printf("data_frames[%d].regions[%d].old_region.extent = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.extent);
					}
					/*(3) area*/
					printf("data_frames[%d].regions[%d].old_region.area_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.area_choice);
					/* area_choice: VALID_REGION__AREA_SHAPE_POINT_SET */
					if(decoded_item->data_frames[i].regions[j].old_region.area_choice == VALID_REGION__AREA_SHAPE_POINT_SET)
					{
						/*(1) anchor, OP*/
						printf("data_frames[%d].regions[%d].old_region.shape_point_set.anchor.lat = %ld\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.lat);
						printf("data_frames[%d].regions[%d].old_region.shape_point_set.anchor.longitude = %ld\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.longitude);
						if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.mask_elevation == 1){
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.anchor.elevation = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.elevation);
						}
						if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.mask_regional == 1){
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.anchor.regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.regional_num);
							for(k = 0; k < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.regional_num; k++)
							{
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.anchor.regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.regional[k].region_id);
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.anchor.regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.anchor.regional[k].reg_ext_value);
							}
						}										
						/*(2) laneWidth, OP*/
						if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.mask_lane_width == 1){
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.lane_width = %hu\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.lane_width);
						}
						/*(3) directionality, OP*/
						if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.mask_directionality == 1){
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.directionality = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.directionality);
						}				
						/*(4) nodeList*/
						/* node_list_choice: NODE_LISTXY_NODES */
						if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.node_list_choice == NODE_LISTXY_NODES)
						{
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes_num);
							for(k = 0; k < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes_num; k++)
							{
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.choice = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice);
								/* (1) delta */
								if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY1)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy1.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy1.x);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy1.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy1.y);
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY2)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy2.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy2.x);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy2.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy2.y);
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY3)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy3.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy3.x);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy3.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy3.y);		
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY4)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy4.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy4.x);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy4.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy4.y);
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY5)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy5.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy5.x);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy5.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy5.y);
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_XY6)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy6.x = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy6.x);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_xy6.y = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_xy6.y);
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_LATLON)
								{
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_lat_lon.lon = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_lat_lon.lon);
									printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.node_lat_lon.lat = %ld\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.node_lat_lon.lat);
								}
								else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.choice == NODE_OFFSET_POINTXY_REGIONAL)
								{
								//	printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.regional.region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.regional.region_id);
								//	printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].delta.regional.reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].delta.regional.reg_ext_value);
								}
								/* (2) attributes, OP */
								if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].mask_attributes == 1)
								{
									/* (2-1) localNode, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_local_node == 1)
									{
										printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.local_node_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.local_node_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.local_node_num; m++)
										{
											printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.local_node[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.local_node[m]);
										}
									}									
									/* (2-2) disabled, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_disabled == 1)
									{
										printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.disabled_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.disabled_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.disabled_num; m++)
										{
											printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.disabled[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.disabled[m]);
										}
									}									
									/* (2-3) enabled, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_enabled == 1)
									{
										printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.enabled_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.enabled_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.enabled_num; m++)
										{
											printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.enabled[%d] = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.enabled[m]);
										}
									}									
									/* (2-4) data, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_data == 1)
									{
										printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data_num; m++)
										{
											if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_PATHENDPOINTANGLE)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].path_end_point_angle = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].path_end_point_angle);
											}
											else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTCENTER)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].lane_crown_point_center = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_crown_point_center);
											}
											else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTLEFT)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].lane_crown_point_left = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_crown_point_left);
											}
											else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANECROWNPOINTRIGHT)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].lane_crown_point_right = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_crown_point_right);
											}
											else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_LANEANGLE)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].lane_angle = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_angle);
											}
											else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_SPEEDLIMITS)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].speed_limits_num = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].speed_limits_num);
												for(n = 0; n < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].speed_limits_num; n++)
												{
													printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].speed_limits[%d].type = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].speed_limits[n].type);
													printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].speed_limits[%d].speed = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].speed_limits[n].speed);
												}		
											}
											else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].lane_data_attribute_choice == LANE_DATA_ATTRIBUTE_REGIONAL)
											{
												printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].regional_num = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].regional_num);
												for(n = 0; n < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].regional_num; n++)
												{
													printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].regional[%d].region_id = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].regional[n].region_id);
					//								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.data[%d].regional[%d].reg_ext_value = %d\n", i, j, k, m, n, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.data[m].regional[n].reg_ext_value);
												}
											}
										}
									}
									/* (2-5) dWidth, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_d_width == 1)
									{
										printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.d_width = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.d_width);
									}									
									/* (2-6) dElevation, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_d_elevation == 1)
									{
										printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.d_elevation = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.d_elevation);
									}									
									/* (2-7) regional, OP */
									if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.mask_regional == 1)
									{
									/*	printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.regional_num = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.regional_num);
										for(m = 0; m < decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.regional_num; m++)
										{
											printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.regional[%d].region_id = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.regional[m].region_id);
											printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.nodes[%d].attributes.regional[%d].reg_ext_value = %d\n", i, j, k, m, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.nodes[k].attributes.regional[m].reg_ext_value);
										}*/
									}
								}
							}
						}
						/* node_list_choice: NODE_LISTXY_COMPUTED */
						else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.node_list_choice == NODE_LISTXY_COMPUTED)
						{
							/*(1) referenceLaneId*/
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.reference_lane_id = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.reference_lane_id);
							/*(2) offsetXaxis*/
							printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.offset_xaxis_item.offset_xaxis_choice = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_xaxis_item.offset_xaxis_choice);
							if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_xaxis_item.offset_xaxis_choice == COMPUTEDLANE__OFFSETXAXIS_SMALL)
							{
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.offset_xaxis_item.small = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_xaxis_item.small);
							}
							else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_xaxis_item.offset_xaxis_choice == COMPUTEDLANE__OFFSETXAXIS_LARGE)
							{
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.offset_xaxis_item.large = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_xaxis_item.large);
							}												
							/*(3) offsetYaxis */
							if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_yaxis_item.offset_yaxis_choice == COMPUTEDLANE__OFFSETYAXIS_SMALL)
							{
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.offset_yaxis_item.small = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_yaxis_item.small);
							}
							else if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_yaxis_item.offset_yaxis_choice == COMPUTEDLANE__OFFSETYAXIS_LARGE)
							{
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.offset_yaxis_item.large = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.offset_yaxis_item.large);
							}							
							/*(4) rotateXY, OP*/
							if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.mask_rotate_xy == 1){
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.rotate_xy = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.rotate_xy);
							}
							/*(5) scaleXaxis, OP*/
							if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.mask_scale_xaxis == 1){
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.scale_xaxis = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.scale_xaxis);
							}
							/*(6) scaleYaxis, OP*/
							if(decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.mask_scale_yaxis == 1){
								printf("data_frames[%d].regions[%d].old_region.shape_point_set.node_list.computed.scale_yaxis = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.shape_point_set.node_list.computed.scale_yaxis);
							}
						}
					}
					/* area_choice: VALID_REGION__AREA_CIRCLE */
					else if(decoded_item->data_frames[i].regions[j].old_region.area_choice == VALID_REGION__AREA_CIRCLE)
					{
						printf("data_frames[%d].regions[%d].old_region.circle.center.lat = %ld\n", i, j, decoded_item->data_frames[i].regions[j].old_region.circle.center.lat);
						printf("data_frames[%d].regions[%d].old_region.circle.center.longitude = %ld\n", i, j, decoded_item->data_frames[i].regions[j].old_region.circle.center.longitude);
						if(decoded_item->data_frames[i].regions[j].old_region.circle.center.mask_elevation == 1){
							printf("data_frames[%d].regions[%d].old_region.circle.center.elevation = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.circle.center.elevation);
						}
						if(decoded_item->data_frames[i].regions[j].old_region.circle.center.mask_regional == 1){
							printf("data_frames[%d].regions[%d].old_region.circle.center.regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.circle.center.regional_num);
							for(k = 0; k < decoded_item->data_frames[i].regions[j].old_region.circle.center.regional_num; k++)
							{
								printf("data_frames[%d].regions[%d].old_region.circle.center.regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.circle.center.regional[k].region_id);
								printf("data_frames[%d].regions[%d].old_region.circle.center.regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.circle.center.regional[k].reg_ext_value);
							}
						}
						printf("data_frames[%d].regions[%d].old_region.circle.radius = %hu\n", i, j, decoded_item->data_frames[i].regions[j].old_region.circle.radius);
						printf("data_frames[%d].regions[%d].old_region.circle.units = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.circle.units);
					}
					/* area_choice: VALID_REGION__AREA_REGION_POINT_SET */
					else if(decoded_item->data_frames[i].regions[j].old_region.area_choice == VALID_REGION__AREA_REGION_POINT_SET)
					{
						
						/* (1) anchor, OP*/
						if(decoded_item->data_frames[i].regions[j].old_region.region_point_set.mask_anchor == 1){
							printf("data_frames[%d].regions[%d].old_region.region_point_set.anchor.lat = %ld\n", i, j, decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.lat);
							printf("data_frames[%d].regions[%d].old_region.region_point_set.anchor.longitude = %ld\n", i, j, decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.longitude);
							if(decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.mask_elevation == 1){
								printf("data_frames[%d].regions[%d].old_region.region_point_set.anchor.elevation = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.elevation);
							}
							if(decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.mask_regional == 1){
								printf("data_frames[%d].regions[%d].old_region.region_point_set.anchor.regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.regional_num);
								for(k = 0; k < decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.regional_num; k++)
								{
									printf("data_frames[%d].regions[%d].old_region.region_point_set.anchor.regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.regional[k].region_id);
									printf("data_frames[%d].regions[%d].old_region.region_point_set.anchor.regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.region_point_set.anchor.regional[k].reg_ext_value);
								}
							}
						}
						/* (2) scale, OP*/
						if(decoded_item->data_frames[i].regions[j].old_region.region_point_set.mask_scale == 1){
							printf("data_frames[%d].regions[%d].old_region.region_point_set.scale = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.region_point_set.scale);
						}
						/* (3) nodeList*/
						printf("data_frames[%d].regions[%d].old_region.region_point_set.node_list_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].old_region.region_point_set.node_list_num);
						for(k = 0; k < decoded_item->data_frames[i].regions[j].old_region.region_point_set.node_list_num; k++)
						{
							/* (1) xOffset*/
							printf("data_frames[%d].regions[%d].old_region.region_point_set.node_list[%d].x_offset = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.region_point_set.node_list[k].x_offset);
							/* (2) yOffset*/
							printf("data_frames[%d].regions[%d].old_region.region_point_set.node_list[%d].y_offset = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.region_point_set.node_list[k].y_offset);
							/* (3) zOffset, OP*/
							if(decoded_item->data_frames[i].regions[j].old_region.region_point_set.node_list[k].mask_z_offset == 1){
								printf("data_frames[%d].regions[%d].old_region.region_point_set.node_list[%d].z_offset = %hi\n", i, j, k, decoded_item->data_frames[i].regions[j].old_region.region_point_set.node_list[k].z_offset);
							}
						}
					}
				}		
			}	
			/* (5-9-9) regional, OP */
			if(decoded_item->data_frames[i].regions[j].mask_regional == 1){
				printf("data_frames[%d].regions[%d].regional_num = %d\n", i, j, decoded_item->data_frames[i].regions[j].regional_num);
				for(k = 0; k < decoded_item->data_frames[i].regions[j].regional_num; k++)
				{
					printf("data_frames[%d].regions[%d].regional[%d].region_id = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].regional[k].region_id);
					printf("data_frames[%d].regions[%d].regional[%d].reg_ext_value = %d\n", i, j, k, decoded_item->data_frames[i].regions[j].regional[k].reg_ext_value);
				}				
			}
		}		
		/* (5-10) sspMsgRights1 */
		printf("data_frames[%d].ssp_msg_rights_1 = %d\n", i, decoded_item->data_frames[i].ssp_msg_rights_1);
		/* (5-11) sspMsgRights2 */
		printf("data_frames[%d].ssp_msg_rights_2 = %d\n", i, decoded_item->data_frames[i].ssp_msg_rights_2);
		/* (5-12) content */
		if(decoded_item->data_frames[i].content_choice == CONTENT_ADVISORY)
		{
			printf("data_frames[%d].advisory_num = %d\n", i, decoded_item->data_frames[i].advisory_num);
			for(j = 0; j < decoded_item->data_frames[i].advisory_num; j++)
			{
				if(decoded_item->data_frames[i].advisory[j].itis_item_choice == ITIS_ITEM_ITIS)
				{
					printf("data_frames[%d].advisory[%d].itis = %d\n", i, j, decoded_item->data_frames[i].advisory[j].itis);
				}
				else if(decoded_item->data_frames[i].advisory[j].itis_item_choice == ITIS_ITEM_TEXT)
				{
					printf("data_frames[%d].advisory[%d].text = %s\n", i, j, decoded_item->data_frames[i].advisory[j].text);
				}
			}
		}
		else if(decoded_item->data_frames[i].content_choice == CONTENT_WORKZONE)
		{
			printf("data_frames[%d].work_zone_num = %d\n", i, decoded_item->data_frames[i].work_zone_num);
			for(j = 0; j < decoded_item->data_frames[i].work_zone_num; j++)
			{
				if(decoded_item->data_frames[i].work_zone[j].item_choice == CONTENTITEM_ITEM_ITIS)
				{
					printf("data_frames[%d].work_zone[%d].itis = %d\n", i, j, decoded_item->data_frames[i].work_zone[j].itis);
				}
				else if(decoded_item->data_frames[i].work_zone[j].item_choice == CONTENTITEM_ITEM_TEXT)
				{
					printf("data_frames[%d].work_zone[%d].text = %s\n", i, j, decoded_item->data_frames[i].work_zone[j].text);
				}
			}
		}
		else if(decoded_item->data_frames[i].content_choice == CONTENT_GENERICSIGN)
		{
			printf("data_frames[%d].generic_sign_num = %d\n", i, decoded_item->data_frames[i].generic_sign_num);
			for(j = 0; j < decoded_item->data_frames[i].generic_sign_num; j++)
			{
				if(decoded_item->data_frames[i].generic_sign[j].item_choice == CONTENTITEM_ITEM_ITIS)
				{
					printf("data_frames[%d].generic_sign[%d].itis = %d\n", i, j, decoded_item->data_frames[i].generic_sign[j].itis);
				}
				else if(decoded_item->data_frames[i].generic_sign[j].item_choice == CONTENTITEM_ITEM_TEXT)
				{
					printf("data_frames[%d].generic_sign[%d].text = %s\n", i, j, decoded_item->data_frames[i].generic_sign[j].text);
				}
			}
		}
		else if(decoded_item->data_frames[i].content_choice == CONTENT_SPEEDLIMIT)
		{
			printf("data_frames[%d].speed_limit_num = %d\n", i, decoded_item->data_frames[i].speed_limit_num);
			for(j = 0; j < decoded_item->data_frames[i].speed_limit_num; j++)
			{
				if(decoded_item->data_frames[i].speed_limit[j].item_choice == CONTENTITEM_ITEM_ITIS)
				{
					printf("data_frames[%d].speed_limit[%d].itis = %d\n", i, j, decoded_item->data_frames[i].speed_limit[j].itis);
				}
				else if(decoded_item->data_frames[i].speed_limit[j].item_choice == CONTENTITEM_ITEM_TEXT)
				{
					printf("data_frames[%d].speed_limit[%d].text = %s\n", i, j, decoded_item->data_frames[i].speed_limit[j].text);
				}
			}
		}
		else if(decoded_item->data_frames[i].content_choice == CONTENT_EXITSERVICE)
		{
			printf("data_frames[%d].exit_service_num = %d\n", i, decoded_item->data_frames[i].exit_service_num);
			for(j = 0; j < decoded_item->data_frames[i].exit_service_num; j++)
			{
				if(decoded_item->data_frames[i].exit_service[j].item_choice == CONTENTITEM_ITEM_ITIS)
				{
					printf("data_frames[%d].exit_service[%d].itis = %d\n", i, j, decoded_item->data_frames[i].exit_service[j].itis);
				}
				else if(decoded_item->data_frames[i].exit_service[j].item_choice == CONTENTITEM_ITEM_TEXT)
				{
					printf("data_frames[%d].exit_service[%d].text = %s\n", i, j, decoded_item->data_frames[i].exit_service[j].text);
				}
			}
		}		
		/* (5-13) url, OP */
		if(decoded_item->data_frames[i].mask_url == 1)
		{
			printf("data_frames[%d].url = %s\n", i, decoded_item->data_frames[i].url);
		}
	}	
	/*(6) regional, OP*/
	if(decoded_item->mask_regional == 1)
	{
		printf("regional_num = %d\n", decoded_item->regional_num);
		for(i = 0; i < decoded_item->regional_num; i++)
		{
			printf("regional[%d].region_id = %d\n", i, decoded_item->regional[i].region_id);
			printf("regional[%d].reg_ext_value = %d\n", i, decoded_item->regional[i].reg_ext_value);
		}
	}
	
	printf("===============================\n");
}

int cmd_rxTim(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	tim_standard_item *decoded_item = NULL;
	int err_tim;

	int err, ret;
	TX_TIMER stop_tim_timer;
	unsigned int psid;
	wme_handle_t handle;
	unsigned short serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxTim <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxTim 123\n");
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
	stop_tim = 0;
	tx_timer_create(&stop_tim_timer, NULL, stop_tim_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_tim == 0) {
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) {
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
		
		decoded_item = (tim_standard_item *)j2735_tim_calloc(1, sizeof(tim_standard_item));
		err_tim = j2735r63_tim_decode(decoded_item, in_wsm.data, MAX_TIM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_tim < 0)
		{
			printf("decode error...\n");
			return -1;
		}
    
		if(SHOW_DECODED == 1){_show_tim_standard_item(decoded_item);}
    
		j2735r63_tim_free_struct(decoded_item);
	}
	
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) {
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_tim_timer);
	wme_exit(&handle);

	return 0;
}

int cmd_j2735r63_tim(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */  	
	(void)command;  	
	(void)argv;
	(void)argc;
	(void)cli;

	tim_standard_item *encode_item = NULL;
	tim_standard_item *decoded_item = NULL;
	int err_tim, i, j, k, loopCnt;  
	size_t size;
	unsigned char *encodeBuf = NULL;

	printf("%s", "Start J2735R63 TIM test!\n");

	loopCnt = 0;
	while(loopCnt < MAX_LOOP_COUNT)
	{
		encodeBuf = (unsigned char *)j2735_tim_calloc(1, MAX_TIM_BUFF_SIZE);
		encode_item = (tim_standard_item *)j2735_tim_calloc(1, sizeof(tim_standard_item));
		/*(1) msgCnt*/
		encode_item->msg_cnt = 127;
		/*(2) timeStamp, OPTIONAL*/
		encode_item->mask_time_stamp = 1;
		encode_item->time_stamp = 527040;
		/*(3) packetID, OPTIONAL*/
		encode_item->mask_packet_id = 1;
		for(i = 0; i < 8; i++)
			encode_item->packet_id[i] = 0x31 + i;
		encode_item->packet_id[8] = '\0';
		/*(4) urlB, OPTIONAL*/
		encode_item->mask_url_b = 1;
		memset(encode_item->url_b, 0, 46);
		memcpy(encode_item->url_b, "TIM URL IA5String", strlen("TIM URL IA5String"));
		encode_item->url_b[strlen("TIM URL IA5String")+1] = '\0';
		/*(5) dataFrames*/
		encode_item->data_frames_num = 1;
		encode_item->data_frames = (traveler_data_frame_item *)j2735_tim_calloc(encode_item->data_frames_num, sizeof(traveler_data_frame_item));
		if(!(encode_item->data_frames))
		{
			printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
			return -MEMORY_ALLOCATE_FAIL;		 
		}
		else
		{
			for(i = 0; i < encode_item->data_frames_num; i++)
			{
				/*Part I, Frame header*/
				/*(5-1)dataFrames->sspTimRights*/
				encode_item->data_frames[i].ssp_tim_rights = 31;
				/*(5-2)dataFrames->frameType*/
				encode_item->data_frames[i].frame_type = 3;
	
				/*(5-3)Case 1: CHOICE_FurtherInfoID*/
	
				encode_item->data_frames[i].msg_id_choice = MSG_ID_FURTHER_INFO_ID; 	
				encode_item->data_frames[i].further_info_id[0] = 0x11;
				encode_item->data_frames[i].further_info_id[1] = 0x22;
				
				memset(&(encode_item->data_frames[i].road_sign_id), 0, sizeof(encode_item->data_frames[i].road_sign_id));
	
				/* (5-3)Case 2: CHOICE_ROADSIGNID*/
	/*			
				encode_item->data_frames[i].msg_id_choice = MSG_ID_ROAD_SIGN_ID;		
				encode_item->data_frames[i].road_sign_id.position.lat = 900000001;
				encode_item->data_frames[i].road_sign_id.position.longitude = 1800000001;
				encode_item->data_frames[i].road_sign_id.position.mask_elevation = 1;		
				encode_item->data_frames[i].road_sign_id.position.elevation = 61439;		
				encode_item->data_frames[i].road_sign_id.view_angle = 15;
				encode_item->data_frames[i].road_sign_id.mask_mutcd_code = 1;		
				encode_item->data_frames[i].road_sign_id.mutcd_code = 6;						
				encode_item->data_frames[i].road_sign_id.mask_crc = 1;		
				encode_item->data_frames[i].road_sign_id.crc[0] = 0x41;
				encode_item->data_frames[i].road_sign_id.crc[1] = 0x42; 	
	*/			
	
				/*(5-4)***dataFrames->startYear******OP***/
				encode_item->data_frames[i].mask_start_year = 1;		
				encode_item->data_frames[i].start_year = 4095;		
				/*(5-5)***dataFrames->startTime****/
				encode_item->data_frames[i].start_time = 527040;
				/*(5-6)***dataFrames->duratonTime****/
				encode_item->data_frames[i].duraton_time = 32000;
				/*(5-7)***dataFrames->priority****/
				encode_item->data_frames[i].priority = 7;
				/*(5-8)dataFrames->sspTimRights*/
				encode_item->data_frames[i].ssp_location_rights = 31;
	
				/*(5-9) dataFrames->regions*/
				encode_item->data_frames[i].regions_num = 1;
				encode_item->data_frames[i].regions = (geographical_path_item *)j2735_tim_calloc(encode_item->data_frames[i].regions_num, sizeof(geographical_path_item));
				if(!(encode_item->data_frames[i].regions))
				{
					printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}
				else
				{
					for(j = 0; j < encode_item->data_frames[i].regions_num; j++)
					{
						/*(5-9-1) name, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_name = 1;
						memset(encode_item->data_frames[i].regions[j].name, 0, 64);
						memcpy(encode_item->data_frames[i].regions[j].name, "TIM DescriptiveName IA5String", strlen("TIM DescriptiveName IA5String"));
						encode_item->data_frames[i].regions[j].name[strlen("TIM DescriptiveName IA5String")+1] = '\0';
						/*(5-9-2) id, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_id = 1;
						encode_item->data_frames[i].regions[j].id.mask_region = 1;
						encode_item->data_frames[i].regions[j].id.region = 65535;
						encode_item->data_frames[i].regions[j].id.id = 65535;
						/*(5-9-3) anchor, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_anchor = 1;
						encode_item->data_frames[i].regions[j].anchor.lat = 900000001;
						encode_item->data_frames[i].regions[j].anchor.longitude = 1800000001;
						encode_item->data_frames[i].regions[j].anchor.mask_elevation = 1;
						encode_item->data_frames[i].regions[j].anchor.elevation = 61439;
						/*(5-9-4) laneWidth, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_lane_width = 1;
						encode_item->data_frames[i].regions[j].lane_width = 32767;
						/*(5-9-5) directionality, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_directionality = 1;
						encode_item->data_frames[i].regions[j].directionality = 3;
						/*(5-9-6) closedPath, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_closed_path = 1;
						encode_item->data_frames[i].regions[j].closed_path = 1;
						/*(5-9-7) direction, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_direction = 1;
						encode_item->data_frames[i].regions[j].direction = 15;
	
						/*(5-9-8) description, OPTIONAL*/
						encode_item->data_frames[i].regions[j].mask_description = 1;
	
						/*(5-9-8) Case: OffsetSystem*/
	
						encode_item->data_frames[i].regions[j].description_choice = GEOGRAPHICALPATH__DESCRIPTION_PATH;
						encode_item->data_frames[i].regions[j].path.mask_scale = 1;
						encode_item->data_frames[i].regions[j].path.scale = 15;
						encode_item->data_frames[i].regions[j].path.offset_choice = OFFSETSYSTEM__OFFSET_XY;
						encode_item->data_frames[i].regions[j].path.xy.node_list_choice = NODE_LISTXY_NODES;
						encode_item->data_frames[i].regions[j].path.xy.nodes_num = 2;
						encode_item->data_frames[i].regions[j].path.xy.nodes = (nodeXY_item *)j2735_tim_calloc(encode_item->data_frames[i].regions[j].path.xy.nodes_num, sizeof(nodeXY_item));
						if(!(encode_item->data_frames[i].regions[j].path.xy.nodes))
						{
							printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
							return -MEMORY_ALLOCATE_FAIL;		 
						}
						else
						{
							for(k = 0; k < encode_item->data_frames[i].regions[j].path.xy.nodes_num; k++)
							{
								encode_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice = NODE_OFFSET_POINTXY_XY4;
								encode_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy4.x = 4094;
								encode_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy4.y = 4094;
								
								encode_item->data_frames[i].regions[j].path.xy.nodes[k].mask_attributes = 0;
							}
						}
	
						/*(5-9-8) Case: GeometricProjection*/
	/*					
						encode_item->data_frames[i].regions[j].description_choice = GEOGRAPHICALPATH__DESCRIPTION_GEOMETRY;
						encode_item->data_frames[i].regions[j].geometry.direction = 15;
						encode_item->data_frames[i].regions[j].geometry.mask_extent = 1;
						encode_item->data_frames[i].regions[j].geometry.extent = 15;
						encode_item->data_frames[i].regions[j].geometry.mask_lane_width = 1;
						encode_item->data_frames[i].regions[j].geometry.lane_width = 32767;
						encode_item->data_frames[i].regions[j].geometry.circle.center.lat = 900000001;
						encode_item->data_frames[i].regions[j].geometry.circle.center.longitude = 1800000001;
						encode_item->data_frames[i].regions[j].geometry.circle.center.mask_elevation = 1;
						encode_item->data_frames[i].regions[j].geometry.circle.center.elevation = 61439;
						encode_item->data_frames[i].regions[j].geometry.circle.radius = 4095;
						encode_item->data_frames[i].regions[j].geometry.circle.units = 7;
	*/
					}
					
				}
				/*(5-10)dataFrames->sspMsgRights1*/
				encode_item->data_frames[i].ssp_msg_rights_1= 31;
				encode_item->data_frames[i].ssp_msg_rights_2= 31;
				encode_item->data_frames[i].content_choice = CONTENT_WORKZONE;
				encode_item->data_frames[i].work_zone_num = 1;
				encode_item->data_frames[i].work_zone = (content_item *)j2735_tim_calloc(encode_item->data_frames[i].work_zone_num, sizeof(content_item));
				if(!(encode_item->data_frames[i].work_zone))
				{
					printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
					return -MEMORY_ALLOCATE_FAIL;		 
				}
				else
				{
					for(j = 0; j < encode_item->data_frames[i].work_zone_num; j++)
					{
						encode_item->data_frames[i].work_zone[j].item_choice = CONTENTITEM_ITEM_ITIS;
						encode_item->data_frames[i].work_zone[j].itis = 65535;
					}
				}
				/*(5-13) url, OPTIONAL*/
				encode_item->data_frames[i].mask_url = 1;
				memset(encode_item->data_frames[i].url, 0, 15);
				memcpy(encode_item->data_frames[i].url, "TIM URL", strlen("TIM URL"));
				encode_item->data_frames[i].url[strlen("TIM URL")+1] = '\0';
	
	
	
			}			
		}
		encode_item->mask_regional = 0;
		encode_item->regional_num = 0;

		/********************************************************************/

		if((loopCnt % 100) == 0)	
			printf("TIM loopCnt = %d\n", loopCnt + 1);

		//if(SHOW_ENCODE == 1){_show_tim_standard_item(&encode_item);}

		err_tim = j2735r63_tim_encode(encode_item, encodeBuf, MAX_TIM_BUFF_SIZE, &size, ENCODE_PRINT_LEVEL);
		if(err_tim < 0)
		{
			printf("tim encode fail...\n");		
			return -1;	
		}
		_cmd_tim_free_struct(encode_item);

		decoded_item = (tim_standard_item *)j2735_tim_calloc(1, sizeof(tim_standard_item));
		err_tim = j2735r63_tim_decode(decoded_item, encodeBuf, MAX_TIM_BUFF_SIZE, DECODE_PRINT_LEVEL);
		if(err_tim < 0)
		{
			printf("decode error...\n");
			return -1;
		}

		if(SHOW_DECODED == 1){_show_tim_standard_item(decoded_item);}
		
		j2735r63_tim_free_struct(decoded_item);
		j2735_tim_free(encodeBuf);

		loopCnt++;
		(void) tx_thread_sleep(10);
	}

	printf("%s", "J2735R63 TIM Test command executed successfully.\n");
		
	return CLI_OK;	
}

int _encode_txTim(unsigned char *encodeBuf, size_t input_buf_size, size_t *size)
{
	int i, j, k, err_tim;
	tim_standard_item *encode_item = NULL;

	encode_item = (tim_standard_item *)j2735_tim_calloc(1, sizeof(tim_standard_item));
	/*(1) msgCnt*/
	encode_item->msg_cnt = 127;
	/*(2) timeStamp, OPTIONAL*/
	encode_item->mask_time_stamp = 1;
	encode_item->time_stamp = 527040;
	/*(3) packetID, OPTIONAL*/
	encode_item->mask_packet_id = 1;
	for(i = 0; i < 8; i++)
		encode_item->packet_id[i] = 0x31 + i;
	encode_item->packet_id[8] = '\0';
	/*(4) urlB, OPTIONAL*/
	encode_item->mask_url_b = 1;
	memset(encode_item->url_b, 0, 46);
	memcpy(encode_item->url_b, "TIM URL IA5String", strlen("TIM URL IA5String"));
	encode_item->url_b[strlen("TIM URL IA5String")+1] = '\0';
	/*(5) dataFrames*/
	encode_item->data_frames_num = 1;
	encode_item->data_frames = (traveler_data_frame_item *)j2735_tim_calloc(encode_item->data_frames_num, sizeof(traveler_data_frame_item));
	if(!(encode_item->data_frames))
	{
		printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
		return -MEMORY_ALLOCATE_FAIL;		 
	}
	else
	{
		for(i = 0; i < encode_item->data_frames_num; i++)
		{
			/*Part I, Frame header*/
			/*(5-1)dataFrames->sspTimRights*/
			encode_item->data_frames[i].ssp_tim_rights = 31;
			/*(5-2)dataFrames->frameType*/
			encode_item->data_frames[i].frame_type = 3;

			/*(5-3)Case 1: CHOICE_FurtherInfoID*/

			encode_item->data_frames[i].msg_id_choice = MSG_ID_FURTHER_INFO_ID;		
			encode_item->data_frames[i].further_info_id[0] = 0x11;
			encode_item->data_frames[i].further_info_id[1] = 0x22;
			
			memset(&(encode_item->data_frames[i].road_sign_id), 0, sizeof(encode_item->data_frames[i].road_sign_id));

			/* (5-3)Case 2: CHOICE_ROADSIGNID*/
/*			
			encode_item->data_frames[i].msg_id_choice = MSG_ID_ROAD_SIGN_ID;		
			encode_item->data_frames[i].road_sign_id.position.lat = 900000001;
			encode_item->data_frames[i].road_sign_id.position.longitude = 1800000001;
			encode_item->data_frames[i].road_sign_id.position.mask_elevation = 1;		
			encode_item->data_frames[i].road_sign_id.position.elevation = 61439;		
			encode_item->data_frames[i].road_sign_id.view_angle = 15;
			encode_item->data_frames[i].road_sign_id.mask_mutcd_code = 1;		
			encode_item->data_frames[i].road_sign_id.mutcd_code = 6;						
			encode_item->data_frames[i].road_sign_id.mask_crc = 1;		
			encode_item->data_frames[i].road_sign_id.crc[0] = 0x41;
			encode_item->data_frames[i].road_sign_id.crc[1] = 0x42;		
*/			

			/*(5-4)***dataFrames->startYear******OP***/
			encode_item->data_frames[i].mask_start_year = 1;		
			encode_item->data_frames[i].start_year = 4095;		
			/*(5-5)***dataFrames->startTime****/
			encode_item->data_frames[i].start_time = 527040;
			/*(5-6)***dataFrames->duratonTime****/
			encode_item->data_frames[i].duraton_time = 32000;
			/*(5-7)***dataFrames->priority****/
			encode_item->data_frames[i].priority = 7;
			/*(5-8)dataFrames->sspTimRights*/
			encode_item->data_frames[i].ssp_location_rights = 31;

			/*(5-9) dataFrames->regions*/
			encode_item->data_frames[i].regions_num = 1;
			encode_item->data_frames[i].regions = (geographical_path_item *)j2735_tim_calloc(encode_item->data_frames[i].regions_num, sizeof(geographical_path_item));
			if(!(encode_item->data_frames[i].regions))
			{
				printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			else
			{
				for(j = 0; j < encode_item->data_frames[i].regions_num; j++)
				{
					/*(5-9-1) name, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_name = 1;
					memset(encode_item->data_frames[i].regions[j].name, 0, 64);
					memcpy(encode_item->data_frames[i].regions[j].name, "TIM DescriptiveName IA5String", strlen("TIM DescriptiveName IA5String"));
					encode_item->data_frames[i].regions[j].name[strlen("TIM DescriptiveName IA5String")+1] = '\0';
					/*(5-9-2) id, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_id = 1;
					encode_item->data_frames[i].regions[j].id.mask_region = 1;
					encode_item->data_frames[i].regions[j].id.region = 65535;
					encode_item->data_frames[i].regions[j].id.id = 65535;
					/*(5-9-3) anchor, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_anchor = 1;
					encode_item->data_frames[i].regions[j].anchor.lat = 900000001;
					encode_item->data_frames[i].regions[j].anchor.longitude = 1800000001;
					encode_item->data_frames[i].regions[j].anchor.mask_elevation = 1;
					encode_item->data_frames[i].regions[j].anchor.elevation = 61439;
					/*(5-9-4) laneWidth, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_lane_width = 1;
					encode_item->data_frames[i].regions[j].lane_width = 32767;
					/*(5-9-5) directionality, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_directionality = 1;
					encode_item->data_frames[i].regions[j].directionality = 3;
					/*(5-9-6) closedPath, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_closed_path = 1;
					encode_item->data_frames[i].regions[j].closed_path = 1;
					/*(5-9-7) direction, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_direction = 1;
					encode_item->data_frames[i].regions[j].direction = 15;

					/*(5-9-8) description, OPTIONAL*/
					encode_item->data_frames[i].regions[j].mask_description = 1;

					/*(5-9-8) Case: OffsetSystem*/

					encode_item->data_frames[i].regions[j].description_choice = GEOGRAPHICALPATH__DESCRIPTION_PATH;
					encode_item->data_frames[i].regions[j].path.mask_scale = 1;
					encode_item->data_frames[i].regions[j].path.scale = 15;
					encode_item->data_frames[i].regions[j].path.offset_choice = OFFSETSYSTEM__OFFSET_XY;
					encode_item->data_frames[i].regions[j].path.xy.node_list_choice = NODE_LISTXY_NODES;
					encode_item->data_frames[i].regions[j].path.xy.nodes_num = 2;
					encode_item->data_frames[i].regions[j].path.xy.nodes = (nodeXY_item *)j2735_tim_calloc(encode_item->data_frames[i].regions[j].path.xy.nodes_num, sizeof(nodeXY_item));
					if(!(encode_item->data_frames[i].regions[j].path.xy.nodes))
					{
						printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
						return -MEMORY_ALLOCATE_FAIL;		 
					}
					else
					{
						for(k = 0; k < encode_item->data_frames[i].regions[j].path.xy.nodes_num; k++)
						{
							encode_item->data_frames[i].regions[j].path.xy.nodes[k].delta.choice = NODE_OFFSET_POINTXY_XY4;
							encode_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy4.x = 4094;
							encode_item->data_frames[i].regions[j].path.xy.nodes[k].delta.node_xy4.y = 4094;
							
							encode_item->data_frames[i].regions[j].path.xy.nodes[k].mask_attributes = 0;
						}
					}

					/*(5-9-8) Case: GeometricProjection*/
/*					
					encode_item->data_frames[i].regions[j].description_choice = GEOGRAPHICALPATH__DESCRIPTION_GEOMETRY;
					encode_item->data_frames[i].regions[j].geometry.direction = 15;
					encode_item->data_frames[i].regions[j].geometry.mask_extent = 1;
					encode_item->data_frames[i].regions[j].geometry.extent = 15;
					encode_item->data_frames[i].regions[j].geometry.mask_lane_width = 1;
					encode_item->data_frames[i].regions[j].geometry.lane_width = 32767;
					encode_item->data_frames[i].regions[j].geometry.circle.center.lat = 900000001;
					encode_item->data_frames[i].regions[j].geometry.circle.center.longitude = 1800000001;
					encode_item->data_frames[i].regions[j].geometry.circle.center.mask_elevation = 1;
					encode_item->data_frames[i].regions[j].geometry.circle.center.elevation = 61439;
					encode_item->data_frames[i].regions[j].geometry.circle.radius = 4095;
					encode_item->data_frames[i].regions[j].geometry.circle.units = 7;
*/
				}
				
			}
			/*(5-10)dataFrames->sspMsgRights1*/
			encode_item->data_frames[i].ssp_msg_rights_1= 31;
			encode_item->data_frames[i].ssp_msg_rights_2= 31;
			encode_item->data_frames[i].content_choice = CONTENT_WORKZONE;
			encode_item->data_frames[i].work_zone_num = 1;
			encode_item->data_frames[i].work_zone = (content_item *)j2735_tim_calloc(encode_item->data_frames[i].work_zone_num, sizeof(content_item));
			if(!(encode_item->data_frames[i].work_zone))
			{
				printf("[TIM] Error: MEMORY_ALLOCATE_FAIL\n");
				return -MEMORY_ALLOCATE_FAIL;		 
			}
			else
			{
				for(j = 0; j < encode_item->data_frames[i].work_zone_num; j++)
				{
					encode_item->data_frames[i].work_zone[j].item_choice = CONTENTITEM_ITEM_ITIS;
					encode_item->data_frames[i].work_zone[j].itis = 65535;
				}
			}
			/*(5-13) url, OPTIONAL*/
			encode_item->data_frames[i].mask_url = 1;
			memset(encode_item->data_frames[i].url, 0, 15);
			memcpy(encode_item->data_frames[i].url, "TIM URL", strlen("TIM URL"));
			encode_item->data_frames[i].url[strlen("TIM URL")+1] = '\0';



		}			
	}
	encode_item->mask_regional = 0;
	encode_item->regional_num = 0;

	
	/********************************************************************/
	
	if(SHOW_ENCODE == 1)
	{
		printf("\nTIM Encode Data:\n");
		_show_tim_standard_item(encode_item);
	}

	err_tim = j2735r63_tim_encode(encode_item, encodeBuf, input_buf_size, size, ENCODE_PRINT_LEVEL);
	if(err_tim < 0)
	{		
		printf("tim encode fail...\n");		
		return -1;	
	}		
	_cmd_tim_free_struct(encode_item);
	
	return 0;
}

int cmd_txTim(cli_instance_t *cli, const char *command, char *argv[], int argc)
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
		printf("txTim <psid> <data_rate> <tx_power> <dest_mac> <interval> <num>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("<data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
		printf("<tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
		printf("<dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
		printf("<interval> the interval of sending packets, 0 - 1000, unit:ms\n");
		printf("<num> the number of sending packets, 1 - 10000\n");
		printf("-- example --\n");
		printf("txTim 123 6 18 FF:FF:FF:FF:FF:FF 1000 2\n");
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

	encodeBuf = (unsigned char *)j2735_tim_calloc(1, MAX_TIM_BUFF_SIZE);
	_encode_txTim(encodeBuf, MAX_TIM_BUFF_SIZE, &len);
	
	memcpy(out_wsm.data, encodeBuf, len);
	out_wsm.length = len;

	j2735_tim_free(encodeBuf);

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

	printf("%s", "cmd_txTim command executed successfully.\n");
	return CLI_OK;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
