#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <libcli.h>
#include <craton/cli.h>
#include <craton/debug.h>
#include <tx_api.h>
#include <dot3_common.h>
#include <dot3_api.h>
#include <dot2_api.h>

TX_THREAD dot3_event_thread;
unsigned char dot3_event_stack[4*1024];
unsigned char dot3_event_flag;
TX_THREAD dot3_wsm_thread;
unsigned char dot3_wsm_stack[4*1024];
unsigned char dot3_wsm_flag;
static wme_handle_t handle;

void timersub(struct timeval *a, struct timeval *b, struct timeval *res){
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


int load_static_wsa_cert(void){
        int ret = DOT2_OK;
        unsigned char pri_key_text[] =
        "218d2c15c7a5f2748dd6016f535f87ba0e7c6e2a0679a2a28c8215602730dd6d";
        unsigned char cert_text[] =
        "40030081001083736f2e004616e1788486000501010002800780808262855ac193f0f1f8539b10801867d21fbe970182eebd2a932e47d4c9cf2ba9708080d2abcffc5184578bd070f618504c1e9d040ed8d17ec4fe70d47e274efd6a93259734796d71c19551c2c3b378c17b975b95a4f0788df696e99782eae791a1d11d";
        unsigned char pri_key_bin[32];
        unsigned char cert_bin[256];
        int ch, len;
        unsigned int cmh = 0;

        len = 0;
        while( sscanf(pri_key_text + len*2, "%02x", &ch) != EOF){
                pri_key_bin[len++] = ch;
        }

        len = 0;
        while( sscanf(cert_text + len*2, "%02x", &ch) != EOF){
                cert_bin[len++] = ch;
        }

        if( sec_cmh_new(&cmh) != DOT2_OK){
                ret = DOT2_FAILURE;
        }
        else if( (ret = sec_cmh_store_cert_and_key(cmh, len, cert_bin, pri_key_bin)) != DOT2_OK){
                ret = DOT2_FAILURE;
        }
        else{
		/* Do nothing */
        }
        return ret;
}

void dot3_event_handler(ULONG input)
{
    int ret;
    struct event_message event;

    while(1){
        ret = wme_event_recv(&handle, &event, 0xFFFFFFFF);
        if(ret != 0) {
            break;
        }
		
        if(event.event == EVENT_CHANNEL){
            if(event.reason == REASON_CHANNEL_AVAILABLE){
                printf("Channel %d assigned ...\n", event.info.channel.channel);
            }
            else {
                printf("Channel %d unassigned ...\n", event.info.channel.channel);
            }
        }

        if(event.event == EVENT_SERVICE){
            if(event.reason == REASON_SERVICE_AVAILABLE){
                printf("Service available ...\n");
            }
            else {
                printf("Service unavailable ...\n");
            }
        }
        tx_thread_sleep(1);
    }

    dot3_event_flag = 0;
    return 0;
}


void dot3_wsm_handler(ULONG input)
{
    int ret;
    int count = 0;
    struct in_wsm in_wsm;

    while(1) {
        ret = wme_wsm_recv(&handle, &in_wsm, 0xFFFFFFFF);
        if(ret != 0) {
            break;
        }
        printf("recv pkt count = %d, ", ++count);
        printf("psid = %d, ", in_wsm.psid);
        printf("channel = %d, ", in_wsm.channel_number);
        printf("tx_power = %d, ", in_wsm.txpwr_level);
        printf("data_rate = %d, ", in_wsm.data_rate);
        printf("data len = %d\n", in_wsm.length);		
    }

    dot3_wsm_flag = 0;
    return 0;
}

int cmd_dot3_test_init(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int err;

	err = wme_init(&handle);
	if(err != 0) {
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);

	tx_thread_create(&dot3_event_thread, "dot3_event", (void (*)(ULONG))dot3_event_handler, 0x1234, dot3_event_stack, sizeof(dot3_event_stack), 30, 30, TX_NO_TIME_SLICE, TX_AUTO_START);
	tx_thread_create(&dot3_wsm_thread, "dot3_wsm", (void (*)(ULONG))dot3_wsm_handler, 0x1234, dot3_wsm_stack, sizeof(dot3_wsm_stack), 30, 30, TX_NO_TIME_SLICE, TX_AUTO_START);

	dot3_event_flag = 1;
	dot3_wsm_flag = 1;
	
	//load_static_wsa_cert();
	//if (ret) {
	//	printf("load_static_wsa_cert failure\n");
	//	abort();
	//}

	return 0;	
}


int cmd_dot3_test_exit(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
    (void)cli;
    (void)command;
    (void)argv;
    (void)argc;

    int err;

    tx_thread_wait_abort(&dot3_event_thread);
    tx_thread_wait_abort(&dot3_wsm_thread);

    while (1) {
        if (dot3_event_thread.tx_thread_state = TX_COMPLETED && dot3_wsm_thread.tx_thread_state == TX_COMPLETED) {
            tx_thread_delete(&dot3_event_thread);
            tx_thread_delete(&dot3_wsm_thread);
            break;
        }
        tx_thread_sleep(1);
    }

    err = wme_exit(&handle);
    if (err != 0) {
        printf("wme_exit = %d\n", err);
        return 0;
    }
		
    return 0;	
}


int cmd_dot3_provider_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int ret;
	wme_handle_t handle;
	unsigned short serv_index;
	unsigned char radio_number, channel_number, access, priority, repeats, is_sec;
	unsigned int psid, timeout, count, lifetime;
	struct provider_info provider_info;

	if(argc != 9) { /* In threadx, cmd is not included in argc */
		printf("dot3_provider_serv_create <handle> <psid> <repeat_rate> <radio_number> <service_channel> <channel_access> <priority> <is_secure> <sign_lifetime>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("    <repeat_rate> repeat rate of WSA packets, 0 - 255\n");
		printf("    <radio_number> radio number of the service, 1 - 2\n");
		printf("    <service_channel> channel number of sending WSMV2 packets, 172 174 176 180 182 184\n");
		printf("    <channel_access> channel access of provider service, 0:both/1:sch/2:cch\n");		
		printf("    <priority> priority of user service, 0 - 15\n");
		printf("    <is_secure> enalbe the security wsa or not, 0:no sec wsa/1:sec wsa,\n");
		printf("    <sign_lifetime> the life time of signature, 10 - 30000 ms\n");
		printf("  -- example --\n");
		printf("    dot3_provider_serv_create 1 123 1 2 172 0 5 0 10\n");
		return 1;
	}

	handle = atoi(argv[0]);
	psid = atoi(argv[1]);
	repeats = atoi(argv[2]);
	radio_number = atoi(argv[3]);
	channel_number = atoi(argv[4]);
	access = atoi(argv[5]);
	priority = atoi(argv[6]);
	is_sec = atoi(argv[7]);
	lifetime = atoi(argv[8]);
	
	/* Create a provider service */
	memset(&provider_info, 0, sizeof(provider_info));
	memcpy(provider_info.dest_mac, "\xFF\xFF\xFF\xFF\xFF\xFF", MAC_SIZE);
	provider_info.wsa_type= is_sec ? WSA_SECURE : WSA_UNSECURE;
	provider_info.psid = psid;
	provider_info.service_priority = priority;
	provider_info.service_channel = channel_number;
	provider_info.radio_number = radio_number;
	provider_info.channel_access = access;
	provider_info.repeat_rate = repeats;
	provider_info.ip_service = 1;
	memcpy(provider_info.psc, "ITRI_WSA", 9);
	provider_info.psc_len= 9;
	provider_info.certificate_id = 0;
	provider_info.signature_lifetime = lifetime;
	provider_info.wsa_count_threshold = 1;
	provider_info.wsa_count_threshold_interval = 50;
	provider_info.info_element_indicator = BITMASK_WSA_COUNT_THRESHOLD | BITMASK_WSA_COUNT_THRESHOLD_INTERVAL;

	ret = wme_provider_serv_create(&handle, &serv_index, &provider_info);
	if(ret < 0) {
		printf("wme_provider_serv_create = %d\n", ret);
		return 0;
	}
	printf("wme_provider_serv_create success, serv_index = %d\n", serv_index);

	return 0;
}


int cmd_dot3_provider_serv_change(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int ret;
	wme_handle_t handle;
	unsigned short serv_index;
	unsigned char radio_number, channel_number, access, priority, repeats, is_sec;
	unsigned int psid, timeout, count, lifetime;
	struct provider_info provider_info;

	if(argc != 10) { /* In threadx, cmd is not included in argc */
		printf("dot3_provider_serv_change <handle> <serv_index> <psid> <repeat_rate> <radio_number> <service_channel> <channel_access> <priority> <is_secure> <sign_lifetime>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <serv_index> index of the provider service\n");        
		printf("    <psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("    <repeat_rate> repeat rate of WSA packets, 0 - 255\n");
		printf("    <radio_number> radio number of the service, 1 - 2\n");
		printf("    <channel_number> channel number of sending WSMV2 packets, 172 174 176 180 182 184\n");
		printf("    <mode> channel access of provider service, 0:both/1:sch/2:cch\n");		
		printf("    <priority> priority of user service, 0 - 15\n");
		printf("    <is_secure> enalbe the security wsa or not, 0:no sec wsa/1:sec wsa,\n");
		printf("    <sign_lifetime> the life time of signature, 10 - 30000 ms\n");
		printf("  -- example --\n");
		printf("    dot3_provider_serv_change 1 123 1 2 172 0 5 0 10\n");
		return 1;
	}

	handle = atoi(argv[0]);
        serv_index = atoi(argv[1]);
	psid = atoi(argv[2]);
	repeats = atoi(argv[3]);
	radio_number = atoi(argv[4]);
	channel_number = atoi(argv[5]);
	access = atoi(argv[6]);
	priority = atoi(argv[7]);
	is_sec = atoi(argv[8]);
	lifetime = atoi(argv[9]);
	
	/* Create a provider service */
	memset(&provider_info, 0, sizeof(provider_info));
	memcpy(provider_info.dest_mac, "\xFF\xFF\xFF\xFF\xFF\xFF", MAC_SIZE);
	provider_info.wsa_type= is_sec ? WSA_SECURE : WSA_UNSECURE;
	provider_info.psid = psid;
	provider_info.service_priority = priority;
	provider_info.service_channel = channel_number;
	provider_info.radio_number = radio_number;
	provider_info.channel_access = access;
	provider_info.repeat_rate = repeats;
	provider_info.ip_service = 1;
	memcpy(provider_info.psc, "ITRI_WSA", 9);
	provider_info.psc_len= 9;
	provider_info.certificate_id = 0;
	provider_info.signature_lifetime = lifetime;
	provider_info.wsa_count_threshold = 1;
	provider_info.wsa_count_threshold_interval = 50;
	provider_info.info_element_indicator = BITMASK_WSA_COUNT_THRESHOLD | BITMASK_WSA_COUNT_THRESHOLD_INTERVAL;

	ret = wme_provider_serv_change(&handle, serv_index, &provider_info);
	if(ret < 0) {
		printf("wme_provider_serv_change = %d\n", ret);
		return 0;
	}
	printf("wme_provider_serv_change success, serv_index = %d\n", serv_index);

	return 0;
}


int cmd_dot3_provider_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;
    
	int ret;
	wme_handle_t handle;
	unsigned short serv_index;

	if(argc != 2) {
		printf("dot3_provider_serv_delete <handle> <serv_index>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <serv_index> index of the provider service\n");
		printf("  -- example --\n");
		printf("    dot3_provider_serv_delete 1 0\n");
		return 1;
	}

	handle = atoi(argv[0]);
	serv_index = atoi(argv[1]);

	ret = wme_provider_serv_delete(&handle, serv_index);
	if(ret < 0) {
		printf("wme_provider_serv_delete = %d\n", ret);
		return 1;
	}
	printf("wme_provider_serv_delete success\n");

	return 0;
}

int cmd_dot3_user_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int ret;
	wme_handle_t handle;
	unsigned short serv_index;    
	unsigned char request_type, radio_number, channel_number, priority;
	unsigned int psid;
	struct user_info user_info;
	
	if(argc != 6) { /* In threadx, cmd is not included in argc */
		printf("dot3_user_serv_create <handle> <psid> <request_type> <priority> <radio_number> <channel_number>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <psid> psid of sending WSMV2 packets, 1 - 127\n");
		printf("    <request_type> access of user service, 0:MATCH/1:NO SCH ACCESS\n");
		printf("    <priority> priority of user service, 0 - 15\n");
		printf("    <radio_number> radio number of the service, 1 - 2\n");
		printf("    <channel_number> channel number of sending WSMV2 packets, 172 174 176 180 182 184\n");
		printf("  -- example --\n");
		printf("    dot3_user_serv_create 1 123 0 5 2 172\n");
		return 1;
	}

	handle = atoi(argv[0]);
	psid = atoi(argv[1]);
	request_type = atoi(argv[2]);
	priority = atoi(argv[3]);
	radio_number = atoi(argv[4]);
	channel_number = atoi(argv[5]);

	/* Create a user service */
	memset(&user_info, 0, sizeof(user_info));
	user_info.request_type = request_type;
	user_info.psid = psid;
	memcpy(user_info.src_mac, "\xFF\xFF\xFF\xFF\xFF\xFF", 6);
	user_info.service_priority = priority;
	user_info.radio_number = radio_number;
	user_info.channel_number = channel_number;

	ret = wme_user_serv_create(&handle, &serv_index, &user_info);
	if(ret < 0) {
		printf("wme_user_serv_create = %d\n", ret);
		return 0;
	}
	printf("wme_user_serv_create success, serv_index = %d\n", serv_index);

	return 0;
}


