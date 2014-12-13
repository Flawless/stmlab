# Compilator prefix
CROSS_PREFIX=arm-softfloat-linux-gnueabi-
# Compilators and utils
CC=$(CROSS_PREFIX)gcc
OBJCOPY=$(CROSS_PREFIX)objcopy
# Paths
STM_PATH=../../stlink/example
LIBS_STM_PATH=$(STM_PATH)/libs_stm
# Platform
PLATFORM=stm32f10x
# Primary compiler flags
CFLAGS=-g -O2 -mlittle-endian -mthumb -ffreestanding -nostdlib -nostdinc
CFLAGS+= -mcpu=cortex-m3 -DSTM32F10X_MD_VL

# Secondary compiler flags
CFLAGS+= -I.
CFLAGS+= -I$(LIBS_STM_PATH)/inc/base
CFLAGS+= -I$(LIBS_STM_PATH)/inc/core_support
CFLAGS+= -I$(LIBS_STM_PATH)/inc/device_support
CFLAGS+= -Wl,-T,stm32f100xb_flash.ld
# Linker flags
LDFLAGS=-L.

# Sources and objects
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

# Rules for compile
all: firmware.bin

%bin: %elf
	$(OBJCOPY) -O binary $^ $@

firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: $.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Rules for clean
clean:
	rm -rf *.o *.elf *.bin

# Rules for write firmware to mcu
write:
	st-flash write v1 firmware.bin 0x8000000

.PHONY: all clean
