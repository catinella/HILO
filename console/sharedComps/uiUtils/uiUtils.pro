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
# Filename: uiUtils.pro
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

QMAKETOOLS_DIR  = "$$PWD/../../qmakeTools"
CONFIG_FILE     = "$$QMAKETOOLS_DIR/conf.pri"
TEMPLATE        = lib
SOURCES        += $$files($$PWD/*.cpp)
HEADERS        += $$files($$PWD/*.h)
CONFIG         += staticlib
QT             += widgets
TARGET          = uiUtils
DESTDIR         = $$PWD

exists($$CONFIG_FILE) {
	message("[i] configuration file $$CONFIG_FILE detected")
	include($$CONFIG_FILE)
}

# Settings by environmwent-vars
include("$$QMAKETOOLS_DIR/envVarOverriding.pri")

# Checking for GNU Debugger enabling setting
include("$$QMAKETOOLS_DIR/gdbToConfig.pri")

# Cleanall rule
include("$$QMAKETOOLS_DIR/cleanallRuleForLib.pri")

