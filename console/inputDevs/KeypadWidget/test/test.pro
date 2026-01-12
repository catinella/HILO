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

CONFIG_FILE     = $$PWD/../../../conf.pri
TEMPLATE        = app
SOURCES        += $$PWD/*.cpp $$PWD/../KeypadWidget.cpp
HEADERS        += $$PWD/../KeypadWidget.h
FORMS          += $$PWD/../KeypadWidget.ui
QT             += widgets
TARGET          = KeypadWidget_test
DESTDIR         = $$PWD
INCLUDEPATH +=                             \
	$$PWD/..                             \
	$$PWD/../../../sharedComps/PinStrip  \
	$$PWD/../../../sharedComps/PinWidget \
	$$PWD/../../../sharedComps/uiUtils
LIBS +=                                                  \
	-L$$PWD/../../../sharedComps/PinStrip  -lPinStrip  \
	-L$$PWD/../../../sharedComps/PinWidget -lPinWidget \
	-L$$PWD/../../../sharedComps/uiUtils   -luiUtils
PRE_TARGETDEPS +=                                         \
	$$PWD/../../../sharedComps/PinStrip/libPinStrip.a   \
	$$PWD/../../../sharedComps/PinWidget/libPinWidget.a \
	$$PWD/../../../sharedComps/uiUtils/libuiUtils.a

include("$$PWD/../../../utils.pri")

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
include("$$PWD/../../../envVarOverriding.pri")

# Checking for GNU Debugger enabling setting
include("$$PWD/../../../gdbToConfig.pri")


cleanall.target   = cleanall
cleanall.commands = $$escape_expand(@rm -fv $$DESTDIR/$$TARGET Makefile)
cleanall.depends  = clean

QMAKE_EXTRA_TARGETS += cleanall
