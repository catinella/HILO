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
# Filename: DragController_test.pro
#
# Author:   Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	QMake setting to creates the Makefile. In this file you can find all rules to build the source dode
#
#	This test can be compiled to test the DragController in two differents way: active or passive mode.
# 	You can select the mode using the HILO_DRAG_MODE environment variable, its accepted values are "ACTIVE" or "PASSIVE"
#	use the following syntax:
#
#		HILO_DRAG_MODE={ACTIVE|PASSIVE} qmake6 *.pro && make
#
#-------------------------------------------------------------------------------------------------------------------------------

QMAKETOOLS_DIR  = "$$PWD/../../../qmakeTools"
CONFIG_FILE     = "$$QMAKETOOLS_DIR/conf.pri"
TEMPLATE        = app
SOURCES        += $$files($$PWD/*.cpp) $$files($$PWD/../DragController.cpp)
HEADERS        += $$files($$PWD/*.h) $$files($$PWD/../DragController.h)
QT             += widgets
TARGET          = DragController_test
INCLUDEPATH     = $$PWD/..
DESTDIR         = $$PWD

HILO_DRAG_MODE = $$(HILO_DRAG_MODE)
equals(HILO_DRAG_MODE, PASSIVE) {
	DEFINES += TEST_PASSIVEMODE
	message("HILO drag mode: PASSIVE")
} else: equals(HILO_DRAG_MODE, ACTIVE) {
	DEFINES += TEST_ACTIVEMODE
	message("HILO drag mode: ACTIVE")
} else {
	message("HILO drag mode not set, default ACTIVE")
	DEFINES += TEST_ACTIVEMODE
}

exists($$CONFIG_FILE) {
	message("[i] configuration file $$CONFIG_FILE detected")
	include($$CONFIG_FILE)
}

# Settings by environmwent-vars
include("$$QMAKETOOLS_DIR/envVarOverriding.pri")

# Checking for GNU Debugger enabling setting
include("$$QMAKETOOLS_DIR/gdbToConfig.pri")

# Cleanall rule definition
include("$$QMAKETOOLS_DIR/cleanallRuleForApp.pri")
