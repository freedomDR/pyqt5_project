#ifndef __DOT2_H__
#define __DOT2_H__

#define SEC_BSM_PSID 0x20
#define SEC_WSA_PSID 0x87

typedef enum {
  DOT2_OK = 0,
  DOT2_FAILURE = 1,
  DOT2_INVALID_KEY_PAIR = 2,
  DOT2_KEYS_DO_NOT_MATCH = 3,
  DOT2_BAD_PARAMETERS = 4,

  // result codes for sec_sign
  DOT2_INCORRECT_INPUT = 5,
  DOT2_NO_CERT_PROVIDED = 6,
  DOT2_NO_PUBLIC_KEY_PROVIDED = 7,
  DOT2_NOT_ENOUGH_INFO_TO_CONTRUCT_CERT_CHAIN = 8,
  DOT2_NO_TRUST_ANCHOR = 9,
  DOT2_CERT_CHAIN_TOO_LONG_FOR_IMPLEMENTATION = 10,
  DOT2_NOT_CRYPTOGRAPHICALLY_VALID = 11,
  DOT2_UNKNOWN_CRYPTOGRAPHICALLY_VALID = 12,
  DOT2_INCONSISTENT_PERMISSIONS_IN_CERT_CHIAN = 13,
  DOT2_REVOKED = 14,
  DOT2_DUBIOUS = 15,
  DOT2_UNSUPPORTED_CRITICAL_INFO_FIELDS = 16,
  DOT2_INVALID_ENCODING = 17,
  DOT2_CURRENT_TIME_BEFORE_CERT_VALIDITY_PERIOD = 18,
  DOT2_CURRENT_TIME_AFTER_CERT_VALIDITY_PERIOD = 19,
  DOT2_EXPIRY_TIME_BEFORE_CERT_VALIDITY_PERIOD = 20,
  DOT2_EXPIRY_TIME_AFTER_CERT_VALIDITY_PERIOD = 21,
  DOT2_INVALID_GENERATION_LOCATION = 22,
  DOT2_INCONSISTENT_PERMISSIONS_IN_CERT = 23,
  DOT2_INCORRECT_REQUESTED_CERT_CHAIN_LENGTH_FOR_SECURITY_PROFILE = 24,
  DOT2_INCORRECT_REQUESTED_CERT_CHAIN_LENGTH_FOR_IMPLEMENTATION = 25,

  // result codes for sec_encrypt
  DOT2_FAIL_ON_SOME_CERTS = 26,
  DOT2_FAIL_ON_ALL_CERTS = 27,

  // result codes for sec_sec_data_preprocessing
  DOT2_UNKNOWN_CERT = 28,
  DOT2_INCONSISTENT_PSID = 29,

  // result codes for sec_sec_data_verification
  DOT2_INCONSISTENT_INPUT_PARAMETERS = 30,
  DOT2_PARSING_INVALID_INPUT = 31,
  DOT2_PARSING_UNSUPPORTED_CRITICAL_INFO_FIELDS = 32,
  DOT2_PARSING_CERT_NOT_FOUND = 33,
  DOT2_PARSING_GENERATION_TIME_NOT_AVAILABLE = 34,
  DOT2_PARSING_GENERATION_LOCATION_NOT_AVAILABLE = 35,
  DOT2_CERT_CHAIN_NOT_ENOUGH_INFO_TO_CONSTRUCT_CHAIN = 36,
  DOT2_CERT_CHAIN_CHAIN_ENDED_AT_UNTRUSTED_ROOT = 37,
  DOT2_CERT_CHAIN_CHAIN_WAS_TOO_LONG_FOR_IMPLEMENTATION = 38,
  DOT2_CERT_CHAIN_CERT_REVOKED = 39,
  DOT2_CERT_CHAIN_OVERDUE_CRL = 40,
  DOT2_CERT_CHAIN_INCONSISTENT_EXPIRY_TIMES = 41,
  DOT2_CERT_CHAIN_INCONSISTENT_START_TIMES = 42,
  DOT2_CERT_CHAIN_INCONSISTENT_CHAIN_PERMISSIONS = 43,
  DOT2_CRYPTO_VERIFICATION_FAILURE = 44,
  DOT2_CONSISTENCY_FUTURE_CERT_AT_GENERATION_TIME = 45,
  DOT2_CONSISTENCY_EXPIRED_CERT_AT_GENERATION_TIME = 46,
  DOT2_CONSISTENCY_EXPIRY_DATE_TOO_EARLY = 47,
  DOT2_CONSISTENCY_EXPIRY_DATE_TOO_LATE = 48,
  DOT2_CONSISTENCY_GENERATION_LOCATION_OUTSIDE_VALIDITY_REGION = 49,
  DOT2_CONSISTENCY_NO_GENERATION_LOCATION = 50,
  DOT2_CONSISTENCY_UNAUTHORIZED_PSID = 51,
  DOT2_INTERNAL_CONSISTENCY_EXPIRY_TIME_BEFORE_GENERATION_TIME = 52,
  DOT2_INTERNAL_CONSISTENCY_EXDATAHASH_DOESNT_MATCH = 53,
  DOT2_INTERNAL_CONSISTENCY_NO_EXDATAHASH_PROVIDED = 54,
  DOT2_INTERNAL_CONSISTENCY_NO_EXDATAHASH_PRESENT = 55,
  DOT2_LOCAL_CONSISTENCY_PSIDS_DONT_MATCH = 56,
  DOT2_LOCAL_CONSISTENCY_CHAIN_WAS_TOO_LONG_FOR_SDEE = 57,
  DOT2_RELEVANCE_GENERATION_TIME_TOO_FAR_IN_PAST = 58,
  DOT2_RELEVANCE_GENERATION_TIME_TOO_FAR_IN_FUTURE = 59,
  DOT2_RELEVANCE_EXPIRY_TIME_IN_PAST = 60,
  DOT2_RELEVANCE_GENERATION_LOCATION_TOO_DISTANT = 61,
  DOT2_RELEVANCE_REPLAYED_SPDU = 62,
  DOT2_RELEVANCE_CERT_EXPIRED = 63,

  // result codes for sec_decrypt
  DOT2_NO_DECRYPTION_KEY_AVAILABLE = 64,
  DOT2_COULDNT_DECRYPT_KEY = 65,
  DOT2_COULDNT_DECRYPT_DATA = 66,
  DOT2_INVALID_FORM_FOR_PLAINTEXT = 67,

  // result codes for ssme_certificate_info
  DOT2_CERT_NOT_FOUND = 68,
  DOT2_MULTIPLE_CERTS_IDENTIFIED = 69,
  DOT2_NOT_YET_VERIFIED = 70,
  DOT2_VERIFIED_AND_TRUSTED = 71,

  // result codes for ssme_add_trust_anchor
  DOT2_CERT_DID_NOT_VERIFY = 72,

  // result codes for ssme_verify_certificate
  DOT2_VERIFIED = 73,
  
  // result codes for ssme_sec_replay_detection
  DOT2_REPLAY = 74,
  DOT2_NOT_REPLAY = 75,

  // others
  DOT2_MSG_TOO_LONG = 76,
  DOT2_GPS_LOCATION_NOT_AVAILABLE = 77,
  DOT2_INVALID_SIGNER_FOR_SIGNED_MSG = 78,
  DOT2_UNSUPPORTED_CERT_CHAIN_LEN = 79,
  DOT2_PARSED_CERT_LENGTH_NOT_MATCH = 80,
  DOT2_UNSUPPORTED_FIELDS = 81,
  DOT2_NO_SPACE_FOR_PUB_CERT = 82,
  RESERVED_RET_CODE
} dot2_return_code_t;

