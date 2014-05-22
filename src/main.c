#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "usart.h"

int main(void) {
    rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOD);
    usart_setup();

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
    gpio_toggle(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    int i;

    printf("Enter a number: ");
    scanf("%d", &i);
    printf("\r\nYou entered: %d\r\n", i);

    while(1) {
        gpio_toggle(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);
        for (i = 0; i < 6000000; i++) { __asm__("nop"); }
        printf("Test\r\n");
    }

    return 0;
}
