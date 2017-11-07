#ifndef _PSM_R63_CONVERTER_H_
#define _PSM_R63_CONVERTER_H_
#define MAX_PSM_BUFF_SIZE 2048

#include "j2735r63_common.h"

typedef struct psm_standard_item_str //R63
{
  unsigned char mask_accelSet:1,
				mask_pathHistory:1,
				mask_pathPrediction:1,
				mask_propulsion:1,
				mask_useState:1,
				mask_crossRequest:1,
				mask_crossState:1,
 				mask_clusterSize:1;
  unsigned char	mask_clusterRadius:1,
				mask_eventResponderType:1,
				mask_activityType:1,
				mask_activitySubType:1,
				mask_assistType:1,
				mask_sizing:1,
				mask_attachment:1,
 				mask_attachmentRadius:1;
  unsigned char mask_animalType:1,
				mask_regional:1,
				rest:6;
	
	unsigned char message_id;
	unsigned char basicType; 	/*ENUMERATED (0..4)*/
	unsigned int  secMark;	 	/*INTEGER (0..65535)*/
	unsigned char msgCnt;   	/*INTEGER (0..127)*/
	unsigned char id[4];	 		/*OCTET STRING (SIZE(4))*/
	position3d_2_item position;
	positional_accuracy_item accuracy;
	unsigned short speed;			/*INTEGER (0..8191)*/
	unsigned short heading;		/*INTEGER (0..28800)*/
	acceleration_set_4way_item accelSet;
	path_history_item pathHistory;
	path_prediction_item pathPrediction;
	propelled_information_item propulsion;
	unsigned short useState;	/*BIT STRING (SIZE (9))*/
	char crossRequest; 				/*BOOLEAN*/
	char crossState; 					/*BOOLEAN*/
	unsigned char clusterSize; 				/*ENUMERATED (0..3)*/
	unsigned char clusterRadius;   		/*INTEGER (0..100)*/
	unsigned char eventResponderType; /*ENUMERATED (0..7)*/
	unsigned char activityType;				/*BIT STRING (SIZE (6))*/
	unsigned char activitySubType;		/*BIT STRING (SIZE (7))*/
	unsigned char assistType;					/*BIT STRING (SIZE (6))*/
	unsigned char sizing;							/*BIT STRING (SIZE (5))*/
	unsigned char attachment; 				/*ENUMERATED (0..6)*/
	unsigned char attachmentRadius;   /*INTEGER (0..200)*/
	unsigned char animalType; 				/*ENUMERATED (0..3)*/

}psm_standard_item;

int j2735r63_psm_encode(psm_standard_item *input_item, unsigned char output_buf[], size_t input_buf_size, size_t *output_buf_size, unsigned char print_level);

int j2735r63_psm_decode(psm_standard_item *return_decoded_item, unsigned char decode_buf[], size_t decode_buf_size, unsigned char print_level);
void j2735r63_psm_free_struct(psm_standard_item *free_psm_item);

#endif
