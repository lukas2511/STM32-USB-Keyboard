#include <stdlib.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#define VENDOR_ID 0x1337
#define PRODUCT_ID 0x7331

void usb_setup(void);
void send_test(void);
void usb_poll(void);
void usb_send_packet(const void *buf, int len);
uint8_t *USBD_HID_GetStruct (uint8_t key0, uint8_t modifier);

