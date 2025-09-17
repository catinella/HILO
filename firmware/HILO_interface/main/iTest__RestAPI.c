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
// Filename: iTest__RestAPI.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This test aim to set the ESP32 in a webserver ables to provides two REST APIs:
//			- http://<ESP32 IP>/status (accessible by GET method)
//			- http://<ESP32 IP>/echo   (accessible by POST method)
//
//		[!] You can test the APIs using CURL tool as in the following examples:
//			curl --verboose http://<ESP32 IP>/status \
//				--request GET
//
//			curl --verbose http://<ESP32 IP>/echo \
//				--request POST  \
//				--heder "Content-Type: application/json" \
//				--data '{"led":1, "time":500}'
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "esp_timer.h"

#include <wError.h>
#include <wifiNetwork.h>
#include <restApiServer.h>


#ifndef HILO_SSIDNAME
#error "You have to define HILO_SSIDNAME symbol in the configuration file (read CMakeLists.txt file for further details)"
#endif

#ifndef HILO_PASSWORD
#error "You have to define HILO_PASSWORD symbol in the configuration file (read CMakeLists.txt file for further details)"
#endif


//------------------------------------------------------------------------------------------------------------------------------
//                                               H A N D L E R S
//------------------------------------------------------------------------------------------------------------------------------
static esp_err_t status_get_handler(httpd_req_t *req) {
	//
	// Description:
	//	This function will be executed for every detected http://<ESP32>/status request.
	//
	bool have_ip = false;
	esp_netif_ip_info_t ip;

	ESP_LOGW(__FILE__, "GET /status request detected");

	// Imposta Content-Type JSON
	httpd_resp_set_type(req, "application/json");

	//
	// IP address retriving from STA interface
	//
	{
		esp_netif_t *sta = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
		if (sta && esp_netif_get_ip_info(sta, &ip) == ESP_OK) have_ip = true;
	}

	//
	// JSON object building
	//
	{
		cJSON     *root = cJSON_CreateObject();
		uint64_t  uptime_ms = (uint64_t)(esp_timer_get_time() / 1000);
		
		cJSON_AddStringToObject(root, "status", "ok");
		if (have_ip) {
			char ipbuf[16];
			snprintf(ipbuf, sizeof(ipbuf), IPSTR, IP2STR(&ip.ip));
			cJSON_AddStringToObject(root, "ip", ipbuf);
		} else {
			cJSON_AddNullToObject(root, "ip");
		}
		cJSON_AddNumberToObject(root, "uptime_ms", (double)uptime_ms);

		char *json = cJSON_PrintUnformatted(root);
		httpd_resp_sendstr(req, json);

		cJSON_Delete(root);
		free(json);
	}
	
	return ESP_OK;
}


static esp_err_t echo_post_handler(httpd_req_t *req) {
	//
	// Description:
	//	The handler associated to the POST method calls
	//
	int       total_len = req->content_len;
	int       max       = 1024;              // Max body size setting to avoid security risks
	esp_err_t err       = ESP_OK;
	
	ESP_LOGW(__FILE__, "POST /echo request detected");
	
	if (total_len <= 0) {
		// 400 = Bad request
		httpd_resp_send_err(req, 400, "Empty body");
		err = ESP_FAIL;

	} else if (total_len > max) {
		// 413 = Too large payload
		httpd_resp_send_err(req, 413, "Body too large");
		err = ESP_FAIL;

	} else {
		char buf[1025];
		int  received = 0, r = 0;

		while (received < total_len && err == ESP_OK) {
			r = httpd_req_recv(req, buf + received, total_len - received);
			if (r <= 0) {
				httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Read error");
				err = ESP_FAIL;
			}
			received += r;
		}
		buf[received] = '\0';
		ESP_LOGI(__FILE__, "POST request's data: \"%s\"", buf != NULL ? buf : "(empty)");

		// Rispondi eco
		httpd_resp_set_type(req, "text/plain");
		httpd_resp_send(req, buf, received);
	}
	
	return(err);
}


static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	//
	// Dewscription:
	//	This is the WiFi events handler
	//	In order to get the complete WiFi events list, look for the "wifi_event_t" enum definition in the
	//	"esp_wifi_types.h" header file
	//
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		// [!] WIFI_EVENT_STA_START means STA is ready to be connected
		esp_wifi_connect();
		
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		ESP_LOGW(__FILE__, "Wi-Fi link has been broken, I will retry to connect to...");
		esp_wifi_connect();
		restApiServer_stop();
		
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(__FILE__, "I got the IP address: " IPSTR, IP2STR(&event->ip_info.ip));
		restApiServer_start();
	}
	return;
}

//------------------------------------------------------------------------------------------------------------------------------
//                                                         M A I N
//------------------------------------------------------------------------------------------------------------------------------
void app_main() {
	//static httpd_handle_t server = NULL;
	httpd_uri_t restApis[RESTAPIS_MAX];

	// [!] YOU HAVE TO INITIALIZE THE DATA STRUCT ALWAYS
	restApiServer_poolInit(restApis);
	
	restApis[0].uri = "/status";
	restApis[0].method = HTTP_GET;
	restApis[0].handler = status_get_handler;
	restApis[0].user_ctx = NULL;
	
	restApis[1].uri = "/echo";
	restApis[1].method = HTTP_POST;
	restApis[1].handler = echo_post_handler;
	restApis[1].user_ctx = NULL;

	// REST APIs definitions acnowledge
	restApiServer_configure(restApis);
	

	if (nvs_flash_init() != ESP_OK)
		// ERROR!
		ESP_LOGE(__FILE__, "(%d) I cannot allocate NVS memory for the driver", __LINE__);

	else if (wifi_connToExtNet(HILO_SSIDNAME, HILO_PASSWORD, wifi_event_handler) == WERROR_SUCCESS)
		ESP_LOGI(__FILE__, "*** WiFi connected! (SSID:%s password:%s) ***", HILO_SSIDNAME, HILO_PASSWORD);
	
	return;
}
