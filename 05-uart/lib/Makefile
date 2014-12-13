CC	 = $(PREFIX)-gcc
AR	 = $(PREFIX)-ar

vpath %.c src src/peripherals

CFLAGS  = -Wall
CFLAGS += -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections
CFLAGS +=  $(MCU_CC_FLAGS)

#CFLAGS += -ffreestanding -nostdlib
CFLAGS += -Iinc -Iinc/core -Iinc/peripherals

SRCS = system_stm32l1xx.c stm32l1xx_gpio.c stm32l1xx_rcc.c misc.c stm32l1xx_syscfg.c startup_stm32l1xx_md.c stm32l1xx_usart.c stm32l1xx_adc.c

OBJS = $(SRCS:.c=.o)

.PHONY: libstm32l1.a

all: libstm32l1.a

%.o : %.c
	$(CC) $(CFLAGS) $(OPTIMIZATION_LEVEL) -c -o $@ $^

libstm32l1.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32l1.a