int cmd_dot3_user_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;
    
	int ret;
	wme_handle_t handle;
	unsigned short serv_index;

	if(argc != 2) {
		printf("dot3_user_serv_delete <handle> <serv_index>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <serv_index> index of the user service\n");
		printf("  -- example --\n");
		printf("    dot3_user_serv_delete 1 0\n");
		return 1;
	}

	handle = atoi(argv[0]);
	serv_index = atoi(argv[1]);

	ret = wme_user_serv_delete(&handle, serv_index);
	if(ret < 0) {
		printf("wme_user_serv_delete = %d\n", ret);
		return 1;
	}
	printf("wme_user_serv_delete success\n");

	return 0;
}


int cmd_dot3_channel_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int ret;
	wme_handle_t handle;
	unsigned short serv_index;    
	unsigned char radio_number, channel_number, slot, priority;
	struct channel_info channel_info;

	if(argc != 5) { /* In threadx, cmd is not included in argc */
		printf("dot3_channel_serv_create <handle> <radio_number> <channel_number> <slot> <priority>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");	
		printf("    <radio_number> radio number of the service, 1-2\n");
		printf("    <channel_number> channel number of sending WSMV2 packets, 172 174 176 180 182 184\n");
		printf("    <slot> channel mode of user service, 1:cch/2:sch/3:both\n");		
		printf("    <priority> priority of user service, 0 - 63\n");
		printf("  -- example --\n");
		printf("    dot3_channel_serv_create 1 1 172 3 5\n");
		return 1;
	}

	handle = atoi(argv[0]);
	radio_number = atoi(argv[1]);
	channel_number = atoi(argv[2]);
	slot = atoi(argv[3]);
	priority = atoi(argv[4]);

	/* Create a channel service */
	memset(&channel_info, 0, sizeof(channel_info));
	channel_info.service_priority = priority;
	channel_info.radio_number = radio_number;
	channel_info.channel_number = channel_number;
	channel_info.time_slot = slot;

	ret = wme_channel_serv_create(&handle, &serv_index, &channel_info);
	if(ret < 0) {
		printf("wme_channel_serv_create = %d\n", ret);
		return 0;
	}
	printf("wme_channel_serv_create success, serv_index = %d\n", serv_index);

	return 0;
}