typedef enum {
  ECDSA_NISTP256_WITH_SHA256 = 0,
  ECDSA_BRAINPOOLP256R1_WITH_SHA256 = 1,
  ECIES_NISTP256 = 2,
  ECIES_BRAINPOOLP256R1 = 3,
  RESERVED_EC_ALGORITHM			// for checking validity
} dot2_ec_algorithms_t;

typedef enum {
  AES_128_CCM = 0,
  RESERVED_SYM_ALGORITHM		// for checking validity
} dot2_sym_algorithms_t;

typedef enum {
  ISOLATED_KEY = 0,
  RECONSTRUCT_SEED = 1,
} private_key_type_t;

typedef enum {
  UNSECURE = 0,
  SIGNED = 1,
  ENCRYPTED = 2,
  SIGNEDCERTIFICATEREQUEST = 3,
  RESERCED_DATA_TYPE
} dot2_data_type_t;

typedef enum {
  DIGEST = 0,
  CERTIFICATE = 1,
  SELF = 2,
  RESERVED_SIGNER_TYPE
} dot2_signer_type_t;

typedef enum {
  X_ONLY = 0,
  COMPRESSED = 1,
  UNCOMPRESSED = 2,
  RESERVED_EC_POINT_CHOICE
} dot2_ec_point_choice_t;

