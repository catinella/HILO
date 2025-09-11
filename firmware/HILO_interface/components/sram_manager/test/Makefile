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
#	use:  make [options]
#		GDB={0|1}         It disable/enables the debug support
#		VERBOSE={0-2}     It sets the test output verbosity
#		T1_NUMOFITEMS=<n> It sets the number of foo-data items the test T1 will use
#
#------------------------------------------------------------------------------------------------------------------------------
SRCs          := $(shell ls *.c)
OBJs          := $(SRCs:.c=.o)
BIN            = unitTests
MODULE         = sram_manager
GDB           ?= 0
VERBOSE       ?= 0
T1_NUMOFITEMS ?= 1048568

ifeq ($(GDB), 1)
	CCOPTS = -g -o0 -Wall -Wconversion 
else
	CCOPTS =    -o2 -Wall -Wconversion 
endif

INCOPTS = -I. -I../include -I../../crossPlatform_code
SYMBOLS =                               \
	"-DMOCK=1"                        \
	"-DVERBOSE_UNITTEST=$(VERBOSE)"   \
	"-DFOODATA_T1NOI=$(T1_NUMOFITEMS)"



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
			@echo "SRCs = \"$(SRCs)\"";                   \
			echo "OBJs = \"$(OBJs)\"";                    \
			echo "T1_NUMOFITEMS = \"$(T1_NUMOFITEMS)\"";