int cmd_dot3_channel_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;
    
	int ret;
	wme_handle_t handle;
	unsigned short serv_index;

	if(argc != 2) {
		printf("dot3_channel_serv_delete <handle> <serv_index>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <serv_index> index of the channel service\n");
		printf("  -- example --\n");
		printf("    dot3_channel_serv_delete 1 1\n");
		return 1;
	}

	handle = atoi(argv[0]);
	serv_index = atoi(argv[1]);

	ret = wme_channel_serv_delete(&handle, serv_index);
	if(ret < 0) {
		printf("wme_channel_serv_delete = %d\n", ret);
		return 1;
	}
	printf("wme_channel_serv_delete success\n");

	return 0;
}


int cmd_dot3_wsm_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int ret;
	wme_handle_t handle;
	unsigned short serv_index;    
	unsigned int psid;
	struct wsm_info wsm_info;	

	if(argc != 2) { /* In threadx, cmd is not included in argc */
		printf("dot3_wsm_serv_create <handle> <psid>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <psid> psid of sending WSM packets, 1 - 127\n");
		printf("  -- example --\n");
		printf("    dot3_wsm_serv_create 1 123\n");
		return 1;
	}

	handle = atoi(argv[0]);
	psid = atoi(argv[1]);

	/* Create a wsm service */
	memset(&wsm_info, 0, sizeof(wsm_info));
	wsm_info.psid = psid;

	ret = wme_wsm_serv_create(&handle, &serv_index, &wsm_info);
	if(ret < 0) {
		printf("wme_wsm_serv_create = %d\n", ret);
		return 0;
	}
	printf("wme_wsm_serv_create success, serv_index = %d\n", serv_index);

	return 0;
}


