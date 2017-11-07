/* Copyright (C) Industrial Technology Research Institute - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Po-Chun, Kang <ttt710516@itri.org.tw>
 */

#ifndef	__DOT3_COMMON_H__
#define	__DOT3_COMMON_H__

#include <stdint.h>
#include <sys/time.h>

/** EID definition */
#define EID_TX_POWER	                        4
#define EID_2D_LOCATION                         5
#define EID_3D_LOCATION                         6
#define EID_ADVERTISER_ID                       7
#define EID_PSC                                 8
#define EID_IPV6_ADDR                           9
#define EID_IPV6_PORT                           10
#define EID_PROVIDER_MAC                        11
#define EID_EDCA_SET                            12
#define EID_SECOND_DNS                          13
#define EID_GATEWAY_MAC                         14
#define EID_CHANNEL_NUMBER                      15
#define EID_DATA_RATE                           16
#define EID_REPEAT_RATE                         17
#define EID_RCPI_THRESHOLD                      19
#define EID_WSA_COUNT_THRESHOLD                 20
#define EID_CHANNEL_ACCESS                      21
#define EID_WSA_COUNT_THRESHOLD_INTERVAL        22
#define EID_CHANNEL_LOAD                        23


/** WSA bit mask definition */
#define BITMASK_REPEAT_RATE                     (1 << 0)
#define BITMASK_2D_LOCATION                     (1 << 1)
#define BITMASK_3D_LOCATION                     (1 << 2)
#define BITMASK_ADVERTISER_ID                   (1 << 3)

#define BITMASK_PSC                             (1 << 4)
#define BITMASK_IPV6_ADDR                       (1 << 5)
#define BITMASK_IPV6_PORT                       (1 << 6)
#define BITMASK_PROVIDER_MAC                    (1 << 7)
#define BITMASK_RCPI_THRESHOLD                  (1 << 8)
#define BITMASK_WSA_COUNT_THRESHOLD             (1 << 9)
#define BITMASK_WSA_COUNT_THRESHOLD_INTERVAL    (1 << 10)

#define BITMASK_EDCA_SET                        (1 << 11)
#define BITMASK_CHANNEL_ACCESS                  (1 << 12)

#define BITMASK_SECOND_DNS                      (1 << 13)
#define BITMASK_GATEWAY_MAC                     (1 << 14)


/** General size definition */
#define PSC_SIZE                31
#define IPV6_SIZE               16
#define MAC_SIZE                6
#define LOCATION_2D_SIZE        8
#define LOCATION_3D_SIZE        10
#define ADVERTISER_ID_SIZE      32
#define DEFAULT_PRIORITY        32 
#define MAX_PRIORITY            63
#define WSA_MAX_ENTRY           8
#define WSA_NUM                 4
#define ENTRY_SIZE              256
#define WSA_PSID                135
#define WSA_MAX_SIZE            1450
#define WSM_MAX_SIZE            1480


/* WME handle definiton */
typedef uint16_t wme_handle_t;


/** Return codes of dot3 APIs */
typedef enum WME_ERR {
    ACCEPT = 0,
    //INVALID_VERSION,
    //INVALID_ROLE,
    INVALID_PARAMETER,
    INVALID_HANDLE,
    WME_SYS_ABORT, 
    UNSPEC,

    //CONFLIT_INDEX,
    WME_CONFLIT_PSID,
    OUT_OF_MEM,
    OUT_OF_ENTRY,
    //INDEX_NOT_FOUND,
    ENTRY_NOT_FOUND,
    //ENTRY_NOT_MATCH,
    INDEX_FULL,
    MIB_NOT_FOUND,
    NO_DEVICE,
    NO_CHANNEL,
    //BUSY_CHANNEL,
    TA_EXISTED,  /* every channel can only have a TA service */
    WME_ERR_MAX,
} wme_err_code_t;


/** Radio number definition */
typedef enum RADIO_NUM {
    RADIO_RESV = 0,
    RADIO_0 = 1,
    RADIO_1 = 2,
    RADIO_MAX = 3,
} wme_radio_num_t;


