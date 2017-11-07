#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcli.h>
#include <craton/cli.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include <tx_api.h>

#include "dot2_api.h"

static TX_THREAD sec_us_test_thread;
static uint8_t sec_us_test_thread_stack[0x400000];
static void test_us_sec_entry(ULONG input);

void sec_timersub(struct timeval *a, struct timeval *b, struct timeval *res){
	if( a->tv_usec < b->tv_usec){
	  	if( a->tv_sec <= b->tv_sec){
		  	res->tv_usec = 0;
			res->tv_sec = 0;
		}
	  	// need sec shift
		res->tv_usec = 1000000 + a->tv_usec - b->tv_usec;
		res->tv_sec = a->tv_sec - 1 - b->tv_sec;
	}
	else{
	  	if( a->tv_sec < b->tv_sec){
		  	res->tv_usec = 0;
			res->tv_sec = 0;
		}
		res->tv_usec = a->tv_usec - b->tv_usec;
		res->tv_sec = a->tv_sec - b->tv_sec;
	}
}

void dump(unsigned int len, unsigned char* str){
  	unsigned int i;

	for( i = 0; i < len; i++){
	  	if( i != 0 && i % 4 == 0){
		  	printf(" ");
		}
		if( i != 0 && i % 16 == 0){
		  	printf("\n");
		}
	  	printf("%02x", str[i]);
	}
	printf("\nlength = %d\n", len);
}

void rand_msg(unsigned int len, unsigned char* str){
  	unsigned int i;

	for( i = 0; i < len; i++){
	  	str[i] = rand()%256;
	}
}

unsigned int test_msg_len;
unsigned char test_msg[256];
unsigned char sign_flag = 0, verify_flag = 0;
unsigned int psid = SEC_BSM_PSID;
unsigned int repeat = 1;
unsigned char done_flag = 0;
int cmd_test_us_sec(cli_instance_t *cli, const char *command, char *argv[], int argc){
  	(void)cli;
	(void)command;
	ULONG trv;

	srand(time(NULL));
	if( argc < 5){
	  	printf("usage: test_us_sec <psid> <sign_flag> <verify_flag> <msg_len> <repeat>\n");
		printf("<psid>: \tpsid to be encoded if <sign_flag> is set\n");
		printf("<sign_flag>:\t0: not sign; 1: sign\n");
		printf("<verify_flag>:\t0: only decode, not verify; 1: decode and verify\n");
		printf("<msg_len>:\tlength of testing message, 1~256\n");
		printf("<repeat>:\t# of testing rounds\n");
	}
	else{
	  	psid = atoi(argv[0]);
		sign_flag = atoi(argv[1]);
		verify_flag = atoi(argv[2]);
		test_msg_len = atoi(argv[3]);
		repeat = atoi(argv[4]);
		if( test_msg_len < 1 || test_msg_len > sizeof(test_msg)){
		  	test_msg_len = 100;
		}

		done_flag = 0;
		trv = tx_thread_create(&sec_us_test_thread,
		    			"sec_us_test_thread",
					test_us_sec_entry, 0,
					sec_us_test_thread_stack,
					sizeof(sec_us_test_thread_stack),
					40,
					40,
					TX_NO_TIME_SLICE, TX_AUTO_START);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_create failed, trv=0x%lx\n", trv);
		}
		else{
			//	wait execution finished and removing testing thread
			while( done_flag == 0){
				(void)usleep(500000);
			}
		}

		trv = tx_thread_terminate(&sec_us_test_thread);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_terminate failed, trv=0x%lx\n", trv);
		}
		trv = tx_thread_delete(&sec_us_test_thread);
		if (trv != TX_SUCCESS) {
		  	(void)fprintf(stderr, "tx_thread_delete failed, trv=0x%lx\n", trv);
		}
		(void)printf("Dot2 demo thread was removed.\n");
	}

	return CLI_OK;
}

