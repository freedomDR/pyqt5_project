#ifndef _PVD_R63_CONVERTER_H_
#define _PVD_R63_CONVERTER_H_
#define MAX_PVD_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct pvd_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_seg_num:1,
			mask_probe_ID:1,
			mask_regional:1,
			rest:4;
	
	unsigned char message_id;
	unsigned int  time_stamp; /*INTEGER (0..527040); invalid: 527040*/
	unsigned int  seg_num; 	  /*INTEGER (0..32767)*/

	vehicle_ident_item probe_ID;
	full_position_vector_item start_vector;
	vehicle_classification_item vehicle_type;

	unsigned char snapshots_num;
	snapshots_item *snapshots;

}pvd_standard_item;

int j2735r63_pvd_encode(pvd_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_pvd_decode(pvd_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_pvd_free_struct(pvd_standard_item *free_pvd_item);

#endif