// basic types
typedef struct {
  int latitude;
  int longitude;
  unsigned short elevation;
} _3dloc;

typedef struct {
  dot2_ec_algorithms_t alg;
  unsigned char r[65];
  unsigned char s[32];
} signature_t;

// Sec
int sec_cmh_new(unsigned int* o_cmh);
int sec_cmh_gen_key_pair(unsigned int i_cmh, dot2_ec_algorithms_t i_alg, private_key_type_t i_key_type, unsigned char* o_pub_key);
int sec_cmh_store_key_pair(unsigned int i_cmh, dot2_ec_algorithms_t i_alg, unsigned char* i_pub_key, unsigned char* i_pri_key);
int sec_cmh_store_cert(unsigned int i_cmh, unsigned int i_cert_len, unsigned char* i_cert, unsigned char* i_pri_key_material);
int sec_cmh_store_cert_and_key(unsigned int i_cmh, unsigned int i_cert_len, unsigned char* i_cert, unsigned char* i_pri_key);
int sec_cmh_del(unsigned int i_cmh);

int sec_scmh_new(dot2_sym_algorithms_t i_alg, unsigned char i_gen_flag, unsigned char* i_sym_key, unsigned int* o_scmh);
int sec_scmh_hashid8(unsigned int i_scmh, unsigned char* o_hashid8);
int sec_scmh_del(unsigned int i_scmh);

typedef struct {
  unsigned int cmh;
  unsigned int data_len;
  unsigned char* data;
  dot2_data_type_t data_type;			// RAW or Ieee1609Dot2Data
  unsigned char ext_data_hash[32];
  unsigned char ext_data_hash_alg;		// should be sha256
  unsigned int psid;
  unsigned char set_gen_time;			// true or false
  unsigned char set_gen_loc;			// true or false
  unsigned int exp_time;			// time duration in ms
  dot2_signer_type_t signer_id_type;		// cert, digest, self
  char signer_id_cert_chain_len;		// 1~256, 0 for MAX
  unsigned char max_cert_chain_len;		// should be positivie integer >= 2
  dot2_ec_point_choice_t sign_with_fast_ver;	// y/w uncompressed, y/w compressed, no
  dot2_ec_point_choice_t ecpoint_format;	// uncompressed, compressed
  unsigned char use_p2p_cert_dist;		// true or false
  unsigned int sdee_id;

  unsigned int signed_data_len;			// in: max length of signed_data; 
						// out: actual length of signed_data;
  unsigned char* signed_data;
} sec_sign_t;
int sec_sign(sec_sign_t* sign_info);

