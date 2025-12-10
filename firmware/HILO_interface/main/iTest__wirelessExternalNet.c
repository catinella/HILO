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
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; if not, write to the 
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
#include "esp_log.h"
#include "esp_wifi_types.h"
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


void app_main(void) {
	// Init NVS
	if (nvs_flash_init() != ESP_OK)
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot allocate NVS memory for the driver", __LINE__);

	else if (wifi_connToExtNet(HILO_SSIDNAME, HILO_PASSWORD, wifi_event_handler) == WERROR_SUCCESS)
		ESP_LOGI(__FILE__, "*** wifi_init_sta finished. SSID:%s password:%s ***", HILO_SSIDNAME, HILO_PASSWORD);

	return;
}

