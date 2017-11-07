#ifndef _EVA_R63_CONVERTER_H_
#define _EVA_R63_CONVERTER_H_
#define MAX_EVA_BUFF_SIZE 2048

#include "j2735r63_common.h"
#include "j2735r63_rsa_api.h"


typedef struct eva_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_id:1,
			mask_responseType:1,
			mask_details:1,
			mask_mass:1,
			mask_basicType:1,
			mask_vehicleType:1,
			mask_responseEquip:1;
	unsigned char mask_responderType:1,
			mask_regional:1,
			rest:6;
	
	unsigned char message_id;
	unsigned int  time_stamp; 		/*INTEGER (0..527040); invalid: 527040*/
	unsigned char id[4];					/*OCTET STRING (SIZE(4))*/
	rsa_standard_item *rsaMsg;
	unsigned char responseType;		/*ENUMERATED (0..6)*/
	emergency_details_item details;
	unsigned char mass;						/*INTEGER (0..255); unavailable: 255*/
	unsigned char basicType; 			/*ENUMERATED (0..15)*/
	unsigned short vehicleType;		/*ENUMERATED (9217..9251)*/
	unsigned short responseEquip;	/*ENUMERATED (9985..10114)*/
	unsigned short responderType;	/*ENUMERATED (9729..9742)*/

}eva_standard_item;

int j2735r63_eva_encode(eva_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_eva_decode(eva_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_eva_free_struct(eva_standard_item *free_eva_item);

#endif

