#ifndef _SPAT_R63_CONVERTER_H_
#define _SPAT_R63_CONVERTER_H_
#define MAX_SPAT_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct spat_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_name:1,
			mask_regional:1,
			rest:5;

	unsigned char message_id;
	unsigned int time_stamp; 	/*INTEGER (0..527040); invalid: 527040*/	
	unsigned char name[64]; 	/*IA5String (size(1..63))*/

	unsigned char intersections_num;
	intersection_state_item *intersections; //sequence (size(1..32)) of  intersectionstate
}spat_standard_item;


int j2735r63_spat_encode(spat_standard_item *encode_item, unsigned char encode_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);
int j2735r63_spat_decode(spat_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_spat_free_struct(spat_standard_item *free_spat_item);

#endif
