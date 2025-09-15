/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
// Filename: iTest__wirelessAP.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This target aims to implement a wireless access point
//		[!] Don't forget to set the HILO_MYSSIDNAME and HILO_MYPASSWORD symbols in the building configuration file
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file  "Linuxwoodo/trunk/templates/lgpl-3.0.txt".
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_mac.h"

#ifndef HILO_MYSSIDNAME
#error "You have to define HILO_MYSSIDNAME symbol in the configuration file (read CMakeLists.txt file for further details)"
#endif

#ifndef HILO_MYPASSWORD
#error "You have to define HILO_MYPASSWORD symbol in the configuration file (read CMakeLists.txt file for further details)"
#endif


static void wifi_event_handler (void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
	//
	// Description:
	//
	//
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
		wifi_event_ap_staconnected_t *e = (wifi_event_ap_staconnected_t *)event_data;
		ESP_LOGI(__FILE__, "STA is connected: " MACSTR ", AID=%d", MAC2STR(e->mac), e->aid);

	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
		wifi_event_ap_stadisconnected_t *e = (wifi_event_ap_stadisconnected_t *)event_data;
		ESP_LOGI(__FILE__, "STA is disconnected: " MACSTR ", AID=%d", MAC2STR(e->mac), e->aid);
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//                                                          M A I N
//------------------------------------------------------------------------------------------------------------------------------
void app_main() {

	// Non Volatile Storage (NVS) initialization
	if (nvs_flash_init() != ESP_OK)
		// ERROR!
		ESP_LOGE(__FILE__, "NVS initialization failed");

	// Network module initialization
	else if (esp_netif_init() != ESP_OK)
		// ERROR!
		ESP_LOGE(__FILE__, "Network module initialization failed");

	// Event handlers manager initialization
	if (esp_event_loop_create_default() != ESP_OK)
		// ERROR!
		ESP_LOGE(__FILE__, "Event handlers manager initialization failed");

	else {
		esp_netif_t        *ap_netif = NULL;
		wifi_init_config_t cfg       = WIFI_INIT_CONFIG_DEFAULT();

		/*
		esp_netif_ip_info_t ip;
		IP4_ADDR(&ip.ip, 192,168,10,1);
		IP4_ADDR(&ip.gw, 192,168,10,1);
		IP4_ADDR(&ip.netmask, 255,255,255,0);
		ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));
		ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ip));
		ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));
		*/

		// AP interface creation (192.168.4.1/24 and DHCP)
		ap_netif = esp_netif_create_default_wifi_ap();

		if (ap_netif == NULL)
			// ERROR!
			ESP_LOGE(__FILE__, "AP creation failed");
		
		// Wi-Fi initialization
		else if (esp_wifi_init(&cfg) != ESP_OK)
			// ERROR!
			ESP_LOGE(__FILE__, "Wi-Fi initialization failed");

		// Wi-Fi events registering
		else if (esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL) != ESP_OK)
			// ERROR!
			ESP_LOGE(__FILE__, "Wi-Fi events handlers registering failed");

		// Checking for the SSID name and the PASSWORD string
		else if (strlen(HILO_MYSSIDNAME) < 8 || strlen(HILO_MYPASSWORD) < 8)
			// ERROR!
			ESP_LOGE(__FILE__, "The SSID name or the PASSWORD string don't respect the WPA2 requirements");

		else {
			wifi_config_t wifi_config = { 0 };

			// AP configuration (SSID must be minimum 8 chars in WPA2)
			snprintf((char*)wifi_config.ap.ssid, sizeof(wifi_config.ap.ssid), HILO_MYSSIDNAME);
			wifi_config.ap.ssid_len    = 0;             // Self calculated

			// AP configuration (PASSWORD must be minimum 8 chars in WPA2)
			snprintf((char*)wifi_config.ap.password, sizeof(wifi_config.ap.password), HILO_MYPASSWORD);
	
			wifi_config.ap.channel        = 6;
			wifi_config.ap.max_connection = 4;
			wifi_config.ap.ssid_hidden    = 0;
			wifi_config.ap.authmode       = WIFI_AUTH_WPA2_PSK;
			wifi_config.ap.pmf_cfg        = (wifi_pmf_config_t){ .required = false };

			// Just for debug purpose!!!!
			// wifi_config.ap.password[0] = '\0';
			// wifi_config.ap.authmode = WIFI_AUTH_OPEN;

			// Setting AP mode
			if (esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK)
				// ERROR!
				ESP_LOGE(__FILE__, "Setting AP mode failed");

			// AP configuring
			else if (esp_wifi_set_config(WIFI_IF_AP, &wifi_config) != ESP_OK)
				// ERROR!
				ESP_LOGE(__FILE__, "AP configuration process failed");
	
			// AP starting...
			else if (esp_wifi_start() != ESP_OK)
				// ERROR!
				ESP_LOGE(__FILE__, "WI-Fi cannot be started");

			else 
				ESP_LOGI(__FILE__, "  **** AP started. SSID:%s  pass:%s  ch:%d ****",
					wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel
				);
		}
	}
	return;
}
