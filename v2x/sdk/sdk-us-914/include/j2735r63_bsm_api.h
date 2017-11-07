#ifndef _BSM_R63_CONVERTER_H_
#define _BSM_R63_CONVERTER_H_
#define MAX_BSM_BUFF_SIZE 2048

#include "j2735r63_common.h"
#include <sys/types.h>	/* For size_t */

typedef struct bsm_standard_item_str //R63
{
	unsigned char mask_partII:1,
			mask_regional:1,
			rest:6;

	unsigned char message_id;

	/*(1) core_data */
	bsm_core_data core_data;

	/*(2) PartII */
	unsigned char partII_num; //SEQUENCE (SIZE(1..8))
	partII_item *partII; //OP
}bsm_standard_item;


int j2735r63_bsm_encode(bsm_standard_item *encode_item, unsigned char encode_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);
int j2735r63_bsm_decode(bsm_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_bsm_free_struct(bsm_standard_item *free_bsm_item);

#endif
