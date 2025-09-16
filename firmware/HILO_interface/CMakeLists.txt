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
#		This file is a set of cmake rules to build the firmware using idf.py Espressif official tool.
#		The code has been developed using the ESP-IDF ver. 5.4.2
#
#		Configuration process:
#		======================
#		To configure the cource code, set the following values in the configuration file (CMakeLists.conf).
#			FIRMWARETEST=<source file> It allows you to build an integration test instead the prod-firmware.
#			DEBUG=<n>                  It sets the debug messages verbosity
#
#		[!] In order to get the list of the available integration tests take a look into the "main" folder
#			ls -l <project home>/main/iTest__*.c
#                                                                                                               cols=128 tab=6
#------------------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(IDF_TARGET esp32)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(HILO_interface)
