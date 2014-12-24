CC	 = $(PREFIX)-gcc
AR	 = $(PREFIX)-ar

vpath %.c src usb/src 

CFLAGS  = -O2  -Wall
CFLAGS += -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections
CFLAGS +=  $(MCU_CC_FLAGS)

#CFLAGS += -ffreestanding -nostdlib
CFLAGS += -Iinc -Iinc/core -Iusb/inc $(TARGET)

#SRCS = misc.c stm32l1xx_syscfg.c startup_stm32l1xx_md.c system_stm32l1xx.c stm32l1xx_tim.c stm32l1xx_dma.c stm32l1xx_adc.c stm32l1xx_gpio.c stm32l1xx_rcc.c
SRCS = $(wildcard src/*.c)
SRCS += $(wildcard usb/src/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: libstm32l1.a

all: libstm32l1.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32l1.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32l1.a

print-%  : ; @echo $* = $($*)
