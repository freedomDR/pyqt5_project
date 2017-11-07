#ifndef	__SAE_COMMON_H_ 
#define	__SAE_COMMON_H_ 

/*Encode and Decode printf_level*/
#define PRINT_NON 0
#define PRINT_BASIC 1
#define PRINT_COMPLETE 2

/*DE_DSRC_MessageID, UPER forms*/
#define MAP_ID 18
#define SPAT_ID 19
#define BSM_ID 20
#define CSR_ID 21
#define EVA_ID 22
#define ICA_ID 23
#define NMEA_ID 24
#define PDM_ID 25
#define PVD_ID 26
#define RSA_ID 27
#define RTCM_ID 28
#define SRM_ID 29
#define SSM_ID 30
#define TIM_ID 31
#define PSM_ID 32

#define MIN_MSGCNT 0
#define MAX_MSGCNT 127

#define TRUE 1

enum REGION_ID //R63
{
	NO_REGION,
	ADD_GRP_A,
	ADD_GRP_B,
	ADD_GRP_C
};

enum PARTII_ID //R63
{
	VEHICLE_SAFETY_EXT,
	SPECIAL_VEHICLE_EXT,
	SUPPLEMENTAL_VEHICLE_EXT
};

enum LANE_TYPE_ATTRIBUTES_ENUM //R63
{
	LANE_TYPE_ATTRIBUTES_NOTHING,
	LANE_TYPE_ATTRIBUTES_VEHICLE,
	LANE_TYPE_ATTRIBUTES_CROSSWALK,
	LANE_TYPE_ATTRIBUTES_BIKELANE,
	LANE_TYPE_ATTRIBUTES_SIDEWALK,
	LANE_TYPE_ATTRIBUTES_MEDIAN,
	LANE_TYPE_ATTRIBUTES_STRIPING,
	LANE_TYPE_ATTRIBUTES_TRACKEDVEHICLE,
	LANE_TYPE_ATTRIBUTES_PARKING	
};

enum NODE_OFFSET_POINT_XY_ENUM //R63
{
	NODE_OFFSET_POINTXY_NOTHING,
	NODE_OFFSET_POINTXY_XY1,
	NODE_OFFSET_POINTXY_XY2,
	NODE_OFFSET_POINTXY_XY3,
	NODE_OFFSET_POINTXY_XY4,
	NODE_OFFSET_POINTXY_XY5,
	NODE_OFFSET_POINTXY_XY6,
	NODE_OFFSET_POINTXY_LATLON,
	NODE_OFFSET_POINTXY_REGIONAL	
};

enum NODE_OFFSET_POINT_LL_ENUM //R63
{
	NODE_OFFSET_POINTLL_NOTHING,
	NODE_OFFSET_POINTLL_LL1,
	NODE_OFFSET_POINTLL_LL2,
	NODE_OFFSET_POINTLL_LL3,
	NODE_OFFSET_POINTLL_LL4,
	NODE_OFFSET_POINTLL_LL5,
	NODE_OFFSET_POINTLL_LL6,
	NODE_OFFSET_POINTLL_LATLON,
	NODE_OFFSET_POINTLL_REGIONAL	
};

enum LANE_DATA_ATTRIBUTE_ENUM //R63
{
	LANE_DATA_ATTRIBUTE_NOTHING,
	LANE_DATA_ATTRIBUTE_PATHENDPOINTANGLE,
	LANE_DATA_ATTRIBUTE_LANECROWNPOINTCENTER,
	LANE_DATA_ATTRIBUTE_LANECROWNPOINTLEFT,
	LANE_DATA_ATTRIBUTE_LANECROWNPOINTRIGHT,
	LANE_DATA_ATTRIBUTE_LANEANGLE,
	LANE_DATA_ATTRIBUTE_SPEEDLIMITS,
	LANE_DATA_ATTRIBUTE_REGIONAL	
};

enum OFFSET_XAXIS_ENUM //R63
{
	OFFSET_XAXIS_NOTHING,
	OFFSET_XAXIS_SMALL,
	OFFSET_XAXIS_LARGE
};

enum OFFSET_YAXIS_ENUM //R63
{
	OFFSET_YAXIS_NOTHING,
	OFFSET_YAXIS_SMALL,
	OFFSET_YAXIS_LARGE	
};

enum NODE_LISTXY_ENUM //R63
{
	NODE_LISTXY_NOTHING,
	NODE_LISTXY_NODES,
	NODE_LISTXY_COMPUTED	
};

enum NODE_LISTLL_ENUM //R63
{
	NODE_LISTLL_NOTHING,
	NODE_LISTLL_NODES	
};

enum RESTRICTION_USER_TYPE_ENUM //R63
{
	RESTRICTION_USER_TYPE_NOTHING,
	RESTRICTION_USER_TYPE_BASICTYPE,
	RESTRICTION_USER_TYPE_REGIONAL	
};

enum MSG_ID_ENUM //R63
{
	MSG_ID_NOTHING,
	MSG_ID_FURTHER_INFO_ID,
	MSG_ID_ROAD_SIGN_ID	
};

enum VALID_REGION__AREA_ENUM //R63
{
	VALID_REGION__AREA_NOTHING,
	VALID_REGION__AREA_SHAPE_POINT_SET,
	VALID_REGION__AREA_CIRCLE,
	VALID_REGION__AREA_REGION_POINT_SET	
};

enum CONTENT_ENUM //R63
{
	CONTENT_NOTHING,
	CONTENT_ADVISORY,
	CONTENT_WORKZONE,
	CONTENT_GENERICSIGN,
	CONTENT_SPEEDLIMIT,
	CONTENT_EXITSERVICE	
};

enum ITIS_ITEM_ENUM //R63
{
	ITIS_ITEM_NOTHING,
	ITIS_ITEM_ITIS,
	ITIS_ITEM_TEXT	
};

enum CONTENTITEM_ITEM_ENUM //R63
{
	CONTENTITEM_ITEM_NOTHING,
	CONTENTITEM_ITEM_ITIS,
	CONTENTITEM_ITEM_TEXT	
};

enum GEOGRAPHICALPATH__DESCRIPTION_ENUM //R63
{
	GEOGRAPHICALPATH__DESCRIPTION_NOTHING,
	GEOGRAPHICALPATH__DESCRIPTION_PATH,
	GEOGRAPHICALPATH__DESCRIPTION_GEOMETRY,
	GEOGRAPHICALPATH__DESCRIPTION_OLDREGION	
};

enum OFFSETSYSTEM__OFFSET_ENUM //R63
{
	OFFSETSYSTEM__OFFSET_NOTHING,
	OFFSETSYSTEM__OFFSET_XY,
	OFFSETSYSTEM__OFFSET_LL	
};

enum COMPUTEDLANE__OFFSETXAXIS_ENUM //R63
{
	COMPUTEDLANE__OFFSETXAXIS_NOTHING,
	COMPUTEDLANE__OFFSETXAXIS_SMALL,
	COMPUTEDLANE__OFFSETXAXIS_LARGE	
};

enum COMPUTEDLANE__OFFSETYAXIS_ENUM //R63
{
	COMPUTEDLANE__OFFSETYAXIS_NOTHING,
	COMPUTEDLANE__OFFSETYAXIS_SMALL,
	COMPUTEDLANE__OFFSETYAXIS_LARGE	
};

enum APPROACHORLANE_ENUM //R63
{
	APPROACHORLANE_NOTHING,
	APPROACHORLANE_APPROACH,
	APPROACHORLANE_LANE	
};

enum INTERSECTION_ACCESS_POINT_ENUM //R63
{
	INTERSECTION_ACCESS_POINT_NOTHING,
	INTERSECTION_ACCESS_POINT_LANE,	
	INTERSECTION_ACCESS_POINT_APPROACH,
	INTERSECTION_ACCESS_POINT_CONNECTION
};

enum PDM_TERM_ENUM 
{
	PDM_TERM_NOTHING,
	PDM_TERM_TERM_TIME,
	PDM_TERM_TERM_DISTANCE
};

enum PDM_SNAPSHOT_ENUM 
{
	PDM_SNAPSHOT_NOTHING,
	PDM_SNAPSHOT_SNAPSHOT_TIME,
	PDM_SNAPSHOT_SNAPSHOT_DISTANCE
};

enum VEHICLE_ID_ENUM 
{
	VEHICLE_ID_NOTHING,
	VEHICLE_ID_ENTITY_ID,
	VEHICLE_ID_STATION_ID
};

enum VEHICLE_CLASS_ENUM 
{
	VEHICLE_CLASS_NOTHING,
	VEHICLE_CLASS_VGROUP,
	VEHICLE_CLASS_RGROUP,
	VEHICLE_CLASS_REQUIP
};

enum PROPELLED_INFORMATION_ENUM 
{
	PROPELLED_INFORMATION_NOTHING,
	PROPELLED_INFORMATION_HUMAN,
	PROPELLED_INFORMATION_ANIMAL,
	PROPELLED_INFORMATION_MOTOR
};

typedef struct regional_extension_str //R63
{
	unsigned char region_id;  /*INTEGER (0..255)*/
	char reg_ext_value;				/*OCTET STRING (SIZE(1))*/
}regional_extension_item;

typedef struct reg_lane_data_attribute_str //R63
{
	unsigned char region_id;  //ADD_GRP_C
}reg_lane_data_attribute_item;

typedef struct vehicle_id_str //R63
{
	unsigned char vehicle_id_choice;
	unsigned char entity_id[4];	/*OCTET STRING (SIZE(4))*/
	unsigned long station_id;		/*INTEGER(0..4294967295)*/
}vehicle_id_item;

typedef struct vehicle_ident_str
{
	unsigned char mask_name:1,
			 mask_vin:1,
			 mask_owner_code:1,
			 mask_id:1,
			 mask_vehicle_type:1,
			 mask_vehicle_class:1,
			 rest:2;

	char name[64];			/*OP; IA5String (SIZE(1..63))*/
	char vin[18];				/*OP; OCTET STRING (SIZE(1..17))*/
	char owner_code[33];/*OP; IA5String(SIZE(1..32))*/
	vehicle_id_item id; /*OP*/
	unsigned char vehicle_type; /*OP; ENUMERATED (0..15)*/
	unsigned char vehicle_class_choice;
	unsigned int vehicle_class;	
}vehicle_ident_item;

