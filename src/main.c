#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "usart.h"
#include "usb.h"
#include "keycodes.h"

static void sendascii(void){
    uint8_t key0, modifier;
    char input = usart_read();
    usbHidKeyCodeFromAsciiChar(input, &key0, &modifier);
    uint8_t *buf = USBD_HID_GetStruct(key0, modifier);
    usb_send_packet(buf, 8);

    buf[0] = 0;
    buf[2] = 0;
    usb_send_packet(buf, 8);

    printf("Char: %d\r\nKeycode: %d\r\n\r\n", input, key0);
}

int main(void) {
    rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOD);

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    usart_setup();
    usb_setup();

    gpio_toggle(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    uint32_t i=0;

    while(1) {
        usb_poll();
        i++;
        if(usart_has_data()){
            sendascii();

        }
        if(i>1000000){
            gpio_toggle(GPIOD, GPIO13);
            i=0;            
        }
    }

    return 0;
}
