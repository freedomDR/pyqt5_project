#ifndef _SRM_R63_CONVERTER_H_
#define _SRM_R63_CONVERTER_H_
#define MAX_SRM_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct srm_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_sequence_number:1,
			mask_requests:1,
			mask_regional:1,
			rest:4;
	
	unsigned char message_id;
	unsigned int  time_stamp; /*INTEGER (0..527040); invalid: 527040*/
	unsigned short second;		/*INTEGER (0..65535)*/
	unsigned char sequence_number; /*INTEGER (0..127)*/

	unsigned char requests_num;
	signal_request_package_item *requests;
	requestor_description_item requestor;

}srm_standard_item;

int j2735r63_srm_encode(srm_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_srm_decode(srm_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_srm_free_struct(srm_standard_item *free_srm_item);

#endif
