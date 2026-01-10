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
# Filename: mainApp.pro
#
# Author:   Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	QMake setting to creates the Makefile. In this file you can find all rules to build the source dode
# 
#	TEMPLATE = {app|lib|subdirs|aux}
#		app     Executable file
#		lib     Library (lib.a, lib.so) file
#		subdirs It creates a Makefile in every sub-folder where there is a file.pro
#		aux     Script driven building process
#
#-------------------------------------------------------------------------------------------------------------------------------

CONFIG_FILE     = $$PWD/../conf.pri
TEMPLATE        = app
SOURCES        += $$files($$PWD/*.cpp)
HEADERS        += $$files($$PWD/*.h)
INCLUDEPATH    += $$PWD/../KeypadWidget
QT             += widgets
TARGET          = HILO_console
DESTDIR         = ../
ENVVARSLIST     = GDB
LIBS           +=                                      \
	-L$$PWD/../inputDevs/KeypadWidget -lKeypadWidget \
	-L$$PWD/../sharedComps/uiUtils    -luiUtils      \
	-L$$PWD/../sharedComps/PinWidget  -lPinWidget    \
	-L$$PWD/../sharedComps/PinStripe  -lPinStripe
PRE_TARGETDEPS +=                                      \
	$$PWD/../sharedComps/uiUtils/libuiUtils.a        \
	$$PWD/../sharedComps/PinWidget/libPinWidget.a    \
	$$PWD/../sharedComps/PinStripe/libPinStripe.a

include("$$PWD/../utils.pri")

# Configuration loading...
exists($$CONFIG_FILE) {
	message("[i] configuration file $$CONFIG_FILE detected")
	include($$CONFIG_FILE)
}

# Checking for dependences
checkPreTargetDepsExist() {} else {
	error("Test failed")
}

# Settings by environmwent-vars
include("$$PWD/../envVarOverriding.pri")

# Checking for GNU Debugger enabling setting
include("$$PWD/../gdbToConfig.pri")
message("CONFIG=$$CONFIG")

#
# Customized rules
#
cleanall.target   = cleanall
cleanall.commands = $$escape_expand(@rm -fv lib$${TARGET}.a Makefile)
cleanall.depends  = clean

QMAKE_EXTRA_TARGETS += cleanall