typedef struct ddate_time_str //R63
{
	unsigned char mask_year:1,
			mask_month:1,
			mask_day:1,
			mask_hour:1,
			mask_minute:1,
			mask_second:1,
			mask_offset:1,
			reserved:1;

	unsigned short year;	/*OP; INTEGER (0..4095); LSB: years; unknown: 0*/
	unsigned char month;	/*OP; INTEGER (0..12); LSB: months; unknown: 0*/
	unsigned char day;		/*OP; INTEGER (0..31); LSB: days; unknown: 0*/
	unsigned char hour; 	/*OP; INTEGER (0..31); LSB: hours; unknown: 31*/
	unsigned char minute;	/*OP; INTEGER (0..60); LSB: minutes; unknown: 60*/
	unsigned short second;/*OP; INTEGER (0..65535); LSB: milliseconds; unknown: 65535*/
	short offset; 				/*OP; INTEGER (-840..840); LSB: minutes; unknown: 0*/
}ddate_time_item; 

typedef struct speed_heading_throttle_confidence_str //R63
{
	unsigned char heading;	/*ENUMERATED(0..7); unavailable: 0*/
	unsigned char speed; 		/*ENUMERATED(0..7); unavailable: 0*/
	unsigned char throttle; /*ENUMERATED(0..3); unavailable: 0*/
}speed_heading_throttle_confidence_item;

typedef struct position_confidence_set_str //R63
{
	unsigned char pos; 			/*ENUMERATED (0..15); unavailable: 0*/
	unsigned char elevation;/*ENUMERATED (0..15); unavailable: 0*/
}position_confidence_set_item;

typedef struct transmission_speed_str //R63
{
	unsigned char transmission_state; /*ENUMERATED (0..7)*/
	unsigned short speed; /*INTEGER (0..8191); LSB: 0.02 m/s; unavailable: 8191*/
}transmission_speed_item;

typedef struct positional_accuracy_str //R63
{
	unsigned char semi_major;		/*RANGE(0..255); LSB = 0.05m; unavailable: 255*/
	unsigned char semi_minor; 	/*RANGE(0..255); LSB = 0.05m; unavailable: 255*/
	unsigned short orientation; /*RANGE(0..65535); LSB = 0.0054932479 degrees; unavailable: 65535*/
}positional_accuracy_item;

typedef struct vehicle_size_str //R63
{
	unsigned short width; /*INTEGER (0..1023); LSB: 1 cm; unavailable: 0*/
	unsigned short length;/*INTEGER (0..4095); LSB: 1 cm; unavailable: 0*/
}vehicle_size_item;

typedef struct wipers_str
{
	unsigned char mask_status_rear:1,
			mask_rate_rear:1,
			rest:6;

	unsigned char status_front; /*ENUMERATED(0...6)*/
	unsigned char rate_front;		/*INTEGER (0..127)*/
	unsigned char status_rear; 	/*OP; ENUMERATED (0...6)*/
	unsigned char rate_rear; 		/*OP; INTEGER (0..127)*/
}wipers_item;

typedef struct brake_system_status_str //R63
{
	unsigned char wheel_brakes;	/*BIT STRING SIZE(5); RANGE(0..31)*/
	unsigned char traction;			/*ENUMERATED (0..3)*/
	unsigned char abs; 					/*ENUMERATED (0..3)*/
	unsigned char scs; 					/*ENUMERATED (0..3)*/
	unsigned char brake_boost;	/*ENUMERATED (0..2)*/
	unsigned char aux_brakes;		/*ENUMERATED (0..3)*/
}brake_system_status_item;

typedef struct steering_str
{
	unsigned char mask_confidence:1,
			mask_rate:1,
			mask_wheels:1,
			rest:5;
	
	signed char angle;	/*RANGE(-126..126); LSB = 1.5 degrees; unavailable: 127*/
	unsigned char confidence;/*OP; ENUMERATED (0...3)*/
	signed char rate;		/*OP; INTEGER (-127..127); LSB: 3 degrees per second*/
	signed char wheels; /*OP; INTEGER (-128..127); LSB: 0.3333 degrees*/
}steering_item;

typedef struct acceleration_set_4way_str //R63
{
	short longitude;	/*INTEGER (-2000..2001); LSB: 0.01 m/s^2; unavailable: 2001*/
	short lat;				/*INTEGER (-2000..2001); LSB: 0.01 m/s^2; unavailable: 2001*/
	signed char vert; /*INTEGER (-127..127); LSB: 0.02 G; unavailable: -127*/
	short yaw;				/*INTEGER (-32767..32767); LSB: 0.01 degrees per second*/
}acceleration_set_4way_item;

typedef struct accel_steer_yaw_rate_confidence_str
{
	unsigned char yaw_rate;							/*ENUMERATED (0...7)*/
	unsigned char acceleration; 				/*ENUMERATED (0...7)*/
	unsigned char steering_wheel_angle; /*ENUMERATED (0...3)*/
}accel_steer_yaw_rate_confidence_item;

typedef struct confidence_set_str
{
	unsigned char mask_accel_confidence :1,
			mask_speed_confidence:1,
			mask_time_confidence:1,
			mask_pos_confidence:1,
			mask_steer_confidence:1,
			mask_heading_confidence:1,
			mask_throttle_confidence:1,
			rest:1;

	accel_steer_yaw_rate_confidence_item accel_confidence;
	speed_heading_throttle_confidence_item speed_confidence;
	unsigned char time_confidence;		/*ENUMERATED (0...39)*/
	position_confidence_set_item pos_confidence;
	unsigned char steer_confidence;		/*ENUMERATED (0...3)*/
	unsigned char heading_confidence; /*ENUMERATED (0...7)*/
	unsigned char throttle_confidence;/*ENUMERATED (0...3)*/
}confidence_set_item;

typedef struct accel_sets_str
{
	unsigned char mask_accel_4way:1,
			mask_vert_accel_thres:1,
			mask_yaw_rate_con:1,
			mask_hoz_accel_con:1,
			mask_confidence_set:1,
			rest:3;

	acceleration_set_4way_item accel_4way;/*OP*/
	unsigned char vert_accel_thres; /*BIT STRING SIZE (4)*/
	unsigned char yaw_rate_con;			/*ENUMERATED (0...7)*/
	unsigned char hoz_accel_con;		/*ENUMERATED (0...7)*/
	confidence_set_item confidence_set;
}accel_sets_item;

typedef struct wiper_set_str //R63
{
	unsigned char mask_status_rear :1,
			mask_rate_rear :1,
			rest:6;

	unsigned char status_front;	/*ENUMERATED {0...6}; unavailable: 0*/
	unsigned char rate_front; 	/*INTEGER (0..127); LSB: 1 minute*/
	unsigned char status_rear;	/*OP; ENUMERATED {0...6}; unavailable: 0*/
	unsigned char rate_rear;		/*OP; INTEGER (0..127); LSB: 1 minute*/
} wiper_set_item;

typedef struct weather_report_str //R63
{
	unsigned char mask_rain_rate :1,
			mask_precip_situation :1,
			mask_solar_radiation :1,
			mask_friction :1,
			mask_road_friction :1,			
			rest:3;

	unsigned char is_raining; //ENUMERATED {precip (1), noPrecip (2), error (3)}
	unsigned short rain_rate; //OP; INTEGER (0..65535)
	unsigned char precip_situation; //OP; ENUMERATED {1...15}; unknown: 2
	unsigned short solar_radiation; //OP; INTEGER (0..65535)
	unsigned char friction; //OP; INTEGER (0..101)
	unsigned char road_friction; //OP; INTEGER (0..50); LSB: 0.02 micro; unavailable: 0
}weather_report_item;

typedef struct pvd_weather_report_str //R63
{
	unsigned char mask_rain_rate :1,
			mask_precip_situation :1,
			mask_solar_radiation :1,
			mask_friction :1,
			rest:4;

	unsigned char is_raining; //ENUMERATED {precip (1), noPrecip (2), error (3)}
	unsigned short rain_rate; //OP; INTEGER (0..65535)
	unsigned char precip_situation; //OP; ENUMERATED {1...15}
	unsigned short solar_radiation; //OP; INTEGER (0..65535)
	unsigned char friction; //OP; INTEGER (0..101)

}pvd_weather_report_item;

typedef struct weather_probe_str //R63
{
	unsigned char mask_air_temp :1,
			mask_air_pressure :1,
			mask_rain_rates :1,
			rest:5;

	unsigned char air_temp; //OP; INTEGER (0..191); unknown: 191
	unsigned char air_pressure; //OP; INTEGER (0..255)
	wiper_set_item rain_rates; //OP
} weather_probe_item;

typedef struct obstacle_detection_str //R63
{
	unsigned char mask_description :1,
			mask_location_details :1,
			mask_vert_event :1,
			rest:5;

	unsigned short ob_dist; //INTEGER (0..32767); LSB: 1 meters
	unsigned short ob_direct; //INTEGER (0..28800); LSB: 0.0125 degrees; unavailable: 28800
	unsigned short description; //OP; INTEGER (0.. 65535)
	unsigned short location_details; //OP; ENUMERATED {7936 to 8191}
	ddate_time_item date_time;
	char vert_event; //OP; BIT STRING 5 bits; RANGE(0..31)
} obstacle_detection_item;

typedef struct disabled_vehicle_str //R63
{
	unsigned char mask_location_details :1,
			rest:7;

	unsigned short status_details; //INTEGER (0.. 65535)
	unsigned short location_details; //OP; ENUMERATED {7936 to 8191}
} disabled_vehicle_item;

typedef struct tires_str{
	unsigned char mask_location:1,
			mask_pressure:1,
			mask_temp:1,
			mask_wheel_sensor_status:1,
			mask_wheel_end_elect_fault:1,
			mask_leakage_rate:1,
			mask_detection:1,
			rest:1;

	unsigned char location;	/*optional, integer (0..255)*/
	unsigned char pressure;	/*optional, integer 0..250)*/
	signed long temp;	/*optional, integer (-8736..55519)*/
	unsigned char wheel_sensor_status;	/*optional, enum (0...3)*/
	unsigned char wheel_end_elect_fault;	/*optional, enum (0...3)*/
	unsigned short leakage_rate;	/*optional, integer (0..64255)*/
	unsigned char detection;	/*optional, enum (0...7)*/
}tires_item;

typedef struct axle_str
{
	unsigned char mask_location:1,
			mask_weight:1,
			rest:6;

	unsigned char location;	/*optional, integer (0..255)*/
	unsigned short weight;	/*optional, integer (0..64255)*/	
}axle_item;