int cmd_dot3_wsm_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;
    
	int ret;
	wme_handle_t handle;
	unsigned short serv_index;

	if(argc != 2) {
		printf("dot3_wsm_serv_delete <handle> <serv_index>\n");
		printf("  -- usage --\n");
		printf("    <handle> handler id\n");
		printf("    <serv_index> index of the wsm service\n");
		printf("  -- example --\n");
		printf("    dot3_wsm_serv_delete 1 1\n");
		return 1;
	}

	handle = atoi(argv[0]);
	serv_index = atoi(argv[1]);

	ret = wme_wsm_serv_delete(&handle, serv_index);
	if(ret < 0) {
		printf("wme_wsm_serv_delete = %d\n", ret);
		return 1;
	}
	printf("wme_wsm_serv_delete success\n");

	return 0;
}


int cmd_dot3_wsm_send(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
    /* Unused parameters */
    (void)cli;
    (void)command;
    (void)argv;
    (void)argc;

    int err, i;
    wme_handle_t handle;
    unsigned short size, num;
    unsigned char radio_number, channel_number, data_rate, tx_power, interval;
    unsigned int psid;
    unsigned char macAddr_c[6] = {0};
    unsigned int macAddr_i[6];
    struct out_wsm out_wsm;
    unsigned char buf[WSM_MAX_SIZE] = {0};
	
    if(argc != 10) { /* In threadx, cmd is not included in argc */
        printf("dot3_wsm_send <handle> <psid> <radio_number> <channel_number> <data_rate> <tx_power> <dest_mac> <size> <interval> <num>\n");
        printf("  -- usage --\n");
        printf("    <handle> handler id\n");
        printf("    <psid> psid of sending WSMV2 packets, 1 - 127\n");
        printf("    <radio_number> radio number of the service, 1-2\n");
        printf("    <channel_number> channel number of sending WSMV2 packets, 172 174 176 180 182 184\n");
        printf("    <data_rate> data rate of sending WSMV2 packets, 6 9 12 18 24 36 48 54\n");
        printf("    <tx_power> tx power level of sending WSMV2 packets, 12- 25, unit:dBm\n");
        printf("    <dest_mac> peer mac address of sending WSMV2 packets, format: XX:XX:XX:XX:XX:XX\n");
        printf("    <size> the size of sending packets, 0 - 1399, unit:byte\n");
        printf("    <interval> the interval of sending packets, 0 - 1000, unit:ms\n");
        printf("    <num> the number of sending packets, 1 - 10000\n");		
        printf("  -- example --\n");
        printf("    dot3_wsm_send 1 123 1 172 6 18 FF:FF:FF:FF:FF:FF 1000 100 10\n");
        return 1;
    }

    handle = atoi(argv[0]);
    psid = atoi(argv[1]);
    radio_number = atoi(argv[2]);
    channel_number = atoi(argv[3]);
    data_rate = atoi(argv[4]);
    tx_power = atoi(argv[5]);

    /* Convert the mac addr */
    if (sscanf(argv[6], "%2x:%2x:%2x:%2x:%2x:%2x\n", &macAddr_i[0], &macAddr_i[1], &macAddr_i[2], &macAddr_i[3], &macAddr_i[4], &macAddr_i[5]) != 6) {
        printf("peerMacAddress invalid...\n");
        return 0;
        }
    for (i = 0; i < 6; i++) {
        macAddr_c[i] = (unsigned char) macAddr_i[i];
    }

    size = atoi(argv[7]);
    interval = atoi(argv[8]);
    num = atoi(argv[9]);

    out_wsm.psid = psid;	
    out_wsm.radio_number = radio_number;	
    out_wsm.channel_number = channel_number;
    out_wsm.data_rate = data_rate;
    out_wsm.txpwr_level = tx_power;
    out_wsm.extensions.channel = 1;
    out_wsm.extensions.rate = 1;
    out_wsm.extensions.power = 1;
    memcpy(out_wsm.dest_mac, macAddr_c, 6);
    memcpy(out_wsm.data, buf, size); 
    out_wsm.length = size;

    while (num != 0) {
    err = wme_wsm_send(&handle, &out_wsm);
        if (err != 0) {
            printf("wsm_send = %d\n", err);
            return 0;
        }
        num--;
        usleep(interval * 1000);
    }

    return 0;
}
/*************************************************************************/


