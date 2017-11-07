#ifndef _TIM_R63_CONVERTER_H_
#define _TIM_R63_CONVERTER_H_
#define MAX_TIM_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct tim_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			  mask_packet_id:1,
		      mask_url_b:1,
		      mask_regional:1,
		      rest:4;

	unsigned char message_id;
	unsigned char msg_cnt; 			/*INTEGER (0..127)*/
	unsigned int time_stamp;		/*INTEGER (0..527040); invalid: 527040*/	
	unsigned char packet_id[9];	/*OCTET STRING (size(9))*/
	unsigned char url_b[46];		/*IA5String (size(1..45)) */

	unsigned char data_frames_num;
	traveler_data_frame_item *data_frames; /*SEQUENCE (SIZE(1..8)) OF TravelerDataFrame*/

	unsigned char regional_num;
	regional_extension_item regional[4]; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-EventDescription}}*/
}tim_standard_item;


int j2735r63_tim_encode(tim_standard_item *encode_item, unsigned char encode_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);
int j2735r63_tim_decode(tim_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_tim_free_struct(tim_standard_item *free_tim_item);

#endif
