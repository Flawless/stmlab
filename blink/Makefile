# HARDWARE
PREFIX=arm-none-eabi
MCU_CC_FLAGS = -mcpu=cortex-m3 -mthumb -Wall
MCU_LD_FLAGS = #-Dstm32l152rb.ld
CC	 = $(PREFIX)-gcc
CPP	 = $(PREFIX)-g++
AS       = $(PREFIX)-as
LD       = $(PREFIX)-gcc
OBJCOPY  = $(PREFIX)-objcopy
SIZE	 = $(PREFIX)-size
# SYSTEM
SYS_LIBRARIES = -lstdc++ -lsupc++ -lm -lc -lg -lnosys
SYS_LD_FLAGS  = 
# PROJECT
# structure
SRCS  = ./src/*
PROJECT_LIB_PATHS = -L./lib
PROJECT_INC_PATHS = -I./inc -I./lib/inc -I./lib/inc/core
OUTPATH = ./build
# 
PROJECT = sample
CC_OPTIM_FLAGS =
CC_DEBUG_FLAGS =
#
INCLUDE_PATHS = $(PROJECT_INC_PATHS)
LIBRARY_PATHS = $(PROJECT_LIB_PATHS)
CFLAGS = $(MCU_CC_FLAGS) $(CC_OPTIM_FLAGS) $(CC_DEBUG_FLAGS) $(PROJECT_LIB_PATHS) $(PROJECT_INC_PATHS) -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -mlittle-endian
LD_FLAGS = $(MCU_CC_FLAGS) $(MCU_LD_FLAGS) -Wl,--gc-sections $(SYS_LD_FLAGS) -Wl,-Map=$(PROJECT).map
LD_SYS_LIBS = $(SYS_LIBRARIES)

BULD_TARGET = $(PROJECT)

# EXECUTIONS
.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJECT).elf

clean:
	rm -f $(OUTPATH)/*
	$(MAKE) -C lib clean
lib:
	$(MAKE) -C lib PREFIX=$(PREFIX) MCU_CC_FLAGS="$(MCU_CC_FLAGS)"

proj: 	$(OUTPATH)/$(PROJECT).elf

$(OUTPATH)/$(PROJECT).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -lstm32l1 -lm -O2 -Tstm32_flash.ld # -Dstm32l152rb.ld -Tstm32l152rb.ld #
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJECT).elf $(OUTPATH)/$(PROJECT).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJECT).elf $(OUTPATH)/$(PROJECT).bin

flash:
	st-flash write $(OUTPATH)/$(PROJECT).bin 0x08000000
erase:
	st-flash erase
