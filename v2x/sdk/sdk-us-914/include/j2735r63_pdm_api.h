#ifndef _PDM_R63_CONVERTER_H_
#define _PDM_R63_CONVERTER_H_
#define MAX_PDM_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct pdm_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
				  mask_data_elements:1,
				  mask_regional:1,
				  rest:5;
	
	unsigned char message_id;
	unsigned int  time_stamp; /*INTEGER (0..527040); invalid: 527040*/
	sample_item sample;
	unsigned short directions;/*BIT STRING (SIZE (16))*/

	unsigned char term_choice;
	unsigned short term_time;/*INTEGER (1..1800)*/
	unsigned short term_distance;/*INTEGER (1..30000)*/

	unsigned char snapshot_choice;
	snapshot_time_item snapshot_time;
	snapshot_distance_item snapshot_distance;

	unsigned char tx_interval;/*INTEGER (0..61)*/

	unsigned char data_elements_num;
	vehicle_status_request_item *data_elements;

}pdm_standard_item;

int j2735r63_pdm_encode(pdm_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_pdm_decode(pdm_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_pdm_free_struct(pdm_standard_item *free_pdm_item);

#endif
