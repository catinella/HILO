#------------------------------------------------------------------------------------------------------------------------------
#                                                    _   _ ___ _     ___  
#                                                   | | | |_ _| |   / _ \
#                                                   | |_| || || |  | | | |
#                                                   |  _  || || |__| |_| |
#                                                   |_| |_|___|_____\___/ 
#                                                    Hardware in the loop
#                                              (https://github.com/catinella/HILO)
#
# Filename: CMakeLists.txt
#
# Author:   Silvano Catinella <catinella@yahoo.com>
#
# Description:
#		This file is a set of cmake rules to build the firmware
#
# License:  LGPL ver 3.0
#
# 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
# 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
# 		or (at your option) any later version. 
#
#		For further details please read the full LGPL text file  "Linuxwoodo/trunk/templates/lgpl-3.0.txt".
# 		You should have received a copy of the GNU General Public License along with this file; 
# 		if not, write to the 
#
#			Free Software Foundation, Inc.,
#			59 Temple Place, Suite 330,
#			Boston, MA  02111-1307  USA
#
#
#
#                                                                                                               cols=128 tab=6
#------------------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(IDF_TARGET esp32)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(HILO_interface)