typedef struct {
  unsigned int sdee_id;
  unsigned int psid;
  dot2_data_type_t content_type;
  unsigned int signed_data_len;
  unsigned char* signed_data;
  unsigned char external_data_hash[32];
  unsigned char hash_alg;
  unsigned char max_cert_chain_len;     // should be positivie integer >= 2
  unsigned char data_hash_for_replay_check[32];
  unsigned char replay_check;		// true or false
  unsigned long long gen_time;		// provided if header not contain gentime
  unsigned char gen_time_in_past_check;	// if true, reject too-old SPDU
  unsigned int validity_period;		// provided if gentimeinpast = true
  unsigned char gen_time_in_future_check;	// if true, reject future SPDU
  unsigned int future_data_period;	// provided if gentimeinfuture = true
  unsigned char expiry_check;		// if true, reject expired SPDU
  unsigned long long expiry_time;	// provided if expirycheck = true
  unsigned char gen_loc_in_cert_check;	// check genloc is inside the region of cert
  unsigned char gen_loc_dis_check;	// check genloc is inside the relevance range
  double validity_dis;			// validity range between recp and genloc (m)
  _3dloc gen_loc;			// provided if header not contain genloc
  unsigned int crl_tolerance;
  unsigned char cert_exp_check;		// if true, reject if any one of cert in chain is expired

  unsigned char signerid8[8];
  signature_t sig;
} sec_sec_data_verification_t;

typedef struct {
  unsigned int data_len;
  unsigned char* data;
  unsigned int sdee_id;
  unsigned char use_p2p_cd;

  dot2_data_type_t content_type;
  unsigned int psid;
  unsigned int ssp_len;
  unsigned char ssp[32];
  unsigned char assurance_level;
  unsigned int next_crl_time;

  unsigned int raw_data_len;			// in: max array size; out: actual data size
  unsigned char* raw_data;
  sec_sec_data_verification_t data_info;	// put extracted info here
} sec_sec_data_preprocessing_t;

int sec_sec_data_preprocessing(sec_sec_data_preprocessing_t* sec_data);
int sec_sec_data_verification(sec_sec_data_verification_t* sec_data);

typedef struct {
  unsigned int data_len;
  unsigned char* data;
  dot2_data_type_t data_type;		// RAW or Ieee1609Dot2Data
  unsigned char enc_key_type;		// static or ephemeral
  unsigned int scmh;
  unsigned int recp_cert_num;
  unsigned char* recp_cert_id_list;	// list of recp cert id (hashid8)
  unsigned char* signed_data_recp_info;
  unsigned char resp_enc_key[8];
  dot2_ec_point_choice_t ecpoint_format;

  unsigned int enc_data_len;		// in: max length of enc_data;
  					// out: actual length of enc_data;
  unsigned char* enc_data;
  unsigned int fail_cert_num;
  unsigned char* fail_cert_id_list;	// list of failed cert id (hashid8)
} sec_encrypt_t;
int sec_encrypt(sec_encrypt_t* encrypt_info);

typedef struct {
  unsigned int data_len;
  unsigned char* data;
  unsigned int cmh;
  unsigned char signed_data_recp_info[32];

  unsigned int decrypted_data_len;	// in: max length of decrypted_data; 
					// out: actual length of decrypted_data;
  unsigned char* decrypted_data;
} sec_decrypt_t;
int sec_decrypt(sec_decrypt_t* decrypt_info);

// SSME
// SSME-Sec

int sec_cmh_store_ca_cert(unsigned int i_cert_len, unsigned char* i_cert);
// for simple use
int dot2_generate_cert(unsigned int i_psid, unsigned int *o_cmh, unsigned int* o_cert_len, unsigned char* o_cert_str);

typedef struct {
  unsigned int i_cmh;
  unsigned char i_sign_flag;
  unsigned int i_msg_len;
  unsigned char* i_msg;
  unsigned int i_lifetime;	// units: ms
  unsigned int o_msg_len;
  unsigned char* o_msg;
} dot2_encode_msg_t;
int dot2_encode_msg(unsigned int i_psid, dot2_encode_msg_t* para);

typedef struct {
  unsigned char i_verify_flag;
  unsigned int i_msg_len;
  unsigned char* i_msg;
  unsigned int o_msg_len;
  unsigned char* o_msg;
  unsigned long long o_generation_time;
  unsigned long long o_expiration_time;
  unsigned int o_next_crl_time;
} dot2_decode_msg_t;
int dot2_decode_msg(unsigned int i_psid, dot2_decode_msg_t* para);

void dot2_set_attach_cert_interval_ms(unsigned int interval_ms);
void dot2_set_change_cert_interval_ms(unsigned int interval_ms);

#endif	/* __DOT2_API_H__ */
