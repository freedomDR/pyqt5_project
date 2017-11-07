#ifndef _RTCM_R63_CONVERTER_H_
#define _RTCM_R63_CONVERTER_H_
//#define MAX_RTCM_BUFF_SIZE 4096
#define MAX_RTCM_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct rtcm_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_anchorPoint:1,
			mask_rtcmHeader:1,
			mask_regional:1,
			rest:4;
	
	unsigned char message_id;
	unsigned char msg_cnt; 		/*INTEGER (0..127)*/
	unsigned char rev;				/*ENUMERATED (0..3)*/
	unsigned int  time_stamp; /*INTEGER (0..527040); invalid: 527040*/
	full_position_vector_item *anchorPoint; //OP
	rtcm_header_item *rtcmHeader;

	unsigned char msgs_num;
	rtcm_message_item *msgs;

}rtcm_standard_item;

int j2735r63_rtcm_encode(rtcm_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_rtcm_decode(rtcm_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_rtcm_free_struct(rtcm_standard_item *free_rtcm_item);

#endif
