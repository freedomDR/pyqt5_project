#ifndef _RSA_R63_CONVERTER_H_
#define _RSA_R63_CONVERTER_H_
#define MAX_RSA_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct rsa_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_description:1,
			mask_priority:1,
			mask_heading:1,
			mask_extent:1,
			mask_position:1,
			mask_further_info_id:1,
			mask_regional:1;

	unsigned char message_id;
	unsigned char msg_cnt; 			/*INTEGER (0..127)*/
	unsigned int time_stamp; 		/*INTEGER (0..527040); invalid: 527040*/
	unsigned short type_event; 	/*INTEGER (0.. 65535)*/

	unsigned char description_num; 
	unsigned short *description; /*SEQUENCE (SIZE(1..8)) OF ITIS.ITIScodes, INTEGER (0.. 65535)*/

	char priority; 							/*OCTET STRING (SIZE(1))*/ 
	unsigned short heading; 		/*BIT STRING (SIZE (16))*/
	unsigned char extent; 			/*ENUMERATED (0..15)*/
	full_position_vector_item *position; 
	unsigned char further_info_id[2]; /*OCTET STRING (SIZE(2))*/
}rsa_standard_item;


int j2735r63_rsa_encode(rsa_standard_item *encode_item, unsigned char encode_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);
int j2735r63_rsa_decode(rsa_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_rsa_free_struct(rsa_standard_item *free_rsa_item);

#endif
