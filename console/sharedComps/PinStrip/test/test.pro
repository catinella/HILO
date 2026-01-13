#-------------------------------------------------------------------------------------------------------------------------------
#                                                    _   _ ___ _     ___  
#                                                   | | | |_ _| |   / _ \
#                                                   | |_| || || |  | | | |
#                                                   |  _  || || |__| |_| |
#                                                   |_| |_|___|_____\___/ 
#                                                    Hardware in the loop
#                                              (https://github.com/catinella/HILO)
#
#
#
#
# Filename: test.pro
#
# Author:   Silvano Catinella <catinella@yahoo.com>
#
# Description:
#
#
#
# License:  LGPL ver 3.0
#
# 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
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
#                                                                                                               cols=128 tab=6
#-------------------------------------------------------------------------------------------------------------------------------

QMAKETOOLS_DIR  = "$$PWD/../../../qmakeTools"
CONFIG_FILE     = "$$QMAKETOOLS_DIR/conf.pri"
TEMPLATE        = app
TARGET          = PinStrip_test
QT             += widgets testlib
DESTDIR         = $$PWD
PRE_TARGETDEPS +=                          \
	$$PWD/../../PinWidget/libPinWidget.a \
	$$PWD/../../uiUtils/libuiUtils.a
INCLUDEPATH +=              \
	$$PWD $$PWD/..        \
	$$PWD/../../PinWidget \
	$$PWD/../../uiUtils
LIBS +=                                   \
	-L$$PWD/../../PinWidget -lPinWidget \
	-L$$PWD/../../uiUtils   -luiUtils
SOURCES +=                           \
	$$PWD/*.cpp                    \
	$$PWD/../PinStrip.cpp          \
	$$PWD/../ConnectionOverlay.cpp \
	$$PWD/../pinConnection.cpp
HEADERS +=                         \
	$$PWD/*.h                    \
	$$PWD/../PinStrip.h          \
	$$PWD/../ConnectionOverlay.h \
	$$PWD/../pinConnection.h

include("$$QMAKETOOLS_DIR/utils.pri")

exists($$CONFIG_FILE) {
	message("[i] configuration file $$CONFIG_FILE detected")
	include($$CONFIG_FILE)
}

# Checking for dependences
checkPreTargetDepsExist() {} else { error("Test failed") }

# Settings by environmwent-vars
include("$$QMAKETOOLS_DIR/envVarOverriding.pri")

# Checking for GNU Debugger enabling setting
include("$$QMAKETOOLS_DIR/gdbToConfig.pri")

# Cleanall rule definition
include("$$QMAKETOOLS_DIR/cleanallRuleForApp.pri")
