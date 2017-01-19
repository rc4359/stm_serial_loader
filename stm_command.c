#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <string.h>
#include<pthread.h>
#include "gdef.h"
#include "type.h"
#include "uart.h"


pthread_t serial_id;
int serial_actived = 0;



void retrive_serial_data(unsigned long len, char *ch)
{
	int i = 0;
	unsigned int buf_len = len + 1;

	char *uart_buf = malloc(sizeof(char) * (buf_len));

	if(uart_buf == NULL)
		return;

	memset(uart_buf, 0 , buf_len);
	memcpy(uart_buf, ch, len);

	for(i = 0; i < buf_len; i++)
	{
		printf(" Uart Rx[%2x] \n", uart_buf[i]);
	}

	free(uart_buf);
}

#if 0
void* polling_serial_port_task(void* arg)
{
	char dummy;
	while(serial_actived)
	{
		uart_read(&dummy);
		usleep(1000);
	}
	return NULL;
}
#endif

void serial_port_init(void)
{

	uart_init();
	uart_rev_callback_reg(retrive_serial_data, 0);
	set_uart_on_off(ON);
#if 0
	serial_actived = 1;
	int err = pthread_create(&serial_id, NULL, &polling_serial_port_task, NULL);

	if(err != 0)
	 printf(" Create pthread has error !! (%d) \n", err);
#endif
}





