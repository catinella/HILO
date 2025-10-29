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

ifeq ($(GDB), 0)
	CFLAGS = -Wall
else
	CFLAGS = -Wall -g
endif

.PHONY: clean

all:			$(EXEs)

utest__%.o:		utest__%.c ../include/$(SUThead)
			@echo "[ CC ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

utest__%:		utest__%.o $(SUTobj) $(SMLOBJs)
			@echo "[ LD ] $@"
			@$(CC) $(CFLAGS) $^ -lcjson -o $@

$(SUTobj):		../$(SUTsrc) ../include/$(SUThead) $(dc_headsFile) $(dc_initCode)
			@echo "[ CC* ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

testData_%.o:	../testData_%.c ../include/testData_%.h
			@echo "[ CC ] $@"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(dc_headsFile):	$(headFiles)
			@echo "[SCRIPT] $@"
			@../buildingTools/inclusionProcCreator.sh --pathErasing --prefix=testData_ --path=../include --targetName=$@

$(dc_initCode):
			@echo "[SCRIPT] $@"
			@../buildingTools/initFunctCreator.sh --prefix=testData_ --path=../include --targetName=$@

clean:
			@rm -fv $(OBJs) $(EXEs) 

cleanall:		clean
			@rm -fv $(SUTobj) $(SMLOBJs) $(dc_headsFile) $(dc_initCode)

debug:
			@echo "Sub-modules: $(SMLOBJs)"