void cmh_clear_cert_cache(void);
int clear_pri_cell_array(void);
static void test_us_sec_entry(ULONG input){
  	(void)input;
  	int ret = DOT2_OK;
	unsigned int i;
  	unsigned char signed_msg[512], extract_msg[512];
	unsigned int signed_msg_len, extract_msg_len;
	unsigned int signer_cmh;
	unsigned char cert_str[512];
	unsigned int cert_str_len;
	struct timeval a, b, diff;
	dot2_encode_msg_t encode_para;
	dot2_decode_msg_t decode_para;

	if( sign_flag != 0){
	  	// need certificate
		// wait until time after 1609.2 start time
		struct timeval sleep_s;
		(void)gettimeofday(&sleep_s, NULL);
		if( (sleep_s.tv_sec - 1072915234) < 5){
		  	(void)printf("Wait system time be after 1609.2 start time...\n");
		}
		while( (sleep_s.tv_sec - 1072915234) < 5){
			(void)usleep(1000000);
			(void)gettimeofday(&sleep_s, NULL);
		}
		cmh_clear_cert_cache();
		clear_pri_cell_array();
		cert_str_len = sizeof(cert_str);
		gettimeofday(&a, NULL);
		ret = dot2_generate_cert(psid, &signer_cmh, &cert_str_len, cert_str);
		gettimeofday(&b, NULL);
		sec_timersub(&b, &a, &diff);
		printf("dot2_generate_cert: %ld.%03ld ms\n", 
		    	diff.tv_sec*1000 + diff.tv_usec/1000, 
			diff.tv_usec%1000);
		if( ret == DOT2_OK){
			printf("Generated cert (cmh %d):\n", signer_cmh);
			(void)dump(cert_str_len, cert_str);
			printf("\n");
		}
		else{
		  	printf("generate_cert(): %d, cmh %d\n", ret, signer_cmh);
		}
		if( psid == SEC_BSM_PSID){
		  	dot2_set_attach_cert_interval_ms(450);
		}
	}
	i = 0;
	while( ret == DOT2_OK && i < repeat){
		usleep(100000);
		(void)rand_msg(test_msg_len, test_msg);
	  	printf("Test msg (round %d):\n", i+1);
		(void)dump(test_msg_len, test_msg);
		printf("\n");
		(void)memset(&encode_para, 0, sizeof(encode_para));
		(void)memset(&decode_para, 0, sizeof(decode_para));

		// encode msg
		encode_para.i_cmh = signer_cmh;
		encode_para.i_sign_flag = sign_flag;
		encode_para.i_msg_len = test_msg_len;
		encode_para.i_msg = test_msg;
		encode_para.i_lifetime = 30000;	// 30s
		encode_para.o_msg_len = sizeof(signed_msg);
		encode_para.o_msg = signed_msg;
		gettimeofday(&a, NULL);
		ret = dot2_encode_msg(psid, &encode_para);
		gettimeofday(&b, NULL);
		sec_timersub(&b, &a, &diff);
		printf("dot2_encode_msg: %ld.%03ld ms\n", 
		    	diff.tv_sec*1000 + diff.tv_usec/1000, 
			diff.tv_usec%1000);
		if( ret == DOT2_OK){
		  	signed_msg_len = encode_para.o_msg_len;
		  	printf("Encoded 1609.2 data:\n");
			(void)dump(signed_msg_len, signed_msg);
			printf("\n");
		}
		else{
		  	printf("dot2_encode_msg(): %d\n", ret);
		}
		if( ret == DOT2_OK){
		  	// decode msg
			decode_para.i_verify_flag = verify_flag;
			decode_para.i_msg_len = signed_msg_len;
			decode_para.i_msg = signed_msg;
			decode_para.o_msg_len = sizeof(extract_msg);
			decode_para.o_msg = extract_msg;
			gettimeofday(&a, NULL);
			ret = dot2_decode_msg(psid, &decode_para);
			gettimeofday(&b, NULL);
			sec_timersub(&b, &a, &diff);
			printf("dot2_decode_msg: %ld.%03ld ms\n", 
			    	diff.tv_sec*1000 + diff.tv_usec/1000, 
				diff.tv_usec%1000);
			if( ret == DOT2_OK){
			  	// decode ok
				extract_msg_len = decode_para.o_msg_len;
				printf("Decoded 1609.2 data:\n");
				(void)dump(extract_msg_len, extract_msg);
				printf("\n");
			}
			else{
			  	printf("dot2_decode_msg(): %d\n", ret);
			}
		}
		i++;
	}
	done_flag = 1;
}

