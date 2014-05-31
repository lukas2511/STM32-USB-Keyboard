#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/cm3/scb.h>

#include "crc.h"
#include "usb.h"
#include "systick.h"
#include "timers.h"

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

void tim2_isr(void){
    if(timer_get_flag(TIM2, TIM_SR_UIF)){
        if(last_usb_request_time < (system_millis-10)){
            gpio_set(GPIOD, GPIO13);
            if(usb_ready == 3 && system_millis>5000) msleep(10);
        }else{
            if(usb_ready >= 1) usb_ready = 3;
            gpio_clear(GPIOD, GPIO13);
        }
        timer_clear_flag(TIM2, TIM_SR_UIF);
    }
}

uint32_t k=0;
static void handle_data(void){
    #ifdef USE_STTERM
    unsigned char tmp_c;
    #endif

    uint32_t tmp=0;
    char crc=0;

    #ifdef USE_STTERM
    if(stlinky_rx_ready()){
        scanf("%c", &tmp_c);
        INPUT[k] = tmp_c;
        gpio_toggle(GPIOD, GPIO15);
    #else
    if(usart_has_data()){
        INPUT[k] = usart_read();
    #endif
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
                if(usb_ready == 3) usb_send_packet(KEYS, 8);
            }else{
                #ifdef DEBUG
                printf("ERROR AT CRC-BYTE %d vs %d\r\n",INPUT[10],crc);
                #endif
            }
            k=0;
        }
    }
}
void tim3_isr(void){
    if(timer_get_flag(TIM3, TIM_SR_UIF)){
        handle_data();
        timer_clear_flag(TIM3, TIM_SR_UIF);
    }
}

void tim4_isr(void){
    if(timer_get_flag(TIM4, TIM_SR_UIF)){
        if(usb_ready==3){
            gpio_toggle(GPIOD,GPIO14);
            KEYS[0]=0;
            KEYS[1]=0;
            KEYS[3]=0;
            KEYS[4]=0;
            KEYS[5]=0;
            KEYS[6]=0;
            KEYS[7]=0;

            KEYS[2]=0x04;
            usb_send_packet(KEYS, 8);
            KEYS[2]=0x00;
            usb_send_packet(KEYS, 8);
            KEYS[2]=0x05;
            usb_send_packet(KEYS, 8);
            KEYS[2]=0x00;
            usb_send_packet(KEYS, 8);
        }
        timer_clear_flag(TIM4, TIM_SR_UIF);
    }
}

void tim5_isr(void){
    if(timer_get_flag(TIM5, TIM_SR_UIF)){
        gpio_toggle(GPIOD, GPIO15);
        timer_clear_flag(TIM5, TIM_SR_UIF);
    }
}

int main(void) {
    iwdg_reset();
    iwdg_set_period_ms(5);
    iwdg_start();

    rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOD);
    systick_setup();

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
    gpio_clear(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    iwdg_set_period_ms(500);
    iwdg_reset();
    msleep(400);
    iwdg_set_period_ms(5);
    iwdg_reset();

    gpio_set(GPIOD, GPIO15);

//    msleep(100000); /* SLEEEEEEEEEEEEEEEEEEEEEEP */

    #ifdef USE_STTERM
    stlinky_init();
    #else
    usart_setup();
    #endif

    timer2_setup(100);

    usb_setup();

    //timer4_setup(1);
    timer3_setup(100);
    timer5_setup(2);

//    gpio_set(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    while(1) {
        iwdg_reset();
    }

    return 0;
}
