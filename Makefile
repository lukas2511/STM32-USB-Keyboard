BINARY = src/main
OBJS += src/usb.o src/crc.o

ifdef DEBUG
CFLAGS += -DDEBUG
endif

ifdef USE_STTERM
OBJS += src/stlinky.o
CFLAGS += -DUSE_STTERM
else
OBJS += src/usart.o
endif

LDSCRIPT = stm32f4-discovery.ld
TOOLCHAIN_DIR = libopencm3

include Makefile.include
