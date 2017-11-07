#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <tx_api.h>
#include <tx_posix.h>
#include <nx_api.h>
#include <libcli.h>
#include <craton/cli.h>
#include <craton/net.h>
#include <craton/fs.h>
//#include <linuxapi.h>
#include <nxd_bsd.h>
#include <atlk/sdk.h>

#include <craton/i2c_driver.h>
//#include <porting_apis.h>

#define PCA9554A_DIR_OUT		0
#define CONFIG_SYS_I2C_PCA9554A_ADDR	0x38 

int pca9554a_set_val(uint8_t chip, uint mask, uint data);
int pca9554a_set_pol(uint8_t chip, uint mask, uint data);
int pca9554a_set_dir(uint8_t chip, uint mask, uint data);
int pca9554a_get_val(uint8_t chip);




void print_antenna_usage(cli_instance_t *cli)
{
	cli_print(cli, "Usage : \n");
	cli_print(cli, "	antenna_select <index>\n");	
	cli_print(cli, "	<index> 0:MMCX 1:FAKRA\n");		
	cli_print(cli, "	Example : antenna_select 1\n");
	
}

int cmd_antenna_select(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	(void)cli;
	(void)command;
	(void)argc;
	(void)argv;
	int val;
	int pin = 0; /* antenna select use exteral GPIO pin 0 */
	static unsigned char chip = CONFIG_SYS_I2C_PCA9554A_ADDR;
	
	if (argc != 1)
	{
		print_antenna_usage(cli);
		return -1;
	}
	else 
	{
		if(argv[0][0] == '0')
		{
			val = 0;
		}
		else if(argv[0][0] == '1')
		{
			val = 1;
		}
		else
		{
			print_antenna_usage(cli);
			return -1;
		}
		
		
		if ((val == 0) || (val == 1))
		{
			(void)pca9554a_set_dir(chip, (1 << pin), PCA9554A_DIR_OUT << pin);
			(void)pca9554a_set_val(chip, (1 << pin), (val << pin));
			if (val == 0)
				cli_print(cli,"Internal GPS select MMCX antenna\n");
			else
				cli_print(cli,"Internal GPS select FAKRA antenna\n");
		}
		else
		{
			print_antenna_usage(cli);
			return -1;
		}	
		
	}
	return 0;
}




