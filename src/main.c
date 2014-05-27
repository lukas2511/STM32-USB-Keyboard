#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "crc.h"
#include "usb.h"

#ifdef USE_STTERM
#include "stlinky.h"
#include <stdio.h>
#else
#include "usart.h"
#endif

// 0: 'S'
// 1-8: KEYS
// 9: 'E'
// 10: CRC
char INPUT[11];

char KEYS[8];

static void sendkeys(void){
    #ifdef DEBUG
    printf("Sending package... ");
    #endif
    usb_send_packet(KEYS, 8);
    #ifdef DEBUG
    printf("Done!\n");
    #endif
}

int main(void) {
    rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOD);

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    #ifdef USE_STTERM
    stlinky_init();
    #else
    usart_setup();
    #endif

    usb_setup();

    gpio_toggle(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    uint32_t i=0;
    uint32_t k=0;
    uint32_t tmp=0;
    char crc=0;

    #ifdef USE_STTERM
    unsigned char tmp_c;
    #endif

    while(1) {
        i++;
        #ifdef USE_STTERM
        if(stlinky_rx_ready()){
            scanf("%c", &tmp_c);
            INPUT[k] = tmp_c;
        #else
        if(usart_has_data()){
            INPUT[k] = usart_read();
        #endif
            gpio_toggle(GPIOD, GPIO15);
            k++;
            if(k==1 && INPUT[0]!=0x53){ // 'S'
                #ifdef DEBUG
                printf("ERROR AT START-BYTE %d\r\n",INPUT[0]);
                #endif
                k=0;
            }
            if(k==10 && INPUT[9]!=0x45){ // 'E'
                #ifdef DEBUG
                printf("ERROR AT END-BYTE %d\r\n",INPUT[8]);
                #endif
                k=0;
            }
            if(k==11){
                #ifdef DEBUG
                printf("Package: ");
                for(tmp=0;tmp<10;tmp++) printf("%d ",INPUT[tmp]);
                printf("\n");
                #endif
                for(tmp=0;tmp<8;tmp++) KEYS[tmp] = INPUT[tmp+1];
                crc = getCRC(KEYS,8);
                if(crc==INPUT[10]){
                    #ifdef DEBUG
                    printf("INPUT OK!\r\n");
                    #endif
                    sendkeys();
                }else{
                    #ifdef DEBUG
                    printf("ERROR AT CRC-BYTE %d vs %d\r\n",INPUT[10],crc);
                    #endif
                }
                k=0;
            }
        }
        if(i>1000000){
            gpio_toggle(GPIOD, GPIO13);
            i=0;
        }
    }

    return 0;
}