typedef struct j1939_data_str
{
	unsigned char 	mask_trailer_weight:1,
			mask_cargo_weight:1,
			mask_steering_axle_temperature:1,
			mask_drive_axle_location:1,
			mask_drive_axle_lift_air_pressure:1,
			mask_drive_axle_temperature:1,			
			mask_drive_axle_lube_pressure:1,
			mask_steering_axle_lube_pressure:1;

	unsigned char mask_tires:1,
			mask_axle:1,
			rest:6;

	unsigned char tires_num; //sequence (size(0..16))
	tires_item *tires; //OP
	
	unsigned char axle_num; //sequence (size(0..16))
	axle_item *axle; //OP
	
	unsigned short trailer_weight;	/*optional, integer (0..64255)*/
	unsigned short cargo_weight;	/*optional, integer (0..64255)*/
	signed short steering_axle_temperature;	/*optional, integer (-40..210)*/
	unsigned char drive_axle_location;	/*optional, integer (0..255)*/
	unsigned short drive_axle_lift_air_pressure;	/*optional, integer (0..1000)*/
	short drive_axle_temperature;	/*optional, integer (-40..210)*/
	unsigned char drive_axle_lube_pressure;	/*optional, integer (0..250)*/
	unsigned char steering_axle_lube_pressure;	/*optional, integer (0..250)*/
}j1939_data_item;

typedef struct bumper_heights_str //R63
{
	unsigned char front; //INTEGER (0..127); LSB: 0.01 meters
	unsigned char rear;	 //INTEGER (0..127); LSB: 0.01 meters
}bumper_heights_item;

typedef struct vehicle_data_str //R63
{
	unsigned char mask_height:1,
			mask_bumpers:1,
			mask_mass:1,
			mask_trailer_weight:1,
			rest:4;

	unsigned char height; //OP; INTEGER (0..127); LSB: 5 cm
	bumper_heights_item bumpers; //OP
	unsigned char mass; //OP; INTEGER (0..255); unavailable: 255
	unsigned short trailer_weight; //OP; INTEGER (0..64255)
}vehicle_data_item;

typedef struct pvd_vehicle_data_str //R63
{
	unsigned char height; /*op, integer (0..127)*/
	bumper_heights_item bumpers; //op
	unsigned char mass; /*op, INTEGER (0..255)*/
	unsigned short trailer_weight;/*op, INTEGER (0..64255)*/
	unsigned char type;/*ENUMERATED(0..15)*/
}pvd_vehicle_data_item;

typedef struct object_str
{
	unsigned int ob_dist;/*INTEGER (0..32767)*/
	unsigned int ob_direct;/* INTEGER (0..28800) */
	ddate_time_item date_time;
}object_item;

typedef struct full_position_vector_str //R63
{
	unsigned char mask_utc_time:1,
			  mask_elevation:1,
			  mask_heading:1,
			  mask_speed:1,
			  mask_pos_accuracy:1,
			  mask_time_confidence:1,
			  mask_pos_confidence:1,
			  mask_speed_confidence:1;

	ddate_time_item *utc_time; //OP
	long longitude; //INTEGER (-1799999999..1800000001); LSB: 1/10 micro degree; unavailable: 1800000001
	long lat; //INTEGER (-900000000..900000001); LSB: 1/10 micro degree; unavailable: 900000001
	int elevation; //OP; RANG(-4096..61439); LSB: 10 cm; unknown: -4096
	unsigned short heading; //OP; INTEGER (0..28800); LSB = 0.0125 degrees; unavailable: 28800
	transmission_speed_item speed; //OP
	positional_accuracy_item pos_accuracy; //OP
	unsigned char time_confidence; //OP; ENUMERATED {0..39}
	position_confidence_set_item pos_confidence; //OP
	speed_heading_throttle_confidence_item speed_confidence; //OP
}full_position_vector_item;

typedef struct vehicle_status_str
{
	unsigned char mask_lights :1,
			mask_light_bar:1,
			mask_wipers:1,
			mask_brake_status:1,
			mask_brake_pressure:1,
			mask_road_friction:1,
			mask_sun_data:1,
			mask_rain_data:1;

	unsigned char mask_air_temp:1,
			mask_air_pres:1,
			mask_steering:1,
			mask_accel_sets:1,
			mask_object:1,
			mask_full_pos:1,
			mask_throttle_pos:1,
			mask_speed_head_c:1;

	unsigned char mask_speed_c:1,
			mask_vehicle_data:1,
			mask_vehicle_ident:1,
			mask_j1939_data:1,
			mask_weather_report:1,
			mask_gps_status:1,
			rest:2;

	unsigned short lights;	/*optional, integer (0..256) */	
	unsigned char light_bar;	/*optional, enum (0...7)*/	
	wipers_item wipers;	/*optional, sequence {
						 statusfront    wiperstatusfront,
						 ratefront      wiperrate,
						 statusrear     wiperstatusrear       optional,
						 raterear       wiperrate             optional
						 } optional */
	brake_system_status_item brake_status;	/*optional, octet string (size(2))*/
	unsigned char brake_pressure;	/*optional, enum (0...15)*/
	unsigned char road_friction;	/*optional, integer (0..50)*/
	unsigned short sun_data;	/*optional, integer (0..1000)*/
	unsigned char rain_data;	/*optional, enum (0...7)*/
	unsigned char air_temp;	/*optional, integer (0..191)*/
	unsigned char air_pres;	/*optional, integer (0..255)*/
	steering_item steering; //OP
	accel_sets_item *accel_sets;	/*optional, sequence {
							 accel4way       accelerationset4way           optional, 
							 vertaccelthres  verticalaccelerationthreshold optional, 
																		   -- wheel exceeded point
							 yawratecon      yawrateconfidence             optional, 
																			-- yaw rate confidence
							 hozaccelcon     accelerationconfidence        optional,     
																			-- acceleration confidence 
							 confidenceset   confidenceset                 optional
																		   -- general confidenceset 
							 } optional, 
							*/
	object_item *object;	/*optional, sequence {
						 obdist          obstacledistance,          -- obstacle distance        
						 obdirect        obstacledirection,         -- obstacle direction        
						 datetime        ddatetime                  -- time detected
						 } optional */	
	
	full_position_vector_item *full_pos;	/*optional */	
	unsigned char throttle_pos; /*optional, integer (0..200)*/
	
	speed_heading_throttle_confidence_item speed_head_c;
	unsigned char speed_c;	/*optional, enum (0...7)*/
	pvd_vehicle_data_item *vehicle_data;	/*optional*/
	vehicle_ident_item *vehicle_ident; //optional
	
	j1939_data_item *j1939_data; //optional
	pvd_weather_report_item *weather_report;//optional
	unsigned char gps_status;	/*optional, bit string,(size(1)) */
}vehicle_status_item;

typedef struct antenna_offset_set_str //R63
{
	short ant_offset_x; //INTEGER (-2048..2047); LSB: 1 cm; unknown: -2048
	short ant_offset_y; //INTEGER (-256..255); LSB: 1 cm; unknown: -256
	short ant_offset_z; //INTEGER (-512..511); LSB: 1 cm; unknown: -512
}antenna_offset_set_item;

typedef struct rtcm_header_str //R63
{
	unsigned char status; //BIT STRING 8 bits; RANGE(0..255)
	antenna_offset_set_item offset_set;
}rtcm_header_item;

typedef struct rtcm_message_str //R63
{
	char rtcm_message[1023]; //OCTET STRING (SIZE(1..1023))
} rtcm_message_item;

typedef struct requested_str //R63
{
	unsigned char requestedItem; //ENUMERATED
} requested_item;


typedef struct rtcm_package_str //R63
{
	unsigned char mask_rtcm_header:1,
                  reserved:7;

	rtcm_header_item rtcm_header; //OP 
	unsigned char msgs_num; //SEQUENCE (SIZE(1..5))
	rtcm_message_item *msgs;
}rtcm_package_item;

typedef struct path_prediction_str //R63
{
	int radius_of_curve; //INTEGER (-32767..32767); LSB: 10 cm
	unsigned char confidence; //INTEGER (0..200); LSB: 0.5 percent
}path_prediction_item;

typedef struct path_history_point_type_01_str
{
	unsigned char mask_elevation_offset:1,
                  mask_time_offset:1,
                  mask_pos_accuracy:1,
                  mask_heading:1,
                  mask_speed:1,
                  reserved:3;

 	int lat_offset;	/* integer (-131072..131071), 
        	             * -- in 1/10th micro degrees 
	                     * -- value  131071 to be used for  131071 or greater
        	             * -- value -131071 to be used for -131071 or less
        	             * -- value -131072 to be used for unavailable lat or long */
 	int long_offset;	/* integer (-131072..131071), 
	                     * -- in 1/10th micro degrees 
        	             * -- value  131071 to be used for  131071 or greater
        	             * -- value -131071 to be used for -131071 or less
        	             * -- value -131072 to be used for unavailable lat or long */
 	short elevation_offset;	/*optional, integer (-2048..2047)
		  		     * -- lsb units of of 10 cm
	                             * -- value  2047 to be used for  2047 or greater
        	                     * -- value -2047 to be used for -2047 or greater
        	                     * -- value -2048 to be unavailable */
	unsigned short time_offset; /*optional, integer (1..65535)
					* -- lsb units of of 10 msec  
        	                        * -- value  65534 to be used for 65534 or greater
        	                        * -- value  65535 to be unavailable */
	positional_accuracy_item *pos_accuracy; /* optional,  octet string */ 							
	signed char heading; /*optional, integer (-128..127)
                  	  * -- where the lsb is in 
                  	  * -- units of 1.5 degrees 
                  	  * -- value -128 for unavailable
                  	  * -- not an offset value */
	transmission_speed_item *speed; /*optional, octet string */
}path_history_point_type_01_item;

typedef struct path_history_point_type_02_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	short elevation_offset; /* integer (-2048...2047) */
	signed char heading; /* integer (-128...127) */
	unsigned short time_offset; /* integer (1...65535) */
	transmission_speed_item speed;
	positional_accuracy_item pos_accuracy;
}path_history_point_type_02_item;

typedef struct path_history_point_type_03_str
{
	int lat_offset; /*integer (-131072...131071) */
	int long_offset; /*integer (-131072...131071) */
	short elevation_offset; /*integer (-2048...2047) */
	unsigned short time_offset; /*integer (0...65535) */
	positional_accuracy_item pos_accuracy;
}path_history_point_type_03_item;

typedef struct path_history_point_type_04_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	short elevation_offset; /* integer (-2048...2047) */
	unsigned short time_offset; /* integer (0...65535) */
}path_history_point_type_04_item;

