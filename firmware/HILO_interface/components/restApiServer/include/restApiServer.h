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
// Filename: restApiServer.h
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This module allows you to start a webserver inside your ESP, and transform your device in a REST-APIs server.
//
//		Symbols:
//			RESTAPIS_MAX    Maximum number of the defined API
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
#ifndef __RAPISVR__
#define __RAPISVR__


#define RESTAPIS_MAX 4

void   restApiServer_poolInit  (httpd_uri_t *restApis);
wError restApiServer_configure (const httpd_uri_t *restApiConf);
wError restApiServer_start     ();
void   restApiServer_stop      ();

#endif
