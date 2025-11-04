#-------------------------------------------------------------------------------------------------------------------------------
#                                                    _   _ ___ _     ___  
#                                                   | | | |_ _| |   / _ \
#                                                   | |_| || || |  | | | |
#                                                   |  _  || || |__| |_| |
#                                                   |_| |_|___|_____\___/ 
#                                                    Hardware in the loop
#
#
# File:   Makefile
#
# Author: Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	
#	
#	
#-------------------------------------------------------------------------------------------------------------------------------

SRCs    := $(shell ls utest__*.c)
OBJs    := $(SRCs:%.c=%.o)
EXEs    := $(SRCs:%.c=%)

SMSRCs  := $(shell ls ../testData_*.c)
SMOBJs  := $(SMSRCs:%.c=%.o)
SMLOBJs := $(notdir $(SMOBJs))

SUTobj   = testDataCompiler.o
SUTsrc  := testDataCompiler.c
SUThead := testDataCompiler.h

dc_headsFile = testDataCompiler_subModsHeaders.h
dc_initCode  = testDataCompiler_subModsRegCalls.dc
headFiles   := $(shell ls ../include/testData_*.h)

GDB     ?= 0
INCLUDE  = -I./ -I../include -I../../crossPlatform_code
SYMBOLS  = "-DMOCK=1"

ifeq ($(GDB), 0)
	CFLAGS = -Wall
else
	CFLAGS = -Wall -g
endif

.PHONY: clean cleanall all debug

all:			$(EXEs) dataViewer

dataViewer.o:	dataViewer.c
			@echo "[ CC(4) ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

dataViewer:		dataViewer.o
			@echo "[ LD(1) ] $@"
			@$(CC) $(CFLAGS) $^ -lncurses -o $@

utest__%.o:		utest__%.c ../include/$(SUThead)
			@echo "[ CC(0) ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

utest__%:		utest__%.o $(SUTobj) $(SMLOBJs) utilities.o
			@echo "[ LD(0) ] $@"
			@$(CC) $(CFLAGS) $^ -lcjson -lm -o $@

$(SUTobj):		../$(SUTsrc) ../include/$(SUThead) $(dc_headsFile) $(dc_initCode)
			@echo "[ CC(1) ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) $(SYMBOLS) -c $< -o $@

utilities.o:	../utilities.c ../include/utilities.h
			@echo "[ CC(2) ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) $(SYMBOLS) -c $< -o $@

testData_%.o:	../testData_%.c ../include/testData_%.h
			@echo "[ CC(3) ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(dc_headsFile):	$(headFiles)
			@echo "[SCRIPT] $@"
			@../buildingTools/inclusionProcCreator.sh --pathErasing --prefix=testData_ --path=../include --targetName=$@

$(dc_initCode):
			@echo "[SCRIPT] $@"
			@../buildingTools/initFunctCreator.sh --prefix=testData_ --path=../include --targetName=$@

clean:
			@rm -fv $(OBJs) $(EXEs) utilities.o dataViewer.o

cleanall:		clean
			@rm -fv $(SUTobj) $(SMLOBJs) $(dc_headsFile) $(dc_initCode) dataViewer

debug:
			@echo "Sub-modules: $(SMLOBJs)"
