#ifndef UART_H
#define UART_H




/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B115200   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO4" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */


struct rev_callback_t
{
	int (*func)(unsigned long, char*);
	unsigned long private_data;
};

int uart_init(void);
int set_uart_on_off(int on_off);

int uart_rev_callback_reg(void *func, unsigned long data);
int uart_write(char data);








#endif