/** Type of WME service status */
typedef enum SERVICE_STATUS {
    STATUS_PENDING = 0,
    STATUS_SATISFIED = 1,
    STATUS_PARTIALLY_SATISFIED = 2,
    STATUS_AVAILABLE = 0,
    STATUS_ACTIVE = 1,
} wme_serv_status_t;


/* TODO: will not use */
/** WME service status definition */
enum WME_ACTION {
	ADD,
	DELETE,
	CHANGE,
};


/** Type of WME event notification */
typedef enum WME_EVENT {
    EVENT_SERVICE,
    EVENT_CHANNEL,
} wme_event_t;


/** Type of WME mib entries */
typedef enum MIB_TYPE {
    PROVIDER_ENTRY,
    USER_ENTRY,
    CHANNEL_ENTRY,
    WSM_ENTRY,
    AVAILABLE_ENTRY,
} wme_mib_t;


/** Type of WME configuration ID */
typedef enum WSA_CFG_ID {
    CFG_REPEAT_RATE,
    CFG_ADVERTISEMENT_ID,
    //CFG_LOCATION,
    //CFG_EDCA,
    CFG_ROUTER_LIFETIME,
    CFG_IP_PREFIX,
    CFG_IP_PREFIX_LEN,
    CFG_DEFAULT_GATEWAY,
    CFG_PRIMARY_DNS,
    CFG_SECONDARY_DNS,
    CFG_GATEWAY_MAC
} wme_cfg_id_t;


/** Type of WME event reason */
typedef enum WME_REASON {
    REASON_SERVICE_AVAILABLE,
    REASON_SERVICE_UNAVAILABLE,
    REASON_CHANNEL_AVAILABLE,
    REASON_CHANNEL_UNAVAILABLE,
} wme_event_reason_t;


/** Receiving WSM defintion */
typedef struct in_wsm
{
    unsigned char       version;
    unsigned char       radio_number;    
    unsigned char       channel_number;
    unsigned char       data_rate;
    signed char         txpwr_level;
    unsigned char       user_priority;  /* Not supporting now */
    unsigned int        psid;
    unsigned char       src_mac[MAC_SIZE];
    struct {
        unsigned char   channel:1,
                        rate:1,
                        power:1,
                        load:1,
                        reserved:4;
    } extensions;
    signed char         rssi;
    unsigned char       real_channel;
    unsigned char       data[WSM_MAX_SIZE];
    unsigned short      length;
}wme_in_wsm_t;


/** Sending WSM defintion */
typedef struct out_wsm
{
    unsigned char       radio_number;
    unsigned char       channel_number;
    unsigned char       data_rate;
    signed char         txpwr_level;
    unsigned char       user_priority;
    unsigned int        psid;
    unsigned long long  expiry_time;  /* Not supporting now */
    unsigned char       dest_mac[MAC_SIZE];
    struct {
        unsigned char   channel:1,
                        rate:1,
                        power:1,
                        load:1,
                        reserved:4;
    } extensions;
    unsigned char       data[WSM_MAX_SIZE];
    unsigned short      length;
} wme_out_wsm_t;


/** TODO:change SLOT -> ACCESS */
/** Type of Slot(Channel Interval) */
typedef enum ACCESS_SLOT {
	SLOT_BOTH = 0,
	SLOT_SCH = 1,
	SLOT_CCH = 2,  
	SLOT_MAX,
} wme_channel_access_t;