/* Dot3 Show Mib Program */
/*************************************************************************/
int cmd_dot3_show_mib(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	struct mib_info info;
	wme_handle_t handle;
	int err;

	err = wme_init(&handle);
	if(err != 0) {
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);	

	printf("*** Provider entry ***\n");
	info.entry_type = PROVIDER_ENTRY;
	info.entry_index = -1;
	info.next = 1;
	while(wme_mib_get(&info) == ACCEPT){
		printf("index = %d\n", info.entry_index);
		printf("priority  = %d\n", info.entry_value.provider_entry.service_priority);
		printf("psid  = %d\n", info.entry_value.provider_entry.psid);
		printf("service channel  = %d\n", info.entry_value.provider_entry.service_channel);		
		printf("channel access  = %d\n", info.entry_value.provider_entry.channel_access);
		printf("channel interval  = %d\n", info.entry_value.provider_entry.channel_interval);
	}

	
	printf("*** User entry ***\n");
	info.entry_type = USER_ENTRY;
	info.entry_index = -1;
	info.next = 1;
	while(wme_mib_get(&info) == ACCEPT){
		printf("index = %d\n", info.entry_index);
		printf("psid  = %d\n", info.entry_value.user_entry.psid);
		printf("psc   = %s\n", info.entry_value.user_entry.psc);
		printf("priority   = %d\n", info.entry_value.user_entry.service_priority);
		printf("wsa_type  = %d\n", info.entry_value.user_entry.wsa_type);
		//printf("radio num  = %d\n", info.entry_value.user_entry.radio_number);
		printf("channel  = %d\n", info.entry_value.user_entry.channel_number);
		printf("interval  = %d\n", info.entry_value.user_entry.channel_interval);
		printf("\n");
	}

	printf("*** Channel entry ***\n");
	info.entry_type = CHANNEL_ENTRY;
	info.entry_index = -1;
	info.next = 1;
	while(wme_mib_get(&info) == ACCEPT){
		printf("index = %d\n", info.entry_index);
		printf("priority  = %d\n", info.entry_value.channel_entry.service_priority);
		printf("channel  = %d\n", info.entry_value.channel_entry.channel_number);
		printf("slot  = %d\n", info.entry_value.channel_entry.time_slot);
		printf("\n");
	}

	printf("*** Wsm entry ***\n");
	info.entry_type = WSM_ENTRY;
	info.entry_index = -1;
	info.next = 1;
	while(wme_mib_get(&info) == ACCEPT){
		printf("index = %d\n", info.entry_index);
		printf("psid = %d\n", info.entry_value.wsm_entry.psid);
		printf("\n");
	}

	printf("*** Available entry ***\n");
	info.entry_type = AVAILABLE_ENTRY;
	info.entry_index = -1;
	info.next = 1;
	while(wme_mib_get(&info) == ACCEPT){
		printf("index = %d\n", info.entry_index);
		printf("repeat_rate = %d\n", info.entry_value.available_entry.repeat_rate);	
		printf("latitude = %d\n", info.entry_value.available_entry.latitude);		
		printf("longitude = %d\n", info.entry_value.available_entry.longitude);		
		printf("psid = %d\n", info.entry_value.available_entry.service_info.psid);
		printf("count_threshold = %d\n", info.entry_value.available_entry.service_info.wsa_count_threshold);
		printf("count_threshold_interval = %d\n", info.entry_value.available_entry.service_info.wsa_count_threshold_interval);		
		printf("channel_number = %d\n", info.entry_value.available_entry.channel_info.channel_number);
		printf("channel_interval = %d\n", info.entry_value.available_entry.channel_info.channel_access);		
		printf("router_life_time = %d\n", info.entry_value.available_entry.router_life_time);
		//printf("wsa_count = %d\n", info.entry_value.available_entry.wsa_count);
		//printf("interval_counter = %d\n", info.entry_value.available_entry.interval_counter);		

		//printf("channel_access = %d\n", info.entry_value.available_entry.channel_info.channel_access);	
		printf("\n");
	}

	wme_exit(&handle);

	return 0;
}
/*************************************************************************/


