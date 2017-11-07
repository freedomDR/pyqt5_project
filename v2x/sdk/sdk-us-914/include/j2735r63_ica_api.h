#ifndef _ICA_R63_CONVERTER_H_
#define _ICA_R63_CONVERTER_H_
#define MAX_ICA_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct ica_standard_item_str //R63
{
	unsigned char mask_time_stamp:1,
			mask_partOne:1,
			mask_path:1,
			mask_pathPrediction:1,
			mask_regional:1,
			rest:3;
	
	unsigned char message_id;
	unsigned char msg_cnt; 		/*INTEGER (0..127)*/
	unsigned char id[4];			/*OCTET STRING (SIZE(4))*/
	unsigned int  time_stamp; /*INTEGER (0..527040); invalid: 527040*/
	bsm_core_data part_one;			/*optional*/
	path_history_item *path;		/*optional*/

	path_prediction_item path_prediction; 	/*optional*/
	intersection_reference_id_item intersectionID;	
	approach_or_lane_item laneNumber;
	unsigned short eventFlag;/*BIT STRING (SIZE (13))*/
}ica_standard_item;

int j2735r63_ica_encode(ica_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_ica_decode(ica_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_ica_free_struct(ica_standard_item *free_ica_item);

#endif

