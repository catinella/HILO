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
# Filename: PinStrip.h
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

CONFIG_FILE   = "$$PWD/../conf.pri"
TEMPLATE      = app
TARGET        = PinStrip_test
QT           += widgets testlib
INCLUDEPATH  += $$PWD $$PWD/.. $$PWD/../../PinWidget
LIBS         += -L$$PWD/../../PinWidget -lPinWidget
DESTDIR       = $$PWD
PINWIDGET_LIB = $$PWD/../../PinWidget/libPinWidget.a
SOURCES      += \
	$$PWD/*.cpp \
	$$PWD/../PinStrip.cpp \
	$$PWD/../ConnectionOverlay.cpp \
	$$PWD/../pinConnection.cpp
HEADERS      += \
	$$PWD/*.h \
	$$PWD/../PinStrip.h \
	$$PWD/../ConnectionOverlay.h \
	$$PWD/../pinConnection.h

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

#-------------------------------------------------------------------------------------------------------------------------------
#                                                E T R A   R U L E S
#-------------------------------------------------------------------------------------------------------------------------------

check_pinwidget.target = check_pinwidget
check_pinwidget.commands = @test -f $$PINWIDGET_LIB || ( \
    	echo "ERROR: Required library not found: $$PINWIDGET_LIB"; \
	echo "You have to compile it manually"; \
	false \
)

PRE_TARGETDEPS      += check_pinwidget
QMAKE_EXTRA_TARGETS += check_pinwidget

cleanall.target   = cleanall
cleanall.commands = $$escape_expand(@rm -fv $$TARGET Makefile)
cleanall.depends  = clean

QMAKE_EXTRA_TARGETS += cleanall

