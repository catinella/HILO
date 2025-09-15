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
// Filename: wifiNetwork.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module contains all the functions used by the ESP32 to connect itself to the client using wireless
//		network or to become itself a wireless access point
//	
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
#include <wifiNetwork.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"


wError wifi_connToExtNet (const char *ssidStr, const char *pwdStr, esp_event_handler_t evh) {
	//
	// Description:
	//	This function allows you to connect the ESP32 device to the argument defined wireless network.
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_ERROR_INITFAILED
	//	WERROR_ERROR_CONFFAILED
	//	WERROR_ERROR_INTFAILURE
	//
	wError             err         = WERROR_SUCCESS;
	wifi_init_config_t cfg         = WIFI_INIT_CONFIG_DEFAULT();
	wifi_config_t      wifi_config = {0};
	
	strcpy((char*)wifi_config.sta.ssid,     ssidStr);
	strcpy((char*)wifi_config.sta.password, pwdStr);
	wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
	

	// Init TCP/IP stack
	if (esp_netif_init() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) TCP/IP stack initialization failed", __LINE__);
		err = WERROR_ERROR_INITFAILED;
		
	// Default event loop creation
	} else if (esp_event_loop_create_default() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot create the events handlers manager", __LINE__);
		err = WERROR_ERROR_INITFAILED;
	
	// Network interface creating and connecting to the wi-fi driver
	} else if (esp_netif_create_default_wifi_sta() == NULL) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) Network interface creation failed", __LINE__);
		err = WERROR_ERROR_INITFAILED;

	// It initializes WiFi subsystem and allocates resource for WiFi driver
	} else if (esp_wifi_init(&cfg) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) esp_wifi_init() failed", __LINE__);
		err = WERROR_ERROR_INITFAILED;

	// Event handlers registering...
	} else if (
		esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,    evh, NULL, NULL)  != ESP_OK ||
		esp_event_handler_instance_register(IP_EVENT,   IP_EVENT_STA_GOT_IP, evh, NULL, NULL) != ESP_OK
	) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot register the event handlers", __LINE__);
		err = WERROR_ERROR_CONFFAILED;

	// WiFi operating mode setting...
	} else if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) WiFi operating mode selecting failed", __LINE__);
		err = WERROR_ERROR_CONFFAILED;

	// WiFi connection configuring
	} else if (esp_wifi_set_config(WIFI_IF_STA, &wifi_config) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) WiFi link configuration failed", __LINE__);
		err = WERROR_ERROR_CONFFAILED;
		
	// Network connection start
	} else if (esp_wifi_start() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) WiFi link starting failed", __LINE__);
		err = WERROR_ERROR_INTFAILURE;
	}

	return(err);
}


wError wifi_intAPstart (const char *ssidStr, const char *pwdStr, esp_event_handler_t evh) {
	//
	// Description:
	//
	//
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_ERROR_INITFAILED
	//	WERROR_ERROR_ILLEGALARG
	//	WERROR_ERROR_INTFAILURE
	//
	wError err = WERROR_SUCCESS;
	
	// Network module initialization
	if (esp_netif_init() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "Network module initialization failed");
		err = WERROR_ERROR_INITFAILED;
		
	// Event handlers manager initialization
	} else if (esp_event_loop_create_default() != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "Event handlers manager initialization failed");
		err = WERROR_ERROR_INITFAILED;

	} else {
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

		if (ap_netif == NULL) {
			// ERROR!
			ESP_LOGE(__FILE__, "AP creation failed");
			err = WERROR_ERROR_INTFAILURE;
		
		// Wi-Fi initialization
		} else if (esp_wifi_init(&cfg) != ESP_OK) {
			// ERROR!
			ESP_LOGE(__FILE__, "Wi-Fi initialization failed");
			err = WERROR_ERROR_INTFAILURE;

		// Wi-Fi events registering
		} else if (esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, evh, NULL, NULL) != ESP_OK) {
			// ERROR!
			ESP_LOGE(__FILE__, "Wi-Fi events handlers registering failed");

		// Checking for the SSID name and the PASSWORD string
		} else if (strlen(ssidStr) < 8 || strlen(pwdStr) < 8) {
			// ERROR!
			ESP_LOGE(__FILE__, "The SSID name or the PASSWORD string don't respect the WPA2 requirements");
			err = WERROR_ERROR_ILLEGALARG;
			
		} else {
			wifi_config_t wifi_config = {0};

			// AP configuration (SSID must be minimum 8 chars in WPA2)
			strcpy((char*)wifi_config.ap.ssid, ssidStr);
			wifi_config.ap.ssid_len    = 0;             // Self calculated

			// AP configuration (PASSWORD must be minimum 8 chars in WPA2)
			strcpy((char*)wifi_config.ap.password, pwdStr);
	
			wifi_config.ap.channel        = 6;
			wifi_config.ap.max_connection = 4;
			wifi_config.ap.ssid_hidden    = 0;
			wifi_config.ap.authmode       = WIFI_AUTH_WPA2_PSK;
			wifi_config.ap.pmf_cfg        = (wifi_pmf_config_t){ .required = false };

			// Just for debug purpose!!!!
			// wifi_config.ap.password[0] = '\0';
			// wifi_config.ap.authmode = WIFI_AUTH_OPEN;

			// Setting AP mode
			if (esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK) {
				// ERROR!
				ESP_LOGE(__FILE__, "Setting AP mode failed");
				err = WERROR_ERROR_CONFFAILED;
				
			// AP configuring
			} else if (esp_wifi_set_config(WIFI_IF_AP, &wifi_config) != ESP_OK) {
				// ERROR!
				ESP_LOGE(__FILE__, "AP configuration process failed");
				err = WERROR_ERROR_CONFFAILED;
				
			// AP starting...
			} else if (esp_wifi_start() != ESP_OK) {
				// ERROR!
				ESP_LOGE(__FILE__, "WI-Fi cannot be started");
				err = WERROR_ERROR_INTFAILURE;

			} else 
				ESP_LOGI(__FILE__, "  **** AP started. SSID:%s  pass:%s  ch:%d ****",
					wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel
				);
		}
	}

	return(err);
}