/** Provider service mib information defintion */
typedef struct mib_provider_service_info{
    unsigned char       dest_mac[MAC_SIZE];	
    unsigned char       wsa_type;  /* enum WSA_TYPE */
    unsigned int        psid;
    unsigned char       psc[PSC_SIZE];
    unsigned char       psc_len;
    unsigned char       service_priority;  /* 0 ~ 63 */
    unsigned char       channel_access;  /* enum ACCESS_SLOT */  
    unsigned char       best_available;  /* true/false */
    unsigned char       operating_class;
    unsigned char       service_channel;
    unsigned char       wsa_channel;
    unsigned char       repeat_rate;  /* 0 ~ 255 per 5s */
    unsigned char       ip_service;  /* true/false */
    unsigned char       ipv6_addr[IPV6_SIZE];
    unsigned short      service_port;
    unsigned char       provider_mac[MAC_SIZE];
    unsigned char       rcpi_threshold;
    unsigned char       wsa_count_threshold;
    unsigned char       wsa_count_threshold_interval;
    unsigned short      signature_lifetime;
    unsigned char       service_status;

    /* internal use */
    unsigned char       radio_number;  /* RADIO_0 or RADIO_1 */
    unsigned short      certificate_id;
    unsigned char       channel_interval;
    unsigned char       channel_available;
    unsigned char       wsa_id;
    unsigned int        info_element_indicator;
} wme_provider_serv_mib_t;


/** Provider service operation information */
typedef struct provider_info{
    unsigned char       action;
    unsigned char       dest_mac[MAC_SIZE];  /* No support */
    unsigned char       wsa_type;  /* enum WSA_TYPE */
    unsigned int        psid;
    unsigned char       psc[PSC_SIZE];
    unsigned char       psc_len;
    unsigned char       service_priority;
    unsigned char       radio_number;  /* RADIO_0 or RADIO_1 */
    unsigned char       service_channel;
    unsigned char       wsa_channel;  /* NO Support, only 178 now */
    unsigned char       channel_access;  /* enum ACCESS_SLOT */
    unsigned char       repeat_rate;
    unsigned char       ip_service;
    unsigned char       ipv6_addr[IPV6_SIZE];
    unsigned short      service_port;
    unsigned char       provider_mac[MAC_SIZE];
    unsigned char       rcpi_threshold;  /* no use now */
    unsigned char       wsa_count_threshold;
    unsigned char       wsa_count_threshold_interval;
    unsigned int        info_element_indicator;
    unsigned short      certificate_id;
    unsigned short      signature_lifetime;  /* 10 ~ 30000ms */
} wme_provider_info_t;


/** User service mib information defintion */
typedef struct mib_user_service_info{
    unsigned char       request_type;
    unsigned int        psid;
    unsigned char       psc[PSC_SIZE];
    unsigned char       psc_len;
    unsigned char       service_priority;
    unsigned char       wsa_type;
    unsigned char       src_mac[MAC_SIZE];
    unsigned char       advertiser_id[ADVERTISER_ID_SIZE];
    unsigned char       advertiser_id_len;
    unsigned char       operating_class;
    unsigned char       channel_number;
    unsigned char       link_quality;
    unsigned char       immediate_access;  /* Only support 0(Channel switch) or 255(Continuous) */
    unsigned char       service_status;

    /* internal use */
    //unsigned char	base_available;  /* if the req assign channel = 0, set to True */
    unsigned char       radio_number;  /* RADIO_0 or RADIO_1 */	
    unsigned char       assigned_channel;  /* the real assigned channel */
    unsigned char       channel_interval;  /* the real assigned channel interval */
} wme_user_serv_mib_t;


/** Type of user service request modes */
typedef enum REQUEST_ACCESS {
    ACCESS_ON_MATCH = 0,
    ACCESS_NO_SCH = 1,
    ACCESS_MAX,
} wme_user_access_t;


/** Type of WSAs */
typedef enum WSA_TYPE{
    WSA_SECURE = 1,
    WSA_UNSECURE = 2,
    WSA_BOTH = 3,
    WSA_ANY = 4,
    WSA_MAX,
} wme_wsa_t;


/** User service operation information */
typedef struct user_info {
	unsigned char		action;
	unsigned char		request_type;  /* enum REQUEST_ACCESS */
	unsigned int		psid;
	unsigned char		psc[PSC_SIZE];
	unsigned char		psc_len;
	unsigned char		service_priority;
	unsigned char		wsa_type;
	unsigned char		radio_number;  /* RADIO_0 or RADIO_1 */	
	unsigned char		channel_number;
	unsigned char		src_mac[MAC_SIZE];
	unsigned char		advertiser_id[ADVERTISER_ID_SIZE];
	unsigned char		advertiser_id_len;	
	unsigned char		link_quality;  /* No support */
	unsigned char		immediate_access;  /* Only support 0(Channel switch) or 255(Continuous) */
} wme_user_info_t;


