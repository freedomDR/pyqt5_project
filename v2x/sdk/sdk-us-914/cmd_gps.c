#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>

#include <libcli.h>
#include <craton/cli.h>
#include <tx_api.h>
#include <atlk/nav_service.h>
#include <gps.h>

void handle_show(cli_instance_t *cli, char *argv[], int argc)
{
	(void) argv;
	(void) argc;
	struct gps_info_p gps;
	gps_info_p gps_list[10] = {0};
	gps_ret_t ret;
	gps_ret_t ret_list;
	int i = 0;

	//gps_init()

	(void) memset(&gps, 0, sizeof(gps));
	ret = get_gps_info_p(&gps);

	if(ret == GPS_OK){
		cli_print(cli, "GPSINFO: time: %s", ctime(&(gps.time)));
		cli_print(cli, "    latitude %.7lf, longitude %.7lf", gps.latitude, gps.longitude);
		cli_print(cli, "    altitude %.7lf m", gps.altitude);
		cli_print(cli, "    ground speed %.7lf m/s", gps.groundspeed);
		cli_print(cli, "    angle %.7lf", gps.angle);
	} else {
		cli_print(cli, "GPSINFO: GPS error!!");
	}

	/* Demo get gps list
	ret_list = get_gps_info_list(gps_list);
	if(ret_list > 0) {
		cli_print(cli, "Total %d records.",ret_list);
		for(i = 0; i < ret_list; i++) {
			cli_print(cli, "GPSINFO[%d]: time: %s.%d", i, ctime(&(gps_list[i].gps.time)), gps_list[i].ms);
			cli_print(cli, "             latitude %.7lf, longitude %.7lf", gps_list[i].gps.latitude, gps_list[i].gps.longitude);
			cli_print(cli, "             altitude %.7lf m", gps_list[i].gps.altitude);
			cli_print(cli, "             ground speed %.7lf m/s", gps_list[i].gps.groundspeed);
			cli_print(cli, "             angle %.7lf", gps_list[i].gps.angle);
		}
	} else {
		cli_print(cli, "GPSINFO: GPS error!!");
	}
	*/
	//gps_exit()
}

void gps_print_help(cli_instance_t *cli)
{
	cli_print(cli, "gps commands");
	cli_print(cli, "usage: gps <command>");
	cli_print(cli, "<command>: show");
}

int cmd_gps(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	(void)cli;
	(void)command;

	if(strcmp(argv[0] , "show") == 0) {
		handle_show(cli, argv, argc);
	} else {
		gps_print_help(cli);
	}
    return CLI_OK;
}

