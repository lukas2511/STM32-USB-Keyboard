#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "crc.h"
#include "usart.h"
#include "usb.h"

// 0: 'S'
// 1-8: KEYS
// 9: 'E'
// 10: CRC
char INPUT[11];

char KEYS[8];
char EMPTY[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static void sendkeys(void){
    usb_send_packet(KEYS, 8);
//    usb_send_packet(EMPTY, 8);
}

int main(void) {
    rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOD);

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    usart_setup();
    usb_setup();

    gpio_toggle(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    uint32_t i=0;
    uint32_t k=0;
    uint32_t tmp=0;
    char crc=0;

    while(1) {
        i++;
        if(usart_has_data()){
            INPUT[k] = usart_read();
            k++;
            if(k==1 && INPUT[0]!=0x53){
                printf("ERROR AT START-BYTE %d\r\n",INPUT[0]);
                k=0;
            }
            if(k==10 && INPUT[9]!=0x45){
                printf("ERROR AT END-BYTE %d\r\n",INPUT[8]);
                k=0;
            }
            if(k==11){
                for(tmp=0;tmp<8;tmp++) KEYS[tmp] = INPUT[tmp+1];
                crc = getCRC(KEYS,8);
                if(crc==INPUT[10]){
                    printf("INPUT OK!\r\n");
                    sendkeys();
                }else{
                    printf("ERROR AT CRC-BYTE %d vs %d\r\n",INPUT[10],crc);
                }
                k=0;
            }
//            sendascii();

        }
        if(i>1000000){
            gpio_toggle(GPIOD, GPIO13);
            i=0;
        }
    }

    return 0;
}
