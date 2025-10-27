
SRCs    := $(shell ls utest__*.c)
OBJs    := $(SRCs:%.c=%.o)
EXEs    := $(SRCs:%.c=%)

SUTobj   = testDataCompiler.o
SUTsrc  := $(SUTobj:%.o=%.c)
SUThead := $(SUTobj:%.o=%.h)

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
			@$(CC) $(CFLAGS) -c $< -o $@

utest__%:		utest__%.o $(SUTobj)
			@echo "[ LD ] $@"
			@$(CC) $(CFLAGS) $^ -o $@

$(SUTobj):		../$(SUTsrc) ../include/$(SUThead) $(dc_headsFile) $(dc_initCode)
			@echo "[ CC* ] $@"
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
			@rm -fv $(SUTobj) $(dc_headsFile) $(dc_initCode)