/** Wsm mib information defintion */
typedef struct mib_wsm_service_info{
	unsigned int 		psid;
} wme_wsm_serv_mib_t;


/** Wsm service operation information */
typedef struct wsm_info
{
    unsigned char	action;
    unsigned int	psid;
} wme_wsm_info_t;


/** Type of Slot(Channel Interval) */
typedef enum TIME_SLOT {
    TIME_SLOT_0 = 1,
    TIME_SLOT_1 = 2,
    TIME_SLOT_BOTH = 3,
    TIME_SLOT_MAX,
} wme_time_slot_t;


/** Channel service mib information defintion */
typedef struct mib_channel_service_info {
    unsigned char       service_priority;
    unsigned char       operating_class;
    unsigned char       channel_number;
    unsigned char       time_slot;  /* enum TIME_SLOT */
    unsigned char       service_status;

    /* internal use */
    unsigned char       radio_number;  /* RADIO_0 or RADIO_1 */
    unsigned char       channel_interval;  /* the real assigned channel interval */	
} wme_channel_serv_mib_t;


/** Channel service operation information */
typedef struct channel_info
{
    unsigned char       action;
    unsigned char       service_priority;
    unsigned char       radio_number;  /* RADIO_0 or RADIO_1 */		
    unsigned char       channel_number;
    unsigned char       time_slot;  /* enum TIME_SLOT */
} wme_channel_info_t;


/** Type of EDCA levels */
typedef enum EDCA_LEVEL {
	EDCA_LEVEL_BK = 0,
	EDCA_LEVEL_BE,
	EDCA_LEVEL_VO,
	EDCA_LEVEL_VI,
	EDCA_LEVEL_MAX,
} wme_edca_level_t;


/** EDCA information definition */
typedef struct mib_edca
{
	//__u8	aci;
	unsigned char       acm;	
	unsigned char       aifsn;
	unsigned char       cw_min;
	unsigned short      cw_max;
	unsigned short      txop_limit;
} wme_mib_edca_t;


/** Mib service information definition used for available service */
typedef struct mib_service_info {
    unsigned int        psid;
    unsigned char       psc_len;
    unsigned char       psc[PSC_SIZE];
    unsigned char       ipv6_addr[IPV6_SIZE];
    unsigned short      service_port;	
    unsigned char       provider_mac[MAC_SIZE];
    unsigned char       rcpi_threshold;
    unsigned char       wsa_count_threshold;
    unsigned char       wsa_count_threshold_interval;
    unsigned int        info_element_indicator;

    /* Internal */
    unsigned char 	channel_index;
} wme_service_mib_t;


/** Mib channel information definition used for available service */
typedef struct mib_channel_info{
    unsigned char       operating_class;
    unsigned char       channel_number;
    unsigned char       adaptable;  /* true/false */	
    unsigned char       data_rate;
    signed char         tx_power;
    unsigned char       channel_access;
    struct mib_edca     edca[EDCA_LEVEL_MAX];
    unsigned int        info_element_indicator;	
} wme_channel_mib_t;