typedef struct path_history_point_type_05_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	short elevation_offset; /* integer (-2048...2047) */ 
	positional_accuracy_item pos_accuracy;
}path_history_point_type_05_item;

typedef struct path_history_point_type_06_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	short elevation_offset; /* integer (-2048...2047) */
}path_history_point_type_06_item;

typedef struct path_history_point_type_07_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	unsigned short time_offset; /* integer (0...65535) */
	positional_accuracy_item pos_accuracy;
}path_history_point_type_07_item;

typedef struct path_history_point_type_08_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	unsigned short time_offset; /* integer (0...65535) ,lsb units of 10msec*/
}path_history_point_type_08_item;

typedef struct path_history_point_type_09_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
	positional_accuracy_item pos_accuracy;
}path_history_point_type_09_item;

typedef struct path_history_point_type_10_str
{
	int lat_offset; /* integer (-131072...131071) */
	int long_offset; /* integer (-131072...131071) */
}path_history_point_type_10_item;

typedef struct path_history_point_str //R63
{
	unsigned char mask_speed:1,
			mask_pos_accuracy:1,
			mask_heading:1,
			reserved:5;
	
	int lat_offset; //INTEGER (-131072..131071); LSB: 0.1 microdegrees; unknown: -131072
	int lon_offset; //INTEGER (-131072..131071); LSB: 0.1 microdegrees; unknown: -131072
	int elevation_offset; //INTEGER (-2048..2047); LSB: 10 cm; unavailable: -2048
	unsigned short time_offset; //INTEGER (1..65535); LSB: 10 mSec; unavailable: 65535
	unsigned short speed; //OP; INTEGER (0..8191); LSB: 0.02 m/s; unavailable; 8191
	positional_accuracy_item pos_accuracy; //OP
	unsigned char heading; //OP; INTEGER (0..240); LSB: 1.5 degrees; unavailable: 240
}path_history_point_item;


typedef struct path_history_str //R63
{
	unsigned char mask_initial_position:1,
			mask_curr_gps_status:1,
			reserved:6;

	full_position_vector_item *initial_position; //OP
	unsigned char curr_gps_status; //OP; BIT STRING 8 bits; RANGE(0..255)
	
	unsigned char crumb_data_num; //SEQUENCE (SIZE(1..23))
	path_history_point_item *crumb_data;
}path_history_item;


typedef struct vehicle_safety_extension_str
{
	unsigned char mask_events:1,
			mask_path_history:1,
			mask_path_prediction:1,
			mask_the_rtcm:1,
			rest:4;

	unsigned short events;	/*optional, integer (0..8192)*/
	path_history_item *path_history;	/*optional*/
	path_prediction_item *path_prediction; /*optional*/
	rtcm_package_item *the_rtcm; /*optional*/
}vehicle_safety_extension_item;

typedef struct node_xy_20b_str //R63
{
	int x; //integer (-512..511)
	int y; //integer (-512..511)
}node_xy_20b_item;

typedef struct node_xy_22b_str //R63
{
	int x; //integer (-1024..1023)
	int y; //integer (-1024..1023)
}node_xy_22b_item;

typedef struct node_xy_24b_str //R63
{
	int x; //INTEGER (-2048..2047); LSB: 1 cm; unknown: -2048
	int y; //INTEGER (-2048..2047); LSB: 1 cm; unknown: -2048
}node_xy_24b_item;

typedef struct node_xy_26b_str //R63
{
	int x; //integer (-4096..4095)
	int y; //integer (-4096..4095)
}node_xy_26b_item;

typedef struct node_xy_28b_str //R63
{
	int x; //integer (-8192..8191)
	int y; //integer (-8192..8191)
}node_xy_28b_item;

typedef struct node_xy_32b_str //R63
{
	int x; //integer (-32768..32767)
	int y; //integer (-32768..32767)
}node_xy_32b_item;

typedef struct node_ll_24b_str //R63
{
	short lon; //INTEGER (-2048..2047)
	short lat; //INTEGER (-2048..2047)
}node_ll_24b_item;

typedef struct node_ll_28b_str //R63
{
	short lon; //INTEGER (-8192..8191)
	short lat; //INTEGER (-8192..8191)
}node_ll_28b_item;

typedef struct node_ll_32b_str //R63
{
	int lon; //INTEGER (-32768..32767)
	int lat; //INTEGER (-32768..32767)
}node_ll_32b_item;

typedef struct node_ll_36b_str //R63
{
	long lon; //INTEGER (-131072..131071)
	long lat; //INTEGER (-131072..131071)
}node_ll_36b_item;

typedef struct node_ll_44b_str //R63
{
	long lon; //INTEGER (-2097152..2097151)
	long lat; //INTEGER (-2097152..2097151)
}node_ll_44b_item;

typedef struct node_ll_48b_str //R63
{
	long lon; //INTEGER (-8388608..8388607)
	long lat; //INTEGER (-8388608..8388607)
}node_ll_48b_item;

typedef struct node_llmd_64b_item_str //R63
{
	long lon; //INTEGER (-1799999999..1800000001)
	long lat; //INTEGER (-900000000..900000001)
}node_llmd_64b_item;

typedef struct node_offset_pointXY_item_str //R63
{
	unsigned char choice;

	union{
		/*choice 1: node_xy1*/
		node_xy_20b_item node_xy1;	
		/*choice 2: node_xy2*/
		node_xy_22b_item node_xy2;
		/*choice 3: node_xy3*/
		node_xy_24b_item node_xy3;
		/*choice 4: node_xy4*/
		node_xy_26b_item node_xy4;
		/*choice 5: node_xy5*/
		node_xy_28b_item node_xy5;
		/*choice 6: node_xy6*/
		node_xy_32b_item node_xy6;
		/*choice 7: node_latlon*/
		node_llmd_64b_item node_lat_lon;
	};	
}node_offset_pointXY_item;

typedef struct signal_head_location_str {
	node_offset_pointXY_item node;
	unsigned char signal_group_id; //integer (0..255)	
}signal_head_location_item;

typedef struct reg_map_data_str //R63
{
	unsigned char mask_signal_head_locations:1,
				  rest:7;

	unsigned char region_id;  //INTEGER (0..255)

	unsigned char signal_head_locations_num;	
	signal_head_location_item *signal_head_locations; //OP, SEQUENCE (SIZE(1..20)) OF SignalHeadLocation
}reg_map_data_item;

typedef struct node_offset_pointLL_item_str //R63
{
	unsigned char choice;

	union{
		/*choice 1: node_ll1*/
		node_ll_24b_item node_ll1;
		/*choice 2: node_ll2*/
		node_ll_28b_item node_ll2;
		/*choice 3: node_ll3*/
		node_ll_32b_item node_ll3;
		/*choice 4: node_ll4*/
		node_ll_36b_item node_ll4;
		/*choice 5: node_ll5*/
		node_ll_44b_item node_ll5;
		/*choice 6: node_ll6*/
		node_ll_48b_item node_ll6;
		/*choice 7: node_latlon*/
		node_llmd_64b_item node_lat_lon;
		/*choice 8: node_regional*/	
		regional_extension_item regional;
	};	
}node_offset_pointLL_item;

typedef struct intersection_reference_id_item_str //R63
{
	unsigned char mask_region:1,
				  rest:7;

	unsigned int region; //OP; INTEGER (0..65535)
	unsigned int id; //INTEGER (0..65535)
}intersection_reference_id_item;

typedef struct its_header_str
{
	char protocol_version; /*integer(0..255)*/
	char message_id; /*integer(0..255)*/
	unsigned long station_id; /*integer(0..4294967295),unsigned long long = 8 bytes,unsigned long = 4 bytes(stationid)*/
}its_header_item;

typedef struct regulatory_speed_limit_str //R63
{
	unsigned char type; //ENUMERATED {0..12}; unknown: 0
	unsigned int speed; //INTEGER (0..8191); LSB: 0.02 m/s; unavailable: 8191
}regulatory_speed_limit_item;

typedef struct lane_type_attributes_str //R63
{
	unsigned char lane_type_choice;

	union{
		/*choice 1: vehicle*/
		unsigned short vehicle; //16 bit string
		/*choice 2: crosswalk*/
		unsigned short crosswalk; //16 bit string
		/*choice 3: bikelane*/
		unsigned short bike_lane; //16 bit string
		/*choice 4: sidewalk*/
		unsigned short sidewalk; //16 bit string
		/*choice 5: median*/
		unsigned short median; //16 bit string
		/*choice 6: striping*/
		unsigned short striping; //16 bit string
		/*choice 7: trackedvehicle*/
		unsigned short tracked_vehicle; //16 bit string
		/*choice 8: parking*/
		unsigned short parking; //16 bit string
	};	
}lane_type_attributes_item;

typedef struct lane_attributes_str //R63
{
	unsigned char mask_regional:1,				  
				  rest:7;

	unsigned char directional_use;	/*BIT STRING  2 bits; RANGE(0..3)*/
	unsigned short shared_with; 		/*BIT STRING 10 bits; RANGE(0..1023)*/
	lane_type_attributes_item lane_type;
}lane_attributes_item;

typedef struct latitude_dms2_str //R63
{
	char d; 					/*INTEGER (-90..90)*/
	unsigned char m; 	/*INTEGER (0..59)*/
	unsigned short s; /*INTEGER (0..5999)*/
}latitude_dms2_item;

typedef struct longitude_dms2_str //R63
{
	short d; 					/*INTEGER (-180..180)*/
	unsigned char m; 	/*INTEGER (0..59)*/
	unsigned short s; /*INTEGER (0..5999)*/
}longitude_dms2_item;

typedef struct altitude_str {
	long value; 							/*INTEGER (-100000..800001)*/
	unsigned char confidence; /*ENUMERATED (0..15)*/
}altitude_item;

typedef struct reg_position3d_item_str //R63
{
	unsigned char region_id; /*INTEGER (0..255)*/
	altitude_item altitude;
}reg_position3d_item;

typedef struct position3d_2_str //R63
{
	unsigned char mask_elevation:1,		
			  mask_regional:1,
			  rest:6;

	long lat; 			/*INTEGER (-900000000..900000001); LSB: 1/10 micro degree; unavailable: 900000001*/
	long longitude; /*INTEGER (-1799999999..1800000001); LSB: 1/10 micro degree; unavailable: 1800000001*/
	long elevation; /*OP; INTEGER (-4096..61439); LSB: 10 cm; Unknown: -4096*/

	unsigned char regional_num; /*SEQUENCE (SIZE(1..4))*/
	reg_position3d_item *regional; /*OP*/
}position3d_2_item;

