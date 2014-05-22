#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

int _write(int file, char *ptr, int len);
int _read(int file, char *ptr, int len);

void usart_gpio_setup(void);
void usart_clock_setup(void);
void usart_setup(void);
int  usart_has_data(void);
char usart_read(void);

#define UART_RX_BUFFER_SIZE 4096
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )

extern unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
extern uint32_t UART_RxHead;
extern uint32_t UART_RxTail;