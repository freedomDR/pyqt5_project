#ifndef _MAP_R63_CONVERTER_H_
#define _MAP_R63_CONVERTER_H_
#define MAX_MAP_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct map_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
				mask_layer_type:1,
				mask_layer_id:1,
				mask_intersections:1,
				mask_road_segments:1,
				mask_data_parameters:1,
				mask_restriction_list:1,
				mask_regional:1;

	unsigned char message_id;	
	unsigned int time_stamp; 		/*INTEGER (0..527040); invalid: 527040*/
	unsigned char msg_issue_revision; /*INTEGER (0..127)*/
	unsigned char layer_type; 	/*ENUMERATED (0..7)*/
	unsigned char layer_id; 		/*INTEGER (0..100)*/

	unsigned char intersections_num; //SEQUENCE (SIZE(1..32))
	intersection_geometry_item *intersections; //OP

	unsigned char road_segments_num; //SEQUENCE (SIZE(1..32))
	road_segment_item *road_segments; //OP

	data_parameters_item data_parameters; //OP

	unsigned char restriction_list_num; //SEQUENCE (SIZE(1..254))
	restriction_class_assignment_item *restriction_list; //OP

	unsigned char regional_num; //SEQUENCE (SIZE(1..4))
	reg_map_data_item *regional; //OP
}map_standard_item;


int j2735r63_map_encode(map_standard_item *encode_item, unsigned char encode_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);
int j2735r63_map_decode(map_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_map_free_struct(map_standard_item *free_map_item);

#endif