/** Available service mib information definition */
typedef struct mib_available_service_info{
    unsigned char       wsa_type;
    unsigned char       src_mac[MAC_SIZE];
    unsigned char       service_specific_permissions;  /* relation to 1609.2 */
    unsigned char       service_priority;
    unsigned char       rcpi;  /* the last received wsa */
    unsigned char       link_quality;
    unsigned char       service_status;

    /* WSA Header Info */
    unsigned char       repeat_rate;	
    signed int          latitude;
    signed int          longitude;
    signed int          elevation;
    unsigned char       advertiser_id_len;
    unsigned char       advertiser_id[ADVERTISER_ID_SIZE];
    unsigned int        info_element_indicator;

    /* WSA Service Info */
    wme_service_mib_t   service_info;

    /* WSA Channel Info */
    wme_channel_mib_t   channel_info;

    /* WRA Info */
    unsigned short      router_life_time;
    unsigned char       ip_prefix[IPV6_SIZE];
    unsigned char       prefix_len;
    unsigned char       default_gateway[IPV6_SIZE];
    unsigned char       primary_dns[IPV6_SIZE];
    unsigned char       secondary_dns[IPV6_SIZE];
    unsigned char       gateway_mac[MAC_SIZE];

    /* internal */
    unsigned char       wsa_id;
    unsigned char       content_count;
    unsigned char       channel_mapping;  /* record the mapping between service info and channel info */
    unsigned char       wsa_count;
    unsigned char       interval_counter;  /* count the passing interval */
} wme_available_serv_mib_t;


/** Mib entry information definition */
typedef struct mib_info
{
    unsigned char       entry_type;
    unsigned char       next;  /* true: fetch the next available index, false: fetch the index */
    short               entry_index;
    union {
        wme_provider_serv_mib_t    provider_entry;
        wme_user_serv_mib_t         user_entry;
        wme_channel_serv_mib_t      channel_entry;
        wme_wsm_serv_mib_t          wsm_entry;
        wme_available_serv_mib_t    available_entry;
        
        //struct mib_channel_info channel_entry;
        //struct mib_capabilities_info capabilities_entry;
        //struct mib_switching_info switching_entry;
        //struct mib_channelset_info channelset_entry;
        //struct mib_edcacch_info edcacch_entry;
        //struct mib_edcasch_info edcasch_entry;
        //struct mib_txprofile_info txprofile_entry;
        //struct mib_timinginfo_info timinginfo_entry;
    } entry_value;
} wme_mib_entry_t;


/** WSA configuration information definition */
typedef struct wsa_cfg_info
{
    signed int          latitude;
    signed int          longitude;
    signed int          elevation;
    unsigned char       advertiser_id[ADVERTISER_ID_SIZE];
    unsigned char       advertiser_id_len;

    unsigned short      router_lifetime;
    unsigned char       ip_prefix[IPV6_SIZE];
    unsigned char       prefix_len;
    unsigned char       default_gateway[IPV6_SIZE];
    unsigned char       primary_dns[IPV6_SIZE];
    unsigned char       secondary_dns[IPV6_SIZE];
    unsigned char       gateway_mac[MAC_SIZE];
    unsigned int        info_element_indicator;  /* for optional field, defined by BITMASK_XXX  */
} wme_wsa_cfg_t;


/** TODO: the follows definitin need to be done!!!!! */
enum WAVE_DEV {
	DEV0_BOTH,
	DEV0_CCH,
	DEV0_SCH,
	DEV1_BOTH,
	DEV1_CCH,
	DEV1_SCH,
	DEV_MAX,
};

enum WAVE_OP{
	OP_EXTEND = 1,
	OP_SWITCH,
	OP_MAX,
};

struct edca {
	//	__u8	aci;
	unsigned char		aifsn;
	unsigned char		ecw_min;
	unsigned char		ecw_max;
	unsigned short	txop_limit;
};

struct dev_info {
	//unsigned char dev_id;
	unsigned char		channel_number;
	//unsigned char	mode;  /* OP_EXTEND¡BOP_SWITCH */
	//unsigned char	disable;
	unsigned char		data_rate;
	unsigned char		tx_power;
	unsigned char		adapter;  /* true/false */
	struct edca		edca[EDCA_LEVEL_MAX];
};

struct event_channel
{
	unsigned char channel;
	unsigned char	access;  /* enum ACCESS_SLOT */  
};

struct event_message
{
	unsigned char	event;
	unsigned char	reason;
	union {
		struct 	event_channel channel;
		struct mib_available_service_info service;
	} info;
};

#endif	/* __DOT3_COMMON_H__ */