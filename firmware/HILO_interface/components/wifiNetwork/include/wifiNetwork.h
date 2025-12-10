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
// Filename: wifiNetwork.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module contains all the functions used by the ESP32 to connect itself to the client using wireless
//		network or to become itself a wireless access point
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
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/
#ifndef __WIFINET__
#define __WIFINET__

#include <wError.h>
#include "esp_event.h"
#include "esp_wifi.h"

wError wifi_connToExtNet (const char *ssidStr, const char *pwdStr, esp_event_handler_t evh);
wError wifi_intAPstart   (const char *ssidStr, const char *pwdStr, esp_event_handler_t evh);

#endif
