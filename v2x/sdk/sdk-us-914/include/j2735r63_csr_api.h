#ifndef _CSR_R63_CONVERTER_H_
#define _CSR_R63_CONVERTER_H_
#define MAX_CSR_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct csr_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_msg_cnt:1,
			mask_id:1,
			mask_regional:1,
			rest:4;
	
	unsigned char message_id;
	unsigned int  time_stamp; /*INTEGER (0..527040); invalid: 527040*/
	unsigned char msg_cnt; 		/*INTEGER (0..127)*/
	unsigned char id[4];			/*OCTET STRING (SIZE(4))*/

	unsigned char requests_num;
	requested_item *requests;

}csr_standard_item;

int j2735r63_csr_encode(csr_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_csr_decode(csr_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_csr_free_struct(csr_standard_item *free_csr_item);

#endif