typedef struct lane_data_attribute_str //R63
{
	unsigned char lane_data_attribute_choice;

	/*choice 1: pathendpointangle*/
	int path_end_point_angle;			/*INTEGER (-150..150)*/ 

	/*choice 2: lanecrownpointcenter*/
	int lane_crown_point_center;	/*INTEGER (-128..127)*/

	/*choice 3: lanecrownpointleft*/
	int lane_crown_point_left;		/*INTEGER (-128..127)*/

	/*choice 4: lanecrownpointright*/
	int lane_crown_point_right;		/*INTEGER (-128..127)*/

	/*choice 5: laneangle*/
	int lane_angle;								/*INTEGER (-180..180)*/

	/*choice 6: speedlimits*/
	unsigned char speed_limits_num; 
	regulatory_speed_limit_item *speed_limits; /*SEQUENCE (size(1..9)) of regulatoryspeedlimit*/

	/*choice 7: regional*/
	unsigned char regional_num;
	reg_lane_data_attribute_item *regional; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-LaneDataAttribute}}*/
}lane_data_attribute_item;

typedef struct node_attribute_setXY_str //R63
{
	unsigned char mask_local_node:1,
				  mask_disabled:1,
				  mask_enabled:1,
				  mask_data:1,				  
				  mask_d_width:1,
				  mask_d_elevation:1,
				  mask_regional:1,
				  rest:1;
	
	unsigned char local_node_num; 
	unsigned char *local_node;/*OP, SEQUENCE (size(1..8)) of nodeattribute, ENUMERATED (0..11)*/

	unsigned char disabled_num; 
	unsigned char *disabled;	/*OP, SEQUENCE (size(1..8)) of nodeattribute, ENUMERATED (0..37)*/

	unsigned char enabled_num; 
	unsigned char *enabled;		/*OP, SEQUENCE (size(1..8)) of nodeattribute, ENUMERATED (0..37)*/

	unsigned char data_num;
	lane_data_attribute_item *data; /*OP, SEQUENCE (size(1..8)) of lanedataattribute*/

	int	d_width;			/*OP, INTEGER (-512..511)*/
	int	d_elevation;	/*OP, INTEGER (-512..511)*/
}node_attribute_setXY_item;

typedef struct node_attribute_setLL_str //R63
{
	unsigned char mask_local_node:1,
				  mask_disabled:1,
				  mask_enabled:1,
				  mask_data:1,				  
				  mask_d_width:1,
				  mask_d_elevation:1,
				  mask_regional:1,
				  rest:1;
	
	unsigned char local_node_num; 
	unsigned char *local_node;			/*OP, SEQUENCE (size(1..8)) of nodeattribute, ENUMERATED (0..11)*/

	unsigned char disabled_num; 
	unsigned char *disabled;				/*OP, SEQUENCE (size(1..8)) of nodeattribute, ENUMERATED (0..37)*/

	unsigned char enabled_num; 
	unsigned char *enabled;					/*OP, SEQUENCE (size(1..8)) of nodeattribute, ENUMERATED (0..37)*/

	unsigned char data_num;
	lane_data_attribute_item *data; /*OP, SEQUENCE (size(1..8)) of lanedataattribute*/

	int	d_width;			/*OP, INTEGER (-512..511)*/
	int	d_elevation;	/*OP, INTEGER (-512..511)*/ 

	unsigned char regional_num;
	regional_extension_item regional[4]; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-EventDescription}} OPTIONAL*/
}node_attribute_setLL_item;

typedef struct nodeXY_str //R63
{
	unsigned char mask_attributes:1,
				  rest:7;

	node_offset_pointXY_item delta;
	node_attribute_setXY_item attributes; //OP
}nodeXY_item;

typedef struct nodeLL_str //R63
{
	unsigned char mask_attributes:1,
				  rest:7;

	node_offset_pointLL_item delta;
	node_attribute_setLL_item attributes; //OP
}nodeLL_item;

typedef struct computed_lane_str //R63
{
	unsigned char mask_rotate_xy:1,
				  mask_scale_xaxis:1,
				  mask_scale_yaxis:1,
				  mask_regional:1,
				  rest:4;
		
	unsigned char reference_lane_id; /*INTEGER (0..255)*/

	struct offset_xaxis_str {
		unsigned char offset_xaxis_choice;

		/*choice 1: small*/
		int small; /*INTEGER (-2047..2047)*/
		/*choice 2: large*/
		int large; /*INTEGER (-32767..32767)*/
	} offset_xaxis_item;

	struct offset_yaxis_str {
		unsigned char offset_yaxis_choice;		

		/*choice 1: small*/
		int small; /*INTEGER (-2047..2047)*/
		/*choice 2: large*/
		int large; /*INTEGER (-32767..32767)*/	
	} offset_yaxis_item;
	
	unsigned int rotate_xy; /*OP, INTEGER (0..28800)*/ 
	int scale_xaxis;	/*OP, INTEGER (-2048..2047)  -- in steps of 0.05 percent*/
	int scale_yaxis;	/*OP, INTEGER (-2048..2047)  -- in steps of 0.05 percent*/
}computed_lane_item;

typedef struct node_listXY_str //R63
{
	unsigned char node_list_choice;

	/*choice 1: nodes*/
	unsigned char nodes_num; 
	nodeXY_item *nodes; /*SEQUENCE (SIZE(2..63)) OF NodeXY*/

	/*choice 2: computed*/
	computed_lane_item computed;
}node_listXY_item;

typedef struct node_listLL_str //R63
{
	unsigned char node_list_choice;

	/*choice 1: nodes*/
	unsigned char nodes_num; 
	nodeLL_item *nodes; /*SEQUENCE (SIZE(2..63)) OF NodeLL*/
}node_listLL_item;

typedef struct connecting_lane_str //R63
{
	unsigned char mask_maneuver:1,
				  rest:7;
	
	unsigned char lane; 		/*INTEGER (0..255)*/
	unsigned short maneuver;/*OP, BIT STRING (size(12))*/
}connecting_lane_item;

typedef struct connection_str //R63
{
	unsigned char mask_remote_intersection:1,
				  mask_signal_group:1,
				  mask_user_class:1,
				  mask_connection_id:1,
				  rest:4;
	
	connecting_lane_item connecting_lane;
	intersection_reference_id_item remote_intersection; /*OP*/
	unsigned char signal_group; /*OP, INTEGER (0..255)*/
	unsigned char user_class;		/*OP, INTEGER (0..255)*/
	unsigned char connection_id;/*OP, INTEGER (0..255)*/	
}connection_item;

typedef struct generic_lane_str //R63
{
	unsigned char mask_name:1,
				  mask_ingress_approach:1,
				  mask_egress_approach:1,
				  mask_maneuvers:1,
				  mask_connects_to:1,
				  mask_overlays:1,
				  mask_regional:1,
				  rest:1;

	unsigned char lane_id;					/*INTEGER (0..255); unavailable: 0*/
	unsigned char name[64]; 				/*OP; IA5String (SIZE(1..63))*/
	unsigned char ingress_approach;	/*OP; INTEGER (0..15); unknown: 0*/
	unsigned char egress_approach;	/*OP; INTEGER (0..15); unknown: 0*/
	lane_attributes_item lane_attributes;	
	unsigned short maneuvers;				/*OP; BIT STRING 12 bits; RANGE(0..4095)*/
	node_listXY_item node_list;

	unsigned char connects_to_num;	/*SEQUENCE (size(1..16))*/
	connection_item *connects_to;		/*OP*/

	unsigned char overlays_num;			/*SEQUENCE (size(1..5))*/
	unsigned char *overlays;				/*OP*/
}generic_lane_item;

typedef struct signal_control_zone_str //R63
{
	regional_extension_item zone;
}signal_control_zone_item;

typedef struct intersection_geometry_str //R63
{
	unsigned char mask_name:1,		
				  mask_lane_width:1,	
				  mask_speed_limits:1,
				  mask_preempt_priority_data:1,
				  mask_regional:1,
				  rest:3;

	unsigned char name[64]; 		/*OP; IA5String (SIZE(1..63))*/
	intersection_reference_id_item id;	
	unsigned char revision; 		/*INTEGER (0..127)*/
	position3d_2_item ref_point;
	unsigned short lane_width; 	/*OP; INTEGER (0..32767); LSB: 1 cm*/

	unsigned char speed_limits_num; /*SEQUENCE (size(1..9))*/
	regulatory_speed_limit_item *speed_limits; /*OP*/
	
	unsigned char lane_set_num;	/*SEQUENCE (size(1..255))*/
	generic_lane_item *lane_set; 

	unsigned char preempt_priority_data_num; /*SEQUENCE (SIZE(1..32))*/
	signal_control_zone_item *preempt_priority_data; /*OP*/
}intersection_geometry_item;

typedef struct data_parameters_str //R63
{
	unsigned char mask_process_method:1,
				  mask_process_agency:1,
				  mask_last_checked_date:1,
				  mask_geoid_used:1,
				  rest:4;

	unsigned char process_method[256]; 		/* OP, IA5STRING (size(1..255))*/
	unsigned char process_agency[256]; 		/* OP, IA5STRING (size(1..255))*/
	unsigned char last_checked_date[256]; /* OP, IA5STRING (size(1..255))*/
	unsigned char geoid_used[256]; 				/* OP, IA5STRING (size(1..255))*/
}data_parameters_item;

typedef struct road_segment_reference_id_str //R63
{
	unsigned char mask_region:1,
				  rest:7;
	
	unsigned int region; 	/*OP, INTEGER (0..65535)*/
	unsigned int id; 			/*INTEGER (0..65535)*/
}road_segment_reference_id_item;

typedef struct road_segment_str //R63
{
	unsigned char mask_name:1,
				  mask_lane_width:1,
				  mask_speed_limits:1,
				  mask_regional:1,
				  resr:4;
	
	unsigned char name[64]; /*OP, IA5STRING (size(1..63))*/
	road_segment_reference_id_item id;
	unsigned char revision; /*INTEGER (0..127)*/
	position3d_2_item ref_point;	
	unsigned short lane_width; /*OP, INTEGER (0..32767) -- units of 1 cm*/

	unsigned char speed_limits_num;
	regulatory_speed_limit_item *speed_limits; /*OP, SEQUENCE (SIZE(1..9)) of regulatoryspeedlimit*/
	
	unsigned char road_lane_set_num;
	generic_lane_item *road_lane_set; /*SEQUENCE (SIZE(1..255)) of genericlane*/
}road_segment_item;

