#-------------------------------------------------------------------------------------------------------------------------------
#
# Description:
#	Makefile for STM32F031K6 (NUCLEO-F031K6)
#	Requirements: arm-none-eabi-gcc, openocd o st-flash
#
#	Configurable symbols:
#		GDB=1             It enables debug support
#		OCD_PROGRAMMER=1  It uses the open-OCD programmer instead the ST one
#
#-------------------------------------------------------------------------------------------------------------------------------

-include conf.mk

ifndef GNUTOOLS_PATH
$(error GNUTOOLS_PATH is not defined into the conf.mk file!)
endif

#
# MCU & Toolchain
#
MCU_FAMILY       := STM32F0xx
PART             := STM32F031x6
CPU              := -mcpu=cortex-m0
FPU              :=
FLOAT_ABI        :=
MCUFLAGS         := $(CPU) -mthumb $(FPU) $(FLOAT_ABI)

#
# Process' configuration
#
GDB              ?= 0
OCD_PROGRAMMER   ?= 0
TARGET           ?= firmware

#
# Executable files
#
TOOLCHAIN_PREFIX := $(GNUTOOLS_PATH)/arm-none-eabi
CC               := $(TOOLCHAIN_PREFIX)-gcc
AS               := $(TOOLCHAIN_PREFIX)-gcc
AR               := $(TOOLCHAIN_PREFIX)-ar
LD               := $(TOOLCHAIN_PREFIX)-ld
OBJCOPY          := $(TOOLCHAIN_PREFIX)-objcopy
SIZE             := $(TOOLCHAIN_PREFIX)-size


# --- Sorgenti ---
SRC_DIRS := \
  ../Core/Src \
  ../Drivers/STM32F0xx_HAL_Driver/Src

STARTUP  := ../Core/Startup/startup_stm32f031k6tx.s
SYSTEM   := ../Core/Src/system_stm32f0xx.c

# Linker script
LDSCRIPT := $(abspath ../STM32F031K6TX_FLASH.ld)

# --- Include ---
INCLUDES := \
  -I../Core/Inc \
  -I../Drivers/CMSIS/Include \
  -I../Drivers/CMSIS/Device/ST/$(MCU_FAMILY)/Include \
  -I../Drivers/$(MCU_FAMILY)_HAL_Driver/Inc

# --- Defines ---
DEFS := -DUSE_HAL_DRIVER -D$(MCU_FAMILY) -D$(PART)

# --- Flags ---
CSTD   := -std=c11
OPT    := -O2
WARN   := -Wall -Wextra -Wshadow -Wlogical-op -Wpointer-arith -Wstrict-prototypes
CFLAGS := $(CSTD) $(OPT) $(WARN) $(DEFS) $(INCLUDES) $(MCUFLAGS) -ffunction-sections -fdata-sections -fno-common
ASFLAGS:= $(MCUFLAGS) -x assembler-with-cpp
LDFLAGS:= $(MCUFLAGS) -Wl,--gc-sections -Wl,-Map,$(TARGET).map -T$(LDSCRIPT)

# Rimuovi eventuali sorgenti HAL che non usi (es. LL) se vuoi snellire
DRVR_SRCS := $(shell find ../Drivers -name '*.c' -not -path '*/Templates/*')
CORE_SRCS := $(shell find ../Core/Src -name '*.c')
ASMS := $(STARTUP)

TSRCS := $(patsubst ../%,%,$(DRVR_SRCS)) $(patsubst ../%,%,$(CORE_SRCS))
TASMS := $(patsubst ../%,%,$(ASMS))

COBJS := $(TSRCS:.c=.o)
DOBJS := $(TSRCS:.c=.d)
AOBJS := $(TASMS:.s=.o)
DEPS  := $(TCOBJS:.o=.d)

VPATH := ..

.PHONY: all clean flash size

#-------------------------------------------------------------------------------------------------------------------------------
#                               C O M P I L I N G   A N D   B U I L D I N G   R U L E S
#-------------------------------------------------------------------------------------------------------------------------------

all:			$(TARGET).elf $(TARGET).bin $(TARGET).hex size

%.o:			%.c
			@echo "[CC] $@"
			@mkdir -p $(dir $@)
			@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o:			%.s
			@echo "[ASM] $@"
			@mkdir -p $(dir $@)
			@$(AS) $(ASFLAGS) -c $< -o $@

$(TARGET).elf:	$(COBJS) $(AOBJS)
			@echo "[LD] $@"
ifeq ($(GDB), 0)
			@$(CC) $(COBJS) $(LDFLAGS) -o $@
else
			$(TOOLCHAIN_PREFIX)-gdb $<
endif

$(TARGET).bin:	$(TARGET).elf
			@echo "[CONV] $@"
			@$(OBJCOPY) -O binary $< $@

$(TARGET).hex:	$(TARGET).elf
			@echo "[CONV] $@"
			@$(OBJCOPY) -O ihex $< $@

size:			$(TARGET).elf
			@echo "[INFO] $@"
			$(SIZE) --format=berkeley $<

cleanall:		clean
			@for ext in elf bin hex map; do \
				rm -fv $(TARGET).$$ext;   \
			done
			@for dir in $$(find . -type d -printf '%P\n'); do \
				rmdir -pv $$dir 2>/dev/null; \
			done

clean:
			@rm -fv $(COBJS) $(DOBJS) $(AOBJS)

ifeq ($(OCD_PROGRAMMER), 1)
flash:		$(TARGET).elf
			openocd -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $< verify reset exit"
else
flash:		$(TARGET).bin
			st-flash write $< 0x08000000
endif

help:
			@echo "------------ C objects ------------"
			@for file in "$(COBJS)"; do echo "$$file"; done
			@echo "-------- Assembly objects ---------"
			@for file in "$(AOBJS)"; do echo "$$file"; done


#
# Self generating compiling rules inclusion
#
-include $(DEPS)

