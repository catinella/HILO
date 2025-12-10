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
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
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
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <wifiNetwork.h>
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
	else
		wifi_intAPstart(HILO_MYSSIDNAME, HILO_MYPASSWORD, wifi_event_handler);
		
	return;
}
