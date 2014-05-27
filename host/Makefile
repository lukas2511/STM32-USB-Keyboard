all: stlink/st-term st-wat

LIBUSB=$(shell pkg-config --cflags libusb-1.0) $(shell pkg-config --libs libusb-1.0)

st-wat: st-wat.c
	gcc -g -O2 $(LIBUSB) -o st-wat st-wat.c stlink/libstlink.a -Istlink/src

stlink/st-term:
	cd stlink; ./autogen.sh; ./configure; make

clean:
	cd stlink; make clean
	rm st-wat
