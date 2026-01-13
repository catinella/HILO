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
# Filename: PinStrip.pro
#
# Author:   Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	QMake setting to creates the Makefile. In this file you can find all rules to build the source code
# 
# 	This class depends by the following modules. You need to compile them before
# 		- PinWidget
# 		- uiUtils
#
#	TEMPLATE = {app|lib|subdirs|aux}
#		app     Executable file
#		lib     Library (lib.a, lib.so) file
#		subdirs It creates a Makefile in every sub-folder where there is a file.pro
#		aux     Script driven building process
#
#-------------------------------------------------------------------------------------------------------------------------------

QMAKETOOLS_DIR  = "$$PWD/../../qmakeTools"
CONFIG_FILE     = "$$QMAKETOOLS_DIR/conf.pri"
TEMPLATE        = lib
SOURCES        += $$files($$PWD/*.cpp)
HEADERS        += $$files($$PWD/*.h)
INCLUDEPATH    += $$PWD $$PWD/../PinWidget $$PWD/../uiUtils
CONFIG         += staticlib
TARGET          = PinStrip
QT             += widgets
PRE_TARGETDEPS += $$PWD/../PinWidget/libPinWidget.a $$PWD/../uiUtils/libuiUtils.a
ENVVARSLIST     = GDB
DESTDIR         = $$PWD

include("$$QMAKETOOLS_DIR/utils.pri")

# Configuration loading...
exists($$CONFIG_FILE) {
	message("[i] configuration file $$CONFIG_FILE detected")
	include($$CONFIG_FILE)
}

# Checking for dependences
checkPreTargetDepsExist() {} else {error("Test failed")}

# Settings by environmwent-vars
include("$$QMAKETOOLS_DIR/envVarOverriding.pri")

# Checking for GNU Debugger enabling setting
include("$$QMAKETOOLS_DIR/gdbToConfig.pri")

# Cleanall rule
include("$$QMAKETOOLS_DIR/cleanallRuleForLib.pri")