typedef struct reg_restriction_user_type_str {
	unsigned char mask_emission:1,
				  rest:7;
	
	unsigned char region_id;  /*INTEGER (0..255)*/
	unsigned char emission; /*OP, ENUMERATED (0..4)*/
}reg_restriction_user_type_item;

typedef struct restriction_user_type_str //R63
{
	unsigned char users_choice;

	/*choice 1: basictype*/
	unsigned char basic_type; /*ENUMERATED (0..13)*/
	
	/*choice 2: regional*/
	unsigned char regional_num;
	reg_restriction_user_type_item regional[4]; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-RestrictionUserType}}*/
}restriction_user_type_item;

typedef struct restriction_class_assignment_str //R63
{
	unsigned char id; /*INTEGER (0..255)*/

	unsigned char users_num;
	restriction_user_type_item *users; /*SEQUENCE (size(1..16)) of  restrictionusertype*/
}restriction_class_assignment_item;

typedef struct time_change_details_str //R63
{
	unsigned char mask_start_time:1,
				  mask_max_end_time:1,
				  mask_likely_time:1,
				  mask_confidence:1,
				  mask_next_time:1,
				  rest:3;

	unsigned int start_time;	/*OP, INTEGER (0..36001)*/
	unsigned int min_end_time;/*INTEGER (0..36001)*/
	unsigned int max_end_time;/*OP, INTEGER (0..36001)*/
	unsigned int likely_time; /*OP, INTEGER (0..36001)*/
	unsigned char confidence; /*OP, INTEGER (0..15)*/
	unsigned int next_time; 	/*OP, INTEGER (0..36001)*/
}time_change_details_item;

typedef struct advisory_speed_str //R63
{
	unsigned char mask_speed:1,
				  mask_confidence:1,
				  mask_distance:1,
				  mask_as_class:1,
				  mask_regional:1,
				  rest:3;
	
	unsigned char type; 			/*ENUMERATED (0..3)*/
	unsigned int speed; 			/*OP, INTEGER (0..500) -- units of 0.1 m/s*/
	unsigned char confidence; /*OP, ENUMERATED (0..7)*/
	unsigned int distance; 		/*OP, INTEGER (0..10000)*/
	unsigned char as_class; 	/*OP, INTEGER (0..255)*/
}advisory_speed_item;

typedef struct movement_event_str //R63
{
	unsigned char mask_timing:1,
				  mask_speeds:1,
				  mask_regional:1,
				  rest:5;
		
	unsigned char event_state; /*ENUMERATED (0..9)*/
	time_change_details_item timing; /*OP*/

	unsigned char speeds_num;
	advisory_speed_item *speeds;/*OP, SEQUENCE (size(1..16)) of advisoryspeed*/
}movement_event_item;

typedef struct vehicle_to_lane_position_str {
	unsigned long station_id; /*INTEGER (0..4294967295)*/
	unsigned char lane_id; 		/*INTEGER (0..255)*/
}vehicle_to_lane_position_item;

typedef struct reg_connection_maneuver_assist_str //R63
{
	unsigned char mask_rsu_distance_from_anchor:1,
				  rest:7;

	unsigned char region_id;  /*INTEGER (0..255)*/
	
	unsigned char vehicle_to_lane_positions_num;
	vehicle_to_lane_position_item vehicle_to_lane_positions[5]; /*SEQUENCE SIZE(1..5) OF VehicleToLanePosition*/
	
	node_offset_pointXY_item rsu_distance_from_anchor; /*OP*/
}reg_connection_maneuver_assist_item;

typedef struct connection_maneuver_assist_str //R63
{
	unsigned char mask_queue_length:1,
				  mask_available_storage_length:1,
				  mask_wait_on_stop:1,
				  mask_ped_bicycle_detect:1,
				  mask_regional:1,
				  rest:3;

	unsigned char connection_id;					/*INTEGER (0..255)*/
	unsigned int queue_length;						/*OP, INTEGER (0..10000)*/
	unsigned int available_storage_length;/*OP, INTEGER (0..10000)*/
	unsigned char wait_on_stop; 					/*OP, BOOLEAN*/
	unsigned char ped_bicycle_detect; 		/*OP, BOOLEAN*/

	unsigned char regional_num;
	reg_connection_maneuver_assist_item *regional; /*OP, SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-ConnectionManeuverAssist}}*/
}connection_maneuver_assist_item;

typedef struct movement_state_str //R63
{
	unsigned char mask_movement_name:1,
				  mask_maneuver_assist_list:1,
				  mask_regional:1,
				  rest:5;
	
	unsigned char movement_name[64];	/*OP, IA5STRING (size(1..63))*/
	unsigned char signal_group; 			/*INTEGER (0..255)*/

	unsigned char state_time_speed_num;
	movement_event_item *state_time_speed; /*SEQUENCE (size(1..16)) of movementevent*/

	unsigned char maneuver_assist_list_num;
	connection_maneuver_assist_item *maneuver_assist_list; /*OP, SEQUENCE (size(1..16)) of connectionmaneuverassist*/
}movement_state_item;

typedef struct prioritization_response_str //R63
{
	unsigned long station_id;		/*INTEGER(0..4294967295)*/
	unsigned char prior_state;	/*ENUMERATED (0..7)*/
	unsigned char signal_group; /*INTEGER (0..255)*/
}prioritization_response_item;

typedef struct reg_intersection_state_str //R63
{
	unsigned char mask_active_prioritizations:1,
				  rest:7;

	unsigned char region_id;  /*INTEGER (0..255)*/

	unsigned char active_prioritizations_num;
	prioritization_response_item *active_prioritizations; /*OP, SEQUENCE SIZE(1..10) OF PrioritizationResponse*/
}reg_intersection_state_item;

typedef struct intersection_state_str //R63
{
	unsigned char mask_name:1,
				  mask_moy:1,
				  mask_time_stamp:1,
				  mask_enabled_lanes:1,
				  mask_maneuver_assist_list:1,
				  mask_regional:1,
				  rest:2;

	unsigned char name[64]; /*OP, IA5STRING (size(1..63))*/
	intersection_reference_id_item id;
	unsigned char revision; /*INTEGER (0..127)*/
	unsigned short status;	/*BIT STRING {size(16)}*/
	unsigned long moy;			/*OP, INTEGER (0..527040)*/
	unsigned int time_stamp;/*OP, INTEGER (0..65535) -- units of miliseconds*/

	unsigned char enabled_lanes_num;
	unsigned char *enabled_lanes; /*OP, SEQUENCE (size(1..16)) of laneid, LaneID ::= INTEGER (0..255)*/

	unsigned char states_num;
	movement_state_item *states;	/*SEQUENCE (size(1..255)) of movementstate*/

	unsigned char maneuver_assist_list_num;
	connection_maneuver_assist_item *maneuver_assist_list; 	/*OP, SEQUENCE (size(1..16)) of connectionmaneuverassist*/ 

	unsigned char regional_num;
	reg_intersection_state_item *regional; /*OP, SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-IntersectionState}}*/
}intersection_state_item;

typedef struct privileged_events_str //R63
{
	unsigned char ssp_rights; /*INTEGER (0..31)*/
	unsigned short event;			/*BIT STRING 6 bits; RANGE(0..63)*/
}privileged_events_item;

typedef struct emergency_details__str //R63
{
	unsigned char mask_events:1,
				  mask_response_type:1,
				  rest:6;
	
	unsigned char ssp_rights;		/*INTEGER (0..31)*/
	unsigned char siren_use;		/*ENUMERATED (0..3)*/
	unsigned char lights_use;		/*ENUMERATED (0..7)*/
	unsigned char multi;				/*ENUMERATED (0..3)*/
	privileged_events_item events;/*OP*/
	unsigned char response_type;/*ENUMERATED (0..6)*/
}emergency_details_item;

typedef struct event_description_str //R63
{
	unsigned char mask_description:1,
				  mask_priority:1,
				  mask_heading:1,
				  mask_extent:1,
				  mask_regional:1,
				  rest:3;
	
	unsigned short type_event;		/*INTEGER (0.. 65535)*/
	unsigned char description_num;/*SEQUENCE (SIZE(1..8))*/
	unsigned short description[8];/*OP; INTEGER (0.. 65535)*/
	char priority; 								/*OCTET STRING (SIZE(1))*/
	unsigned short heading;				/*BIT STRING 16 bits; RANGE(0..65535)*/
	unsigned char extent;					/*ENUMERATED (0..15)*/
}event_description_item;

typedef struct pivot_point_description_str //R63
{
	short pivot_offset;					/*INTEGER (-1024..1023); LSB: 1 cm; unknown: -1024*/
	unsigned short pivot_angle; /*INTEGER (0..28800); LSB: 0.0125 degrees; unavailable: 28800*/
	char pivots; 								/*BOOLEAN 1 bit; RANGE(0..1)*/
} pivot_point_description_item;

typedef struct trailer_history_point_str //R63
{
	unsigned char mask_elevation_offset:1,
				  mask_heading:1,
				  rest:6;

	unsigned short pivot_angle; /*INTEGER (0..28800); LSB: 0.0125 degrees; unavailable: 28800*/
	unsigned short time_offset; /*INTEGER (1..65535); LSB: 10 mSec; unavailable: 65535*/
	node_xy_24b_item position_offset;
	char elevation_offset; 			/*OP, INTEGER (-64..63); LSB: 10 cm; unavailable: -64*/
	unsigned char heading; 			/*OP, INTEGER (0..240); LSB: 1.5 degrees; unavailable: 240*/
} trailer_history_point_item;

typedef struct trailer_unit_description_str //R63
{
	unsigned char mask_height:1,
				  mask_mass:1,
				  mask_bumper_heights:1,
				  mask_center_of_gravity:1,
				  mask_rear_pivot:1,
				  mask_rear_wheel_offset:1,
				  mask_elevation_offset:1,
				  mask_crumb_data:1;
	
	char is_dolly;					/*BOOLEAN 1 bit; RANGE(0..1)*/
	unsigned short width;		/*INTEGER (0..1023); LSB: 1 cm; unavailable: 0*/
	unsigned short length;	/*INTEGER (0..4095); LSB: 1 cm; unavailable: 0*/
	unsigned char height;		/*OP, INTEGER (0..127); LSB: 5 cm*/
	unsigned char mass;			/*OP, INTEGER (0..255); LSB: 500 kg; unknown: 0*/
	bumper_heights_item bumper_heights; /*OP*/
	unsigned char center_of_gravity; /*OP, INTEGER (0..127); LSB: 5 cm*/
	pivot_point_description_item front_pivot;
	pivot_point_description_item rear_pivot; /*OP*/
	short rear_wheel_offset;/*OP, INTEGER (-2048..2047); LSB: 1 cm; unknown: -2048*/
	node_xy_24b_item position_offset;
	char elevation_offset;	/*OP, INTEGER (-64..63); LSB: 10 cm; unavailable: -64*/
	
	unsigned char crumb_data_num; /*SEQUENCE (SIZE(1..23))*/ 
	trailer_history_point_item *crumb_data; /*OP*/
} trailer_unit_description_item;

