#ifndef _SSM_R63_CONVERTER_H_
#define _SSM_R63_CONVERTER_H_
#define MAX_SSM_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct ssm_standard_item_str //R63
{
	unsigned char   mask_time_stamp:1,
					mask_sequence_number:1,
					mask_regional:1,
					rest:5;
	
	unsigned char message_id;
	unsigned int  time_stamp; 	/*INTEGER (0..527040); invalid: 527040*/
	unsigned int second;				/*INTEGER (0..65535)*/
	unsigned char sequence_number; 	/*INTEGER (0..127)*/

	unsigned char status_num;
	signal_status_item *status;

}ssm_standard_item;

int j2735r63_ssm_encode(ssm_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_ssm_decode(ssm_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_ssm_free_struct(ssm_standard_item *free_ssm_item);

#endif
