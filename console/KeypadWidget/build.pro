#-------------------------------------------------------------------------------------------------------------------------------
# 
#	TEMPLATE = {app|lib|subdirs|aux}
#		app     Executable file
#		lib     Library (lib.a, lib.so) file
#		subdirs It creates a Makefile in every sub-folder where there is a file.pro
#		aux     Script driven building process
#
#-------------------------------------------------------------------------------------------------------------------------------
TEMPLATE = lib
SOURCES += $$files($$PWD/*.cpp)
HEADERS += $$files($$PWD/*.h)
FORMS   += $$files($$PWD/*.ui)
CONFIG  += staticlib
TARGET   = KeypadWidget
QT      += widgets
GDB      = $$(GDB)

equals(GDB, 1) {
	message(WARNING! You are in debug mode)
	CONFIG  += debug
	CONFIG  -= release
} else {
	message([i] You are in release mode)
}

cleanall.target   = cleanall
cleanall.commands = $$escape_expand(@rm -fv lib$${TARGET}.a Makefile)
cleanall.depends  = clean

QMAKE_EXTRA_TARGETS += cleanall
