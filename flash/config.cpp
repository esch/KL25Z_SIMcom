/*
 config.c
*
*/
// #include <esp8266.h>
// #include "ets_sys.h"
// #include "os_type.h"
// #include "mem.h"
// #include "osapi.h"
// #include "user_interface.h"

// #include "mqtt.h"
#include "config.h"
// #include "user_config.h"
// #include "debug.h"

// extern void  _console(const char* format, ...);
#ifdef LATER
SYSCFG sysCfg;
SAVE_FLAG saveFlag;

void ICACHE_FLASH_ATTR CFG_Save()
{
	spi_flash_read((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,(uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	os_printf("cfg-save: %d \n",saveFlag.flag);

	if (saveFlag.flag == 0) {
		spi_flash_erase_sector(CFG_LOCATION + 1);
		spi_flash_write((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE,(uint32 *)&sysCfg, sizeof(SYSCFG));
		saveFlag.flag = 1;
		spi_flash_erase_sector(CFG_LOCATION + 3);
		spi_flash_write((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,(uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	} else {
		spi_flash_erase_sector(CFG_LOCATION + 0);
		spi_flash_write((CFG_LOCATION + 0) * SPI_FLASH_SEC_SIZE,(uint32 *)&sysCfg, sizeof(SYSCFG));
		saveFlag.flag = 0;
		spi_flash_erase_sector(CFG_LOCATION + 3);
		spi_flash_write((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,(uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	}

	os_printf("CFG_Save ths key: %s \n",(char *)sysCfg.ths_key);
}


void ICACHE_FLASH_ATTR CFG_Info() {
	_console("\r\nCfg Info: 0x%x-0x%x (0x%x bytes)\r\n",CFG_LOCATION*0x1000,CFG_LOCATION*0x1000+sizeof(SYSCFG),  sizeof(SYSCFG));	
}


void ICACHE_FLASH_ATTR CFG_Load()
{

	os_printf("\r\nload cfg...0x%x-0x%x (%d bytes)\r\n",CFG_LOCATION,CFG_LOCATION+sizeof(SYSCFG),  sizeof(SYSCFG));
	spi_flash_read((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,(uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	os_printf("add saveFlag : %x \n", *(unsigned int  *)(&saveFlag) ) ;

	if (saveFlag.flag == 0) {
		spi_flash_read((CFG_LOCATION + 0) * SPI_FLASH_SEC_SIZE,(uint32 *)&sysCfg, sizeof(SYSCFG));
	} else {
		spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE,(uint32 *)&sysCfg, sizeof(SYSCFG));
	}

	os_printf("CFG_Load flag: %d \n\n",(int )saveFlag.flag);

	os_printf("CFG_Load ths key: %s \n",(char *)sysCfg.ths_key);
	// os_printf("CFG_Load ths channel: %d \n",(int )sysCfg.ths_channel);
	// os_printf("CFG_Load ths field: %d \n",(int )sysCfg.ths_field);
	os_printf("CFG_Load ths enable: %d \n",(int )sysCfg.ths_enable);

	
	if(sysCfg.cfg_holder != CFG_HOLDER){
		os_memset(&sysCfg, 0x00, sizeof sysCfg);

		sysCfg.cfg_holder = CFG_HOLDER;

		sysCfg.sta_type=STA_TYPE;
		sysCfg.httpd_port=HTTPD_PORT;
		sysCfg.httpd_auth=HTTPD_AUTH;

		sysCfg.ntp_enable=NTP_ENABLE;
		sysCfg.ntp_tz=NTP_TZ;
	
		os_sprintf((char *)sysCfg.sta_mode, "%s", STA_MODE);
		os_sprintf((char *)sysCfg.sta_ip, "%s", STA_IP);
		os_sprintf((char *)sysCfg.sta_mask, "%s", STA_MASK);
		os_sprintf((char *)sysCfg.sta_gw, "%s", STA_GW);
		os_sprintf((char *)sysCfg.sta_ssid, "%s", STA_SSID);
		os_sprintf((char *)sysCfg.sta_pass, "%s", STA_PASS);
		sysCfg.sta_type=STA_TYPE;

		os_sprintf((char *)sysCfg.ap_ip, "%s", AP_IP);
		os_sprintf((char *)sysCfg.ap_mask, "%s", AP_MASK);
		os_sprintf((char *)sysCfg.ap_gw, "%s", AP_GW);

		sysCfg.httpd_port=HTTPD_PORT;
		sysCfg.httpd_auth=HTTPD_AUTH;
		os_sprintf((char *)sysCfg.httpd_user, "%s", HTTPD_USER);
		os_sprintf((char *)sysCfg.httpd_pass, "%s", HTTPD_PASS);


		sysCfg.ds18_updatefreq = 120;
		sysCfg.ds18_enable = 1;

		sysCfg.ntp_enable = NTP_ENABLE;
		sysCfg.ntp_tz = NTP_TZ;


		os_printf(" default configurations\r\n");

		CFG_Save();
	}

}
#endif
