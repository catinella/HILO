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
#	QMake setting to creates the Makefile. In this file you can find all rules to build the source dode
# 
#	TEMPLATE = {app|lib|subdirs|aux}
#		app     Executable file
#		lib     Library (lib.a, lib.so) file
#		subdirs It creates a Makefile in every sub-folder where there is a file.pro
#		aux     Script driven building process
#
#-------------------------------------------------------------------------------------------------------------------------------

CONFIG_FILE     = "$$PWD/../conf.pri"
TEMPLATE        = lib
SOURCES        += $$files($$PWD/*.cpp)
HEADERS        += $$files($$PWD/*.h)
INCLUDEPATH    += $$PWD $$PWD/../PinWidget
CONFIG         += staticlib
TARGET          = PinStrip
QT             += widgets
PRE_TARGETDEPS += $$PWD/../PinWidget/libPinWidget.a

exists($$CONFIG_FILE) {
	message("[i] configuration file $$CONFIG_FILE detected")
	include($$CONFIG_FILE)
} else {
	# Checking for environment variables
	GDB = $$(GDB)
}

equals(GDB, 1) {
	message("WARNING! You are building $$TARGET module in debug mode")
	CONFIG  += debug
	CONFIG  -= release
} else {
	message("[i] You are building $$TARGET in release mode")
	CONFIG  += release
	CONFIG  -= debug
}

cleanall.target   = cleanall
cleanall.commands = $$escape_expand(@rm -fv lib$${TARGET}.a Makefile)
cleanall.depends  = clean

QMAKE_EXTRA_TARGETS += cleanall
