
MAKEFLAGS += -rR

CC   = gcc
GDB ?= 0

testSrcs := $(shell ls test_*.c)
testObjs := $(testSrcs:.c=.o)
testExes := $(testSrcs:.c=)

ifeq ($(GDB), 0)
CFLAGS = -o2
else
CFLAGS = -o0 -g
endif

INC = -I. -I../Inc $(shell pkg-config --cflags minuteC)

MINUTE_SCG  = $(shell pkg-config --variable=prefix minuteC)/share/minute/minute_cCodeGenerator.py

.SECONDARY:	$(wildcard *.sgc)
.PHONY:	all, clean, cleanall

all:		$(testExes)

test_%.o:	test_%.c ../Src/%.c ../Inc/%.h test_%.sgc
		@echo "[ CC ] $@"
		@$(CC) -Wall $(CFLAGS) $(INC) -c $< -o $@

test_%.sgc:	test_%.c
		@echo "[SCRIPT] $@"
		@$(MINUTE_SCG) --srcFilename=$< --tgtFilename=$@

test_%:	test_%.o %.o
		@echo "[ LD ] $@"
		@gcc -Wall -o $@ $^ $(shell pkg-config --static --libs minuteC)

%.o:		../Src/%.c ../Inc/%.h
		@echo "[ CC ] $@"
		@$(CC) -Wall $(CFLAGS) $(INC) -c $< -o $@

clean:
		@echo "[CLEAN]"
		@rm -fv *.o

cleanall:	clean
		@rm -fv $(testExes) *.sgc

