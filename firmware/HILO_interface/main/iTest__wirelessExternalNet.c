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
//
// Filename: iTest__wirelessExternalNet.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This integration test aims to check for the capability to connect itself to a remote wireless access point.
//		In order to performs this test you must have a wireless access point, and the data of one available account
//		in that device. In the CMake configuration file, you have to set the following symbols:
//			HILO_SSIDNAME
//			HILO_PASSWORD
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
//
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wError.h"


#ifndef HILO_SSIDNAME
#error "You have to define HILO_SSIDNAME symbol in the configuration file (read CMakeLists.txt file for further details)"
#endif

#ifndef HILO_PASSWORD
#error "You have to define HILO_PASSWORD symbol in the configuration file (read CMakeLists.txt file for further details)"
#endif


static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();

	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
		ESP_LOGI(__FILE__, "The ESP32 has successfully connected to the \"%s\" net", HILO_SSIDNAME);

	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		ESP_LOGI(__FILE__, "Reconnecting in progress...");
		esp_wifi_connect();

	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(__FILE__, "IP address acknowledged: " IPSTR, IP2STR(&event->ip_info.ip));
	}

	return;
}


void wifi_init_sta (const char *ssidStr, const char *pwdStr) {
	wifi_init_config_t cfg         = WIFI_INIT_CONFIG_DEFAULT();
	wifi_config_t      wifi_config = {0};
	
	strcpy((char*)wifi_config.sta.ssid,     ssidStr);
	strcpy((char*)wifi_config.sta.password, pwdStr);
	wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
	

	// Init TCP/IP stack
	if (esp_netif_init() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) TCP/IP stack initialization failed", __LINE__);

	// Default event loop creation
	} else if (esp_event_loop_create_default() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot create the events handlers manager", __LINE__);
	
	// Network interface creating and connecting to the wi-fi driver
	} else if (esp_netif_create_default_wifi_sta() == NULL) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) Network interface creation failed", __LINE__);

	// It initializes WiFi subsystem and allocates resource for WiFi driver
	} else if (esp_wifi_init(&cfg) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) ", __LINE__);

	// Event handlers registering...
	} else if (
		esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL)  != ESP_OK ||
		esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL) != ESP_OK
	) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot register the event handlers", __LINE__);

	// WiFi operating mode setting...
	} else if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) WiFi operating mode selecting failed", __LINE__);

	// WiFi connection configuring
	} else if (esp_wifi_set_config(WIFI_IF_STA, &wifi_config) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) WiFi link configuration failed", __LINE__);

	// Network connection start
	} else if (esp_wifi_start() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) WiFi link starting failed", __LINE__);

	} else 
		ESP_LOGI(__FILE__, "*** wifi_init_sta finished. SSID:%s password:%s ***", HILO_SSIDNAME, HILO_PASSWORD);
}


void app_main(void) {
	// Init NVS
	if (nvs_flash_init() != ESP_OK)
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot allocate NVS memory for the driver", __LINE__);
	else
		wifi_init_sta(HILO_SSIDNAME, HILO_PASSWORD);

	return;
}