typedef struct trailer_data_str //R63
{
	unsigned char ssp_rights; /*INTEGER (0..31)*/
	pivot_point_description_item connection;
	
	unsigned char units_num;	/*SEQUENCE (SIZE(1..8))*/ 
	trailer_unit_description_item *units;
} trailer_data_item;

typedef struct vehicle_classification_str //R63
{
	unsigned char mask_key_type:1,
				  mask_role:1,
				  mask_iso3883:1,
				  mask_hpms_type:1,
				  mask_vehicle_type:1,
				  mask_response_equip:1,
				  mask_responder_type:1,
				  mask_fuel_type:1;
	unsigned char mask_regional:1,				  
				  rest:7;

	unsigned char key_type;		/*OP, INTEGER (0..255); unknown: 0*/
	unsigned char role;				/*OP, ENUMERATED (0..22)*/
	unsigned char iso3883;		/*OP, INTEGER (0..100)*/
	unsigned char hpms_type;	/*OP, ENUMERATED (0..15); unavailable: 0*/
	unsigned short vehicle_type;	/*OP, ENUMERATED (9217 to 9251)*/
	unsigned short response_equip;/*OP, ENUMERATED (9985 to 10114)*/
	unsigned short responder_type;/*OP, ENUMERATED (9729 to 9742)*/
	unsigned char fuel_type; 	/*OP, INTEGER (0..15); unknown: 0*/
} vehicle_classification_item;

typedef struct vehicle_safety_extensions_str //R63
{
	unsigned char mask_events:1,
				  mask_path_history:1,
				  mask_path_prediction:1,
				  mask_lights:1,
				  rest:4;

	unsigned short events; /*OP; BIT STRING 13 bits; RANGE(0..8191)*/
	path_history_item *path_history; /*OP*/
	path_prediction_item path_prediction; /*OP*/
	unsigned short lights; /*OP; BIT STRING 9 bits; RANGE(0..511)*/
}vehicle_safety_extensions_item;

typedef struct special_vehicle_extensions_str //R63
{
	unsigned char mask_vehicle_alerts:1,
				  mask_description:1,
				  mask_trailers:1,
				  rest:5;

	emergency_details_item vehicle_alerts; /*OP*/
	event_description_item description; /*OP*/
	trailer_data_item trailers; /*OP*/
}special_vehicle_extensions_item;

typedef struct supplemental_vehicle_extensions_str //R63
{
	unsigned char mask_classification:1,
				  mask_class_details:1,
				  mask_vehicle_data:1,
				  mask_weather_report:1,
				  mask_weather_probe:1,
				  mask_obstacle:1,
				  mask_status:1,
				  mask_speed_profile:1;
	unsigned char mask_the_rtcm:1,
				  mask_regional:1,
				  rest:6;

	unsigned char classification; /*OP, INTEGER (0..255); unknown: 0*/
	vehicle_classification_item class_details; /*OP*/
	vehicle_data_item vehicle_data;		/*OP*/
	weather_report_item weather_report; /*OP*/
	weather_probe_item weather_probe;	/*OP*/
	obstacle_detection_item obstacle;	/*OP*/
	disabled_vehicle_item status; 		/*OP*/
	
	unsigned char speed_profile_num; /*SEQUENCE (SIZE(1..20))*/ 
	unsigned char speed_profile[20]; /*OP; INTEGER (0..31)*/
	rtcm_package_item the_rtcm; /*OP*/
}supplemental_vehicle_extensions_item;

typedef struct partII_str //R63
{
	unsigned char partII_id; /*0:vse; 1:spve; 2:suve*/
	vehicle_safety_extensions_item *vse;
	special_vehicle_extensions_item *spve;
	supplemental_vehicle_extensions_item *suve;
}partII_item;

typedef struct bsm_core_data_str //R63
{
	unsigned char msg_cnt;	/*INTEGER (0..127)*/
	unsigned char id[4];		/*OCTET STRING (SIZE(4))*/
	unsigned short sec_mark;/*INTEGER (0..65535); units of milliseconds; unavailable: 65535*/
	int lat; 			/*INTEGER (-900000000..900000001); LSB: 1/10 micro degree; unavailable: 900000001*/
	int longitude;/*INTEGER (-1799999999..1800000001); LSB: 1/10 micro degree; unavailable: 1800000001*/
	int elev;			/*RANG(-4096..61439); LSB = 10 cm; unknown: -4096*/
	positional_accuracy_item accuracy;
	unsigned char transmission_state; /*ENUMERATED (0..7)*/
	unsigned short speed;		/*INTEGER (0..8191); LSB: 0.02 m/s; unavailable: 8191*/
	unsigned short heading; /*INTEGER (0..28800); LSB: 0.0125 degrees; unavailable: 28800*/
	signed char angle;			/*RANGE(-126..127); LSB = 1.5 degrees; unavailable: 127*/
	acceleration_set_4way_item accel_set;
	brake_system_status_item brakes;
	vehicle_size_item size;
}bsm_core_data;

typedef struct heading_slice_str
{
	unsigned char from000_0to022_5degrees:1,
					from022_5to045_0degrees:1,
					from045_0to067_5degrees:1,
					from067_5to090_0degrees:1,
					from090_0to112_5degrees:1,
					from112_5to135_0degrees:1,
					from135_0to157_5degrees:1,
					from157_5to180_0degrees:1;
   
   unsigned char from180_0to202_5degrees:1,
				   from202_5to225_0degrees:1,
				   from225_0to247_5degrees:1,
				   from247_5to270_0degrees:1,
				   from270_0to292_5degrees:1,
				   from292_5to315_0degrees:1,
				   from315_0to337_5degrees:1,
				   from337_5to360_0degrees:1;
}heading_slice_item;

typedef struct vehicle_request_status_str
{
	char msb;/* (msb)  brakes-on*/
	char emergency_use;/*emergency use or operation*/
	char lights_in_use;/*lights in use (see also the light bar element)*/
	char bit5;
	char lightbar_in_use;
	char transist_status;/* -- when a priority, map the values of 
						-- lightbarinuse to the lower 4 bits 
						-- and set the 5th bit to zero
						-- when a preemption, map the values of 
						-- transiststatus to the lower 5 bits
						*/				
}vehicle_request_status_item;

typedef struct transit_status_str
{
	char none;	
	char an_ad_ause;
	char a_bike_load;
	char door_open;
	char occ_m;
	char occ_l;
}transit_status_item;

typedef struct position3d_str //R63
{
	unsigned char mask_elevation:1,
			  mask_regional:1,
		      rest:6;

	long lat;				/* INTEGER(-900000000..900000001) */
	long longitude;	/* INTEGER(-1799999999..1800000001) */
	int elevation; 	/*OP; RANG(-4095..61439); LSB = 0.1 meters; unknown: 61440*/

	unsigned char regional_num;
	regional_extension_item regional[4]; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-EventDescription}} OPTIONAL*/
}position3d_item;

typedef struct road_sign_id_str //R63
{
	unsigned char mask_mutcd_code:1,
		      mask_crc:1,
		      rest:6;

	position3d_item position;
	short view_angle;					/*BIT STRING (size(16))*/
	unsigned char mutcd_code;	/*OP, ENUMERATED (0..6)*/
	unsigned char crc[2];			/*OP*/
}road_sign_id_item;

typedef struct region_offsets_str //R63
{
	unsigned char mask_z_offset:1,
		      rest:7;

	signed short x_offset; /*INTEGER (-32768..32767)*/ 
	signed short y_offset; /*INTEGER (-32768..32767)*/
	signed short z_offset; /*OP, INTEGER (-32768..32767)*/
}region_offsets_item;

typedef struct shape_point_set_str //R63
{
	unsigned char mask_anchor:1,
				mask_lane_width:1,
				mask_directionality:1,
				rest:5;

	position3d_item anchor;		/*OP*/
	unsigned short lane_width;/*OP, INTEGER (0..32767)*/
	unsigned char directionality;/*OP, ENUMERATED (0..3)*/
	node_listXY_item node_list;
}shape_point_set_item;

typedef struct circle_str //R63
{
	position3d_item center;
	unsigned short radius;/*INTEGER (0..4095)*/
	unsigned char units;	/*ENUMERATED (0..7)*/
}circle_item;

typedef struct region_point_set_str //R63
{
	unsigned char mask_anchor:1,
				mask_scale:1,
		      	rest:6;

	position3d_item anchor; /*OP*/
	unsigned char scale;		/*OP, INTEGER (0..15)*/

	unsigned char node_list_num;
	region_offsets_item *node_list; /*SEQUENCE (SIZE(1..64)) OF RegionOffsets*/
}region_point_set_item;

typedef struct offset_system_str //R63
{
	unsigned char mask_scale:1,
		      	rest:7;

	unsigned char scale; /*OP, INTEGER (0..15)*/
	
	unsigned char offset_choice;/*1-2*/
	/*offset_choice: 1*/
	node_listXY_item xy;
	/*offset_choice: 2*/
	node_listLL_item ll;
}offset_system_item;

typedef struct geometric_projection_str //R63
{
	unsigned char mask_extent:1,
				mask_lane_width:1,
				mask_regional:1,
				rest:5;
	
	short direction;					/*BIT STRING SIZE(16)*/ 
	unsigned char extent; 		/*OP, ENUMERATED (0..15)*/
	unsigned short lane_width;/*OP, INTEGER (0..32767) -- units of 1 cm*/
	circle_item circle;

	unsigned char regional_num;
	regional_extension_item regional[4]; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-EventDescription}} OPTIONAL*/
}geometric_projection_item;

typedef struct valid_region_str //R63
{
	unsigned char mask_extent:1,
		      rest:7;

	short direction;			/*BIT STRING SIZE(16)*/ 
	unsigned char extent; /*OP, ENUMERATED (0..5)*/
	
	unsigned char area_choice;/*1-3*/
	/*area_choice: 1*/
	shape_point_set_item shape_point_set;
	/*area_choice: 2*/
	circle_item circle;
	/*area_choice: 3*/
	region_point_set_item region_point_set;
}valid_region_item;

