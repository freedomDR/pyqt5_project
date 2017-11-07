
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>

#include <tx_api.h>
#include <atlk/v2x_service.h>

#include <atlk/nav.h>
#include <atlk/mibs/wlan-mib.h>
#include <libcli.h>
#include <craton/cli.h>
#include <craton/syslog.h>

#include <linuxapi.h>
#include <dot3_common.h>
#include <dot2_api.h>
#include <dot3_api.h>
#include <gps.h>
#include <craton/bootparam.h>


int sec_hsm_init(void);
int dot3_init();
//void init_ITRIdot4(void);

/* P1609.3 Test Command ProtoType */
int cmd_dot3_test_init(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_test_exit(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_provider_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_provider_serv_change(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_provider_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_user_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_user_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_channel_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_channel_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_wsm_serv_create(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_wsm_serv_delete(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_wsm_send(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_show_mib(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_cfg_wsa(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_dot3_show_channel(cli_instance_t *cli, const char *command, char *argv[], int argc);


int cmd_audio_test(cli_instance_t *cli, const char *command, char *argv[], int argc);

int cmd_wave(cli_instance_t *cli, const char *command, char *argv[], int argc);
//int cmd_ip(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_gps(cli_instance_t *cli, const char *command, char *argv[], int argc);
void _ip_init(void);
int cmd_test_us_sec(cli_instance_t *cli, const char *command, char *argv[], int argc);


int cmd_gpio_pca9554a(cli_instance_t *cli, const char *command, char *argv[], int argc);

int cmd_j2735r63_bsm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txBsm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxBsm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_map(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txMap(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxMap(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_rsa(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txRsa(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxRsa(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_spat(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txSpat(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxSpat(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_rtcm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txRtcm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxRtcm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_tim(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txTim(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxTim(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_csr(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txCsr(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxCsr(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_nmea(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txNmea(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxNmea(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_ica(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txIca(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxIca(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_eva(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txEva(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxEva(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_pdm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txPdm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxPdm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_ssm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txSsm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxSsm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_srm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txSrm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxSrm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_pvd(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txPvd(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxPvd(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_j2735r63_psm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_txPsm(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_rxPsm(cli_instance_t *cli, const char *command, char *argv[], int argc);

int cmd_oslink(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_oslink_debug(cli_instance_t *cli, const char *command, char *argv[], int argc);
int cmd_cvt_rule(cli_instance_t *cli, const char *command, char *argv[], int argc);


int check_hw_proto(void);


void craton_user_init(void)
{
	/* CLI instance */
	cli_instance_t *cli = NULL;
	/* CLI command handle */
	struct cli_command *command = NULL;
	/* Autotalks return code */
	atlk_rc_t rc = ATLK_OK;
	int ret;

	printf("This is US, version 914, Protocol: P1609-2016 and J2735R63, Date: 20170224\n");

	linux_api_init();
//	_ip_init();
	wave_init();
	gps_init();
	sec_hsm_init();
	dot3_init();

	/* This is a reference func for user to generate cert for WSAs */
	//ret = load_static_wsa_cert();
	//if (ret) {
	//	printf("load_static_wsa_cert failure\n");
	//	abort();
	//}

	//check_hw_proto();

	/* Get CRATON UART CLI instance */
	rc = cli_instance_get(&cli, CLI_INSTANCE_TYPE_UART);
	if (atlk_error(rc)) {
		fprintf(stderr, "cli_instance_get: %s\n", atlk_rc_to_str(rc));
		abort();
	}
	assert(cli != NULL);

	/* Register example command */
	//command = cli_register_command(cli, NULL, "ip", cmd_ip,
	//	 PRIVILEGE_UNPRIVILEGED, MODE_ANY, "ITRI ip command");
	
	/* GPS Test Commands */
	command = cli_register_command(cli, NULL, "gps"                      , cmd_gps                      , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "GPS Test"                             );

	/* P1609.3 Test Commands */
	command = cli_register_command(cli, NULL, "dot3_test_init"           , cmd_dot3_test_init           , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tInit dot3 test environment");
	command = cli_register_command(cli, NULL, "dot3_test_exit"           , cmd_dot3_test_exit           , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tExit dot3 test environment");
	command = cli_register_command(cli, NULL, "dot3_provider_serv_create", cmd_dot3_provider_serv_create, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tCreate a provider service" );
	command = cli_register_command(cli, NULL, "dot3_provider_serv_change", cmd_dot3_provider_serv_change, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tChange a provider service" ); 
	command = cli_register_command(cli, NULL, "dot3_provider_serv_delete", cmd_dot3_provider_serv_delete, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tDelete a provider service" ); 
	command = cli_register_command(cli, NULL, "dot3_user_serv_create"    , cmd_dot3_user_serv_create    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tCreate a user service"     );
	command = cli_register_command(cli, NULL, "dot3_user_serv_delete"    , cmd_dot3_user_serv_delete    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tDelete a user service"     ); 
	command = cli_register_command(cli, NULL, "dot3_channel_serv_create" , cmd_dot3_channel_serv_create , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tCreate a channel service"  );
	command = cli_register_command(cli, NULL, "dot3_channel_serv_delete" , cmd_dot3_channel_serv_delete , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tDelete a channel service"  ); 
	command = cli_register_command(cli, NULL, "dot3_wsm_serv_create"     , cmd_dot3_wsm_serv_create     , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tCreate a WSM service"      );
	command = cli_register_command(cli, NULL, "dot3_wsm_serv_delete"     , cmd_dot3_wsm_serv_delete     , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tDelete a WSM service"      ); 
	command = cli_register_command(cli, NULL, "dot3_wsm_send"            , cmd_dot3_wsm_send            , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tSend WSM packets"          );
	command = cli_register_command(cli, NULL, "dot3_show_mib"            , cmd_dot3_show_mib            , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tShow dot3 MIB Information"            );
      //command = cli_register_command(cli, NULL, "dot3_cfg_wsa"             , cmd_dot3_cfg_wsa             , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "ITRI dot3 cfg wsa command"            );
      //command = cli_register_command(cli, NULL, "dot3_show_channel"        , cmd_dot3_show_channel        , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "ITRI dot3 show channel command"       );


	
	command = cli_register_command(cli, NULL, "test_us_sec", cmd_test_us_sec,
                                PRIVILEGE_UNPRIVILEGED, MODE_ANY, "\tDot2 encode/decode test");

	/* Audio Test Commands */
//	command = cli_register_command(cli, NULL, "audio_test"               , cmd_audio_test               , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Audio Test"                           );

	/* J2735R63 Test Commands */
	command = cli_register_command(cli, NULL, "j2735r63_bsm"             , cmd_j2735r63_bsm             , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_bsm encode/decode test");
	command = cli_register_command(cli, NULL, "txBsm"                    , cmd_txBsm                    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send BSM");
	command = cli_register_command(cli, NULL, "rxBsm"                    , cmd_rxBsm                    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive BSM");
	command = cli_register_command(cli, NULL, "j2735r63_map"             , cmd_j2735r63_map             , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_map encode/decode test");
	command = cli_register_command(cli, NULL, "txMap"                    , cmd_txMap                    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send MAP");
	command = cli_register_command(cli, NULL, "rxMap"                    , cmd_rxMap                    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive MAP");
	command = cli_register_command(cli, NULL, "j2735r63_rsa"             , cmd_j2735r63_rsa             , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_rsa encode/decode test");
	command = cli_register_command(cli, NULL, "txRsa"                    , cmd_txRsa                    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send RSA");
	command = cli_register_command(cli, NULL, "rxRsa"                    , cmd_rxRsa                    , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive RSA");
	command = cli_register_command(cli, NULL, "j2735r63_spat"            , cmd_j2735r63_spat            , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_spat encode/decode test");
	command = cli_register_command(cli, NULL, "txSpat"                   , cmd_txSpat                   , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send SPAT");
	command = cli_register_command(cli, NULL, "rxSpat"                   , cmd_rxSpat                   , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive SPAT");
	command = cli_register_command(cli, NULL, "j2735r63_rtcm"			 , cmd_j2735r63_rtcm			, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_rtcm encode/decode test");
	command = cli_register_command(cli, NULL, "txRtcm"					 , cmd_txRtcm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send RTCM");
	command = cli_register_command(cli, NULL, "rxRtcm"					 , cmd_rxRtcm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive RTCM");
	command = cli_register_command(cli, NULL, "j2735r63_tim"			 , cmd_j2735r63_tim				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_tim encode/decode test");
	command = cli_register_command(cli, NULL, "txTim"					 , cmd_txTim					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send TIM");
	command = cli_register_command(cli, NULL, "rxTim"					 , cmd_rxTim					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive TIM");
	command = cli_register_command(cli, NULL, "j2735r63_csr"			 , cmd_j2735r63_csr				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_csr encode/decode test");
	command = cli_register_command(cli, NULL, "txCsr"					 , cmd_txCsr					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send CSR ");
	command = cli_register_command(cli, NULL, "rxCsr"					 , cmd_rxCsr					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive CSR");			
	command = cli_register_command(cli, NULL, "j2735r63_nmea"			 , cmd_j2735r63_nmea			, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_nmea encode/decode test");
	command = cli_register_command(cli, NULL, "txNmea"					 , cmd_txNmea					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send NMEA");
	command = cli_register_command(cli, NULL, "rxNmea"					 , cmd_rxNmea					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive NMEA");	
	command = cli_register_command(cli, NULL, "j2735r63_ica"			 , cmd_j2735r63_ica				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_ica encode/decode test");
	command = cli_register_command(cli, NULL, "txIca"					 , cmd_txIca					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send ICA");
	command = cli_register_command(cli, NULL, "rxIca"					 , cmd_rxIca					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive ICA");
	command = cli_register_command(cli, NULL, "j2735r63_eva"			 , cmd_j2735r63_eva				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_eva encode/decode test");
	command = cli_register_command(cli, NULL, "txEva"					 , cmd_txEva					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send EVA");
	command = cli_register_command(cli, NULL, "rxEva"					 , cmd_rxEva					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive EVA");
	command = cli_register_command(cli, NULL, "j2735r63_pdm"			 , cmd_j2735r63_pdm				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_pdm encode/decode test");
	command = cli_register_command(cli, NULL, "txPdm"					 , cmd_txPdm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send PDM");
	command = cli_register_command(cli, NULL, "rxPdm"					 , cmd_rxPdm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive PDM");
	command = cli_register_command(cli, NULL, "j2735r63_ssm"			 , cmd_j2735r63_ssm				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_ssm encode/decode test");
	command = cli_register_command(cli, NULL, "txSsm"					 , cmd_txSsm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send SSM");
	command = cli_register_command(cli, NULL, "rxSsm"					 , cmd_rxSsm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive SSM");
	command = cli_register_command(cli, NULL, "j2735r63_srm"			 , cmd_j2735r63_srm				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_srm encode/decode test");
	command = cli_register_command(cli, NULL, "txSrm"					 , cmd_txSrm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send SRM");
	command = cli_register_command(cli, NULL, "rxSrm"					 , cmd_rxSrm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive SRM");
	command = cli_register_command(cli, NULL, "j2735r63_pvd"			 , cmd_j2735r63_pvd				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_pvd encode/decode test");
	command = cli_register_command(cli, NULL, "txPvd"					 , cmd_txPvd					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send PVD");
	command = cli_register_command(cli, NULL, "rxPvd"					 , cmd_rxPvd					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive PVD");
	command = cli_register_command(cli, NULL, "j2735r63_psm"			 , cmd_j2735r63_psm				, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "j2735r63_psm encode/decode test");
	command = cli_register_command(cli, NULL, "txPsm"					 , cmd_txPsm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Send PSM");
	command = cli_register_command(cli, NULL, "rxPsm"					 , cmd_rxPsm					, PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Receive PSM");

	/* PCA9554A GPIO Test Commands */
	//command = cli_register_command(cli, NULL, "gpio_pca9554a"            , cmd_gpio_pca9554a            , PRIVILEGE_UNPRIVILEGED, MODE_ANY, "GPIO for PCA9554A"                    ); 	
    
	/* OSlink */
	cmd_oslink(cli, NULL, NULL, 0);	
	
	     
	command = cli_register_command(cli, NULL, "oslink_debug", cmd_oslink_debug,
                PRIVILEGE_UNPRIVILEGED, MODE_ANY, "os link rule command debug for US protocol");	
    
	
	command = cli_register_command(cli, NULL, "cvt_rule", cmd_cvt_rule,
                PRIVILEGE_UNPRIVILEGED, MODE_ANY, "Convert packet between WSM & UDP"); 	
                                 
                
	/* Add your own cmd by using the function "cli_register_command" */

	if (command == NULL) {
		fprintf(stderr, "cli_register_command returned NULL\n");
		abort();
	}

	return;
}

