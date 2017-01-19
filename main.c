#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <string.h>
#include "uart.h"
#include "stm_command.h"



int main(int argc, char *argv[])
{
	serial_port_init();

	
	for(;;)
	{
		uart_write(0x7F);
		sleep(2);
	}

	return 0;
}

