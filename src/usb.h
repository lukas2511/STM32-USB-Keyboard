#include <stdlib.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/f4/nvic.h>

#define VENDOR_ID 0x6666
#define PRODUCT_ID 0x1

void usb_setup(void);
void send_test(void);
void usb_send_packet(const void *buf, int len);

extern volatile uint32_t last_usb_request_time;
extern uint8_t usb_ready;
