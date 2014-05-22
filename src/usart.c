#include "usart.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void usart_gpio_setup(void)
{
	/* Setup GPIO pins for USART2 transmit/receive. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO2);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);

	/* Setup USART2 TX/RX pins as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO3);
}

void usart_clock_setup(void)
{
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);
}

void usart_setup(void)
{
	usart_clock_setup();
	usart_gpio_setup();

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX | USART_MODE_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);

        /* Disable Buffers */
	setbuf(stdout, NULL);
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			usart_send_blocking(USART2, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

int _read(int file, char *ptr, int len)
{
        len=1; /* lol wat? */

	int i;

	if (file == STDIN_FILENO) {
		for (i = 0; i < len; i++) {
			ptr[i] = usart_recv_blocking(USART2);
		}
		return i;
	}
	errno = EIO;
	return -1;
}
