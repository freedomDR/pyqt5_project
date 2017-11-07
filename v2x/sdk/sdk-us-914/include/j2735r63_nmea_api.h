#ifndef _NMEA_R63_CONVERTER_H_
#define _NMEA_R63_CONVERTER_H_
#define MAX_NMEA_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct nmea_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_rev:1,
			mask_msg:1,
			mask_wdCount:1,
			mask_regional:1,
			rest:3;
	
	unsigned char message_id;
	unsigned int  time_stamp; 	/*INTEGER (0..527040); invalid: 527040*/
	unsigned char rev; 					/*ENUMERATED (0..6)*/
	unsigned short msg;					/*INTEGER (0..32767)*/
	unsigned short wdCount;			/*INTEGER (0..1023)*/
	unsigned char payload[1023];/*OCTET STRING (SIZE(1023))*/

}nmea_standard_item;

int j2735r63_nmea_encode(nmea_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_nmea_decode(nmea_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_nmea_free_struct(nmea_standard_item *free_nmea_item);

#endif
