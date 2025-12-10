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
// Filename: restApiServer.c
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module allows you to start a webserver inside your ESP, and transform your device in a REST-APIs server.
//		The dirty work has made by the ESP IDF library and almost everything is hidden. You can see the data flow in
//		the following ASCII diagram (by ChatGPT-5)
//
//
//	        [Client Browser / curl]
//	                  |
//	                  |
//	                  v
//	           TCP/IP Request
//	                  |
//	                  |
//	        +---------------------+
//	        |        LwIP         |      (TCP/IP stack)
//	        +---------------------+
//	                  |
//	                  |
//	             Socket API
//	         (accept/recv/send)
//	                  |
//	                  |
//	        +---------------------+
//	        |     HTTPD Task      |      crated by httpd_start()
//	        |    (http daemon)    |
//	        +---------------------+
//	                  |
//	                  |
//	            URI matching
//	     (httpd_register_uri_handler)
//	                  |
//	                  |
//	        +--------------------+
//	        |      Handler       |
//	        +--------------------+
//	                  |
//	                  |
//	      httpd_resp_send(req, ...)
//	                  |
//	                  |
//	             Socket API
//	                  |
//	        +---------------------+
//	        |        LwIP         |
//	        +---------------------+
//	                  |
//	                  |
//	           TCP/IP Response
//	                  |
//	                  |
//	        [Client Browser / curl]
//	
//
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
//#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "esp_timer.h"

#include <wError.h>
#include <restApiServer.h>

static httpd_handle_t server = NULL;
static httpd_uri_t    restApisPool[RESTAPIS_MAX];
static bool           initFlag = false;

/*
static void restApiServer_printApisList() {
	//
	// Description:
	//	It print the REST APIs list in the debug console. It has been developed just FOR DEBUG PURPOSE!
	//
	for (uint8_t x = 0; restApisPool[x].uri != NULL; x++) {
		ESP_LOGW(__FILE__, "%d) %s", x, restApisPool[x].uri);
	}
	return;
}
*/

void restApiServer_poolInit (httpd_uri_t *restApis) {
	//
	// Description:
	//	It initializes the argument defined list of structures
	//
	for (uint8_t t=0; t<RESTAPIS_MAX; t++)
		restApis[t].uri = NULL;
	return;
}
	
wError restApiServer_configure (const httpd_uri_t *restApis) {
	//
	// Description:
	//	It copies the argument defined APIs definitions in the module's internal memory
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_ERROR_DATAOVERFLOW
	//
	uint8_t x = 0;
	wError  err = WERROR_SUCCESS;
	
	if (initFlag == false) {
		restApiServer_poolInit(restApisPool);
		initFlag = true;
	}
	
	while (restApis[x].uri != NULL) {
		if (x < RESTAPIS_MAX) {
			ESP_LOGI(__FILE__, "%s definition recording...", restApis[x].uri);
			restApisPool[x] = restApis[x];
			x++;
		} else {
			// ERROR!
			ESP_LOGE(__FILE__, "Too many APIs definitions!!");
			err = WERROR_ERROR_DATAOVERFLOW;
		}
	}
	return(err);
}

	
wError restApiServer_start() {
	//
	// Description:
	//	This function starts the webserver and configures it to manage the incoming requests by REST-APIs
	//
	// Returned value:
	//	WERROR_SUCCESS
	//	WERROR_WARNING_RESBUSY
	//	WERROR_WARNING_EMPTYLIST
	//	WERROR_ERROR_INTFAILURE
	//	WERROR_ERROR_REGPROCFAILED
	//
	wError         err    = WERROR_SUCCESS;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();

	if (server != NULL) {
		// WARNING!
		ESP_LOGW(__FILE__, "The webserver is already running");
		err = WERROR_WARNING_RESBUSY;
		
	} else if (httpd_start(&server, &config) != ESP_OK) {
		// ERROR!
		ESP_LOGE(__FILE__, "HTTP server starting failed");
		err = WERROR_ERROR_INTFAILURE;
		
	} else {
		uint8_t x = 0;

		while (restApisPool[x].uri != NULL) {
			if (httpd_register_uri_handler(server, &(restApisPool[x])) == ESP_OK) {
				// SUCCESS
				ESP_LOGI(__FILE__, "\"%s\" REST-API successfully registered", restApisPool[x].uri);
				x++;
			} else {
				// ERROR!
				ESP_LOGE(__FILE__, "Handler registration failed");
				err = WERROR_ERROR_REGPROCFAILED;
				break;
			}
		}

		if (x == 0) {
			// WARNING!
			ESP_LOGW(__FILE__, "HTTP server is running without REST APIs definitions");
			err = WERROR_WARNING_EMPTYLIST;
		} else
			ESP_LOGI(__FILE__, "HTTP server is running and accepts %d rest-apis", x);
	}
	
	return(err);
}

void restApiServer_stop() {
	//
	// Description:
	//	It stops the web-server
	//
	if (server != NULL) {
		httpd_stop(server);
		server = NULL;
	}
	return;
}
