#------------------------------------------------------------------------------------------------------------------------------
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
# File:   Makefile
#
# Author: Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	This file allows you to build all unit tests stored in this folder.
#	use:  make [GDB=1]
#
#------------------------------------------------------------------------------------------------------------------------------
SRCs  := $(shell ls *.c)
OBJs  := $(SRCs:.c=.o)
BIN    = unitTests
MODULE = sram_manager
GDB   ?= 0

ifeq ($(GDB), 1)
	CCOPTS = -g -o0 -Wall -Wconversion 
else
	CCOPTS =    -o2 -Wall -Wconversion 
endif

INCOPTS = -I. -I../include -I../../crossPlatform_code
SYMBOLS = -DMOCK=1


.PHONY: all clean

all:			$(BIN)

$(BIN):		$(OBJs) $(MODULE).o
			@echo "[LD] $@"
			@$(CC) $(CCOPTS) -o $@ $^
		
%.o:			%.c
			@echo "[CC] $@"
			@$(CC) $(CCOPTS) -c $(SYMBOLS) $(INCOPTS) -o $@ $<

$(MODULE).o:	../$(MODULE).c ../include/$(MODULE).h
			@echo "[CC] $@"
			@$(CC) $(CCOPTS) -c $(SYMBOLS) $(INCOPTS) -o $@ $<

clean:
			@rm -rvf $(OBJs)

cleanall:		clean
			@rm -fv $(BIN) $(MODULE).o

debug:
			@echo "SRCs = \"$(SRCs)\""
			@echo "OBJs = \"$(OBJs)\""
