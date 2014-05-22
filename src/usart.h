#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int _write(int file, char *ptr, int len);
int _read(int file, char *ptr, int len);

void usart_gpio_setup(void);
void usart_clock_setup(void);
void usart_setup(void);
