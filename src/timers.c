#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3/cm3/scb.h>
#include "timers.h"

void timer2_setup(uint32_t hz){
    uint32_t prescaler = 84000000 / hz;
    uint32_t period = 1;
    while(prescaler>8400){
        prescaler=prescaler/10;
        period=period*10;
    }

    rcc_periph_clock_enable(RCC_TIM2);
    nvic_enable_irq(NVIC_TIM2_IRQ);
    timer_reset(TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_counter(TIM2,0);
    timer_set_prescaler(TIM2,prescaler-1);
    timer_set_period(TIM2,period-1);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}

void timer3_setup(uint32_t hz){
    uint32_t prescaler = 84000000 / hz;
    uint32_t period = 1;
    while(prescaler>8400){
        prescaler=prescaler/10;
        period=period*10;
    }

    rcc_periph_clock_enable(RCC_TIM3);
    nvic_enable_irq(NVIC_TIM3_IRQ);
    timer_reset(TIM3);
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_counter(TIM3,0);
    timer_set_prescaler(TIM3,prescaler-1);
    timer_set_period(TIM3,period-1);
    timer_enable_irq(TIM3, TIM_DIER_UIE);
    timer_enable_counter(TIM3);
}

void timer4_setup(uint32_t hz){
    uint32_t prescaler = 84000000 / hz;
    uint32_t period = 1;
    while(prescaler>8400){
        prescaler=prescaler/10;
        period=period*10;
    }

    rcc_periph_clock_enable(RCC_TIM4);
    nvic_enable_irq(NVIC_TIM4_IRQ);
    timer_reset(TIM4);
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_counter(TIM4,0);
    timer_set_prescaler(TIM4,prescaler-1);
    timer_set_period(TIM4,period-1);
    timer_enable_irq(TIM4, TIM_DIER_UIE);
    timer_enable_counter(TIM4);
}

void timer5_setup(uint32_t hz){
    uint32_t prescaler = 84000000 / hz;
    uint32_t period = 1;
    while(prescaler>8400){
        prescaler=prescaler/10;
        period=period*10;
    }

    rcc_periph_clock_enable(RCC_TIM5);
    nvic_enable_irq(NVIC_TIM5_IRQ);
    timer_reset(TIM5);
    timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_counter(TIM5,0);
    timer_set_prescaler(TIM5,prescaler-1);
    timer_set_period(TIM5,period-1);
    timer_enable_irq(TIM5, TIM_DIER_UIE);
    timer_enable_counter(TIM5);
}