typedef struct geographical_path_str //R63
{
	unsigned char mask_name:1,
		      mask_id:1,
		      mask_anchor:1,
		      mask_lane_width:1,
		      mask_directionality:1,
		      mask_closed_path:1,
		      mask_direction:1,
		      mask_description:1;
	unsigned char mask_regional:1,
		      rest:7;

	char name[64]; 								/*OP, IA5String (SIZE(1..63))*/
	road_segment_reference_id_item id; /*OP*/
	position3d_item anchor; 			/*OP*/
	unsigned short lane_width;		/*OP, INTEGER (0..32767) -- units of 1 cm*/
	unsigned char directionality;	/*OP, ENUMERATED (0..3)*/

	char closed_path;/*OP, BOOLEAN*/
	short direction; /*OP, BIT STRING (SIZE (16))*/

	unsigned char description_choice;/*1-3*/
	/*description_choice: 1*/
	offset_system_item path;
	/*description_choice: 2*/
	geometric_projection_item geometry;
	/*description_choice: 3*/
	valid_region_item old_region;

	unsigned char regional_num;
	regional_extension_item regional[4]; /*SEQUENCE (SIZE(1..4)) OF RegionalExtension {{REGION.Reg-EventDescription}} OPTIONAL*/
}geographical_path_item;

typedef struct iti_scodes_and_text_str //R63
{
	unsigned char itis_item_choice;/*1-2*/
	/*choice: 1*/
	unsigned int itis;/*INTEGER (0..65565)*/
	/*choice: 2*/
	unsigned char text[500];/*IA5STRING (size(1..500))*/
}iti_scodes_and_text_item;

typedef struct content_str //R63
{
	unsigned char item_choice;/*1-2*/
	/*choice: 1*/
	unsigned int itis;/*INTEGER (0..65565)*/
	/*choice: 2*/
	unsigned char text[16];/*IA5STRING	(size(1..16))*/
}content_item;

typedef struct traveler_data_frame_str //R63
{
	unsigned char mask_start_year:1,
				mask_url:1,
				rest:6;

	unsigned char ssp_tim_rights; /*INTEGER (0..31)*/ 
	unsigned char frame_type;/*ENUMERATED (0..3)*/

	unsigned char msg_id_choice;
	/*msgid_choice:0, OCTET STRING (size(2))*/
	unsigned char further_info_id[2];
	/*msgid_choice:1*/	
	road_sign_id_item road_sign_id;

	unsigned short start_year; 		/*OPTIONAL, INTEGER (0..4095)*/ 
	unsigned int start_time; 			/*INTEGER (0..527040)*/
	unsigned short duraton_time;	/*INTEGER (0..32000)*/
	unsigned char priority; 			/*INTEGER (0..7)*/
	unsigned char ssp_location_rights; /*INTEGER (0..31)*/ 

	unsigned char regions_num;
	geographical_path_item *regions; /*SEQUENCE (SIZE(1..16)) OF GeographicalPath*/

	unsigned char ssp_msg_rights_1; /*INTEGER (0..31)*/ 
	unsigned char ssp_msg_rights_2; /*INTEGER (0..31)*/ 

	unsigned char content_choice;/*1-5*/
	/*contentchoice:1*/
	unsigned char advisory_num;
	iti_scodes_and_text_item *advisory; /*SEQUENCE (size(1..100))*/
	/*contentchoice:2*/
	unsigned char work_zone_num;
	content_item *work_zone; 						/*SEQUENCE (size(1..16))*/
	/*contentchoice:3*/
	unsigned char generic_sign_num;
	content_item *generic_sign; 				/*SEQUENCE (size(1..16))*/
	/*contentchoice:4*/
	unsigned char speed_limit_num;
	content_item *speed_limit; 					/*SEQUENCE (size(1..16))*/
	/*contentchoice:5*/
	unsigned char exit_service_num;
	content_item *exit_service; 				/*SEQUENCE (size(1..16))*/

	unsigned char url[15];							/*OPTIONAL, IA5STRING (size(1..15))*/
}traveler_data_frame_item;

typedef struct approach_or_lane_str //R63
{
	unsigned char approach_or_lane_choice;

	unsigned char approach; /*INTEGER (0..15)*/ 
	unsigned char lane; 		/*INTEGER (0..255)*/
}approach_or_lane_item;

typedef struct sample_str //R63
{
	unsigned char sample_start; /*INTEGER (0..255)*/ 
	unsigned char sample_end;   /*INTEGER (0..255)*/
}sample_item;

typedef struct snapshot_time_str //R63
{
	unsigned char speed1;  /*INTEGER (0..31)*/ 
	unsigned char time1;   /*INTEGER (0..61)*/
	unsigned char speed2;  /*INTEGER (0..31)*/ 
	unsigned char time2;   /*INTEGER (0..61)*/
}snapshot_time_item;

typedef struct snapshot_distance_str //R63
{
	unsigned short distance1;  /*INTEGER (0..1023)*/ 
	unsigned char  speed1;     /*INTEGER (0..31)*/
	unsigned short distance2;  /*INTEGER (0..1023)*/ 
	unsigned char  speed2;     /*INTEGER (0..31)*/
}snapshot_distance_item;

typedef struct requestor_type_str //R63
{
	unsigned char   mask_subrole:1,
					mask_request:1,
					mask_iso3883:1,
					mask_hpmsType:1,
					mask_regional:1,
					rest:3;
	
	unsigned char role;				/*ENUMERATED (0..22)*/
	unsigned char subrole;		/*ENUMERATED (0..15)*/
	unsigned char request;		/*ENUMERATED (0..15)*/
	unsigned char iso3883;		/*INTEGER (0..100)*/
	unsigned char hpmsType;		/*ENUMERATED (0..15)*/
}requestor_type_item;

typedef struct vehicle_status_request_str //R63
{
	unsigned char mask_subType:1,
				  mask_sendOnLessThenValue:1,
				  mask_sendOnMoreThenValue:1,
				  mask_sendAll:1,
				  rest:4;

	unsigned char dataType; 	/*ENUMERATED (0..28)*/
	unsigned char subType; 		/*INTEGER (1..15)*/
	int sendOnLessThenValue; 	/*INTEGER (-32767..32767)*/
	int sendOnMoreThenValue; 	/*INTEGER (-32767..32767)*/
	char sendAll; 						/*BOOLEAN; RANGE(0..1)*/
} vehicle_status_request_item;

typedef struct signal_requester_info_str //R63
{
	unsigned char   mask_role:1,
					mask_type_data:1,
					rest:6;

	vehicle_id_item id;
	unsigned char request;					/*INTEGER (0..255)*/
	unsigned char sequence_number;	/*INTEGER (0..127)*/
	unsigned char role;							/*ENUMERATED (0..22)*/	

	requestor_type_item type_data;

}signal_requester_info_item;

typedef struct intersection_access_point_str //R63
{
	unsigned char intersection_access_point_choice;

	unsigned char lane; 			/*INTEGER (0..255)*/
	unsigned char approach; 	/*INTEGER (0..15)*/ 
	unsigned char connection; /*INTEGER (0..255)*/ 
}intersection_access_point_item;

typedef struct signal_status_package_str //R63
{
	unsigned char   mask_requester:1,
					mask_outboundOn:1,
					mask_minute:1,
					mask_second:1,
					mask_duration:1,
					mask_regional:1,
					rest:2;
	
	signal_requester_info_item requester;
	intersection_access_point_item inboundOn;
	intersection_access_point_item outboundOn;
	unsigned int  minute;		/*INTEGER (0..527040)*/
	unsigned int  second;		/*INTEGER (0..65535)*/
	unsigned int  duration;	/*INTEGER (0..65535)*/
	unsigned char status;		/*ENUMERATED (0..7)*/	

}signal_status_package_item;

typedef struct signal_status_str //R63
{
	unsigned char sequence_number;		/*INTEGER (0..127)*/
	intersection_reference_id_item id;

	unsigned char sig_status_num;
	signal_status_package_item *sig_status;

}signal_status_item;

typedef struct signal_request_str //R63
{
	unsigned char   mask_outBoundLane:1,
					mask_regional:1,
					rest:6;
	
	intersection_reference_id_item id;
	unsigned char request_ID;		/*INTEGER (0..255)*/
	unsigned char request_type;	/*ENUMERATED (0..3)*/
	intersection_access_point_item inBoundLane;
	intersection_access_point_item outBoundLane;
	
}signal_request_item;

typedef struct signal_request_package_str //R63
{
	unsigned char   mask_minute:1,
					mask_second:1,
					mask_duration:1,
					mask_regional:1,
					rest:4;
	
	signal_request_item request;
	
	unsigned int minute; 		/*INTEGER (0..527040)*/
	unsigned int second;		/*INTEGER (0..65535)*/
	unsigned int duration;	/*INTEGER (0..65535)*/

}signal_request_package_item;

typedef struct requestor_position_vector_str //R63
{
	unsigned char   mask_heading:1,
					mask_speed:1,
					rest:6;
	
	position3d_2_item position;
	unsigned int heading; 		/*INTEGER (0..28800)*/
	transmission_speed_item speed;

}requestor_position_vector_item;

typedef struct requestor_description_str //R63
{
	unsigned char   mask_type:1,
					mask_position:1,
					mask_name:1,
					mask_routeName:1,
					mask_transitStatus:1,
					mask_transitOccupancy:1,
					mask_transitSchedule:1,
					mask_regional:1;

	vehicle_id_item id;
	requestor_type_item type;
	requestor_position_vector_item position;
	unsigned char name[64];				 /*IA5STRING (size(1..63))*/
	unsigned char routeName[64];   /*IA5STRING (size(1..63))*/
	unsigned char transitStatus;   /*BIT STRING(SIZE(8))*/
	unsigned char transitOccupancy;/*ENUMERATED (0..7)*/
	  signed char transitSchedule; /*INTEGER (-122..121)*/

}requestor_description_item;

typedef struct snapshots_str //R63
{
	unsigned char   mask_safety_ext:1,
					mask_data_set:1,
					rest:6;
	
	full_position_vector_item the_position;
	vehicle_safety_extensions_item safety_ext;
	vehicle_status_item data_set;
}snapshots_item;

typedef struct propelled_information_str //R63
{
	unsigned char propelled_information_choice;
	unsigned char human;	/*ENUMERATED (0..5)*/
	unsigned char animal;	/*ENUMERATED (0..3)*/
	unsigned char motor;	/*ENUMERATED (0..5)*/
	
}propelled_information_item;


#endif
