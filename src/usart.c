#include "usart.h"

unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
uint32_t UART_RxHead;
uint32_t UART_RxTail;

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

	nvic_enable_irq(NVIC_USART2_IRQ);

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX | USART_MODE_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Enable USART2 Receive interrupt. */
	usart_enable_rx_interrupt(USART2);

	/* Finally enable the USART. */
	usart_enable(USART2);

    /* Disable stupid output buffers. */
	setbuf(stdout, NULL);
}

void usart2_isr(void)
{
	static uint8_t data;

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

		/* Retrieve the data from the peripheral. */
		data = usart_recv(USART2);

		/* Put Data into receive buffer */
		uint32_t tmphead;
		tmphead = (UART_RxHead + 1) & UART_RX_BUFFER_MASK;
	    UART_RxBuf[tmphead] = data;
	    UART_RxHead = tmphead;
	}
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

int usart_has_data(void){
	return (UART_RxHead!=UART_RxTail);
}

char usart_read(void){
	while(!usart_has_data());
	uint32_t tmptail;
	tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
	UART_RxTail = tmptail;
	return UART_RxBuf[tmptail];
}

int _read(int file, char *ptr, int len)
{
    len=1; /* lol wat? */

	int i;

	uint32_t tmptail;

	if (file == STDIN_FILENO) {
		for (i = 0; i < len; i++) {
			if(UART_RxHead!=UART_RxHead){
				tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
				ptr[i] = UART_RxBuf[tmptail];
				UART_RxTail = tmptail;
			}else{
				break;
			}
//			ptr[i] = usart_recv_blocking(USART2);
		}
		return i;
	}
	errno = EIO;
	return -1;
}