/* Dot3 Config WSA Program */
/*************************************************************************/
int cmd_dot3_cfg_wsa(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int err;
	struct wsa_cfg_info info;
	wme_handle_t handle;

	err = wme_init(&handle);
	if(err != 0) {
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);

	err = wme_wsa_cfg_get(&info);
	if(err != 0) {
		printf("wme_wsa_cfg_get = %d\n", err);
		return 0;
	}
	printf("current wsa config\n");
	printf("  advertiser id = %s\n", info.advertiser_id);

	wme_exit(&handle);

	return 0;
}
/*************************************************************************/


/* Dot3 Channel Status Program */
/*************************************************************************/
int cmd_dot3_show_channel(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	int err;
	struct dev_info dev_c, dev_s;
	wme_handle_t handle;

	err = wme_init(&handle);
	if(err != 0) {
		printf("wme_init = %d\n", err);
		return 0;
	}
#if 0
	err = wme_device_get(DEV_CCH0, &dev_c);
	if(err != 0) {
		printf("wme_device_get = %d\n", err);
		printf("query CCH0 failed...\n");
		return 0;
	}

	if(dev_c.mode == OP_SWITCH){
		err = wme_device_get(DEV_SCH0, &dev_s);
		if(err != 0){
			printf("wme_device_get = %d\n", err);
			printf("query SCH0 failed...\n");
			return 1;
		}
	}

	if(dev_c.mode == OP_EXTEND)
	        printf("wave0 channel = %d\n", dev_c.channel_number);
	else
	        printf("wave0 channel = %d / %d\n", dev_c.channel_number, dev_s.channel_number);


	err = wme_device_get(DEV_SCH1, &dev_s);
	if(err != 0){
		printf("wme_device_get = %d\n", err);
		printf("query SCH1 failed...\n");
		return 0;
	}

	if(dev_s.mode == OP_EXTEND)
		printf("wave1 channel = %d\n", dev_s.channel_number);
	else
		printf("wave1 channel = 0 / %d\n", dev_s.channel_number);
#endif
	//show_channel();

	wme_exit(&handle);

	return 0;
}
/*************************************************************************/

