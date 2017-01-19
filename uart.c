#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "gdef.h"
#include "type.h"
#include "uart.h"

int fd, c, res;
struct termios oldtio, newtio;
char buf[255];

pthread_t tid;
void *tret;
int uart_on = 0;

struct rev_callback_t rev_callback =
{
	.func = NULL,
	.private_data = 0,
};

int set_uart_on_off(int on_off)
{
	uart_on = on_off;

	return 0;
}

int uart_rev_callback_reg(void *func, unsigned long data)
{
	rev_callback.func = func;
	rev_callback. private_data = data;

	return 0;
}

int uart_read(char *data)
{
	char serial_in[255];


	while(uart_on)
	{
		int res = read(fd, serial_in, 255);

        	serial_in[res] = 0;             /* set end of string, so we can printf */
		if(res > 0)
			printf("[%2x] \n", serial_in[0]);
        	//proc_log_dmesg("[boardtest_proc] : uart1 get %s", serial_in, res);
#if 0
		if(rev_callback.func != NULL)
			rev_callback.func(rev_callback.private_data, serial_in);
#else
//		if(rev_callback.func != NULL)
//			rev_callback.func(res, serial_in);

#endif
	}
	return 0;
}

#if 1

int uart_write(char data)
{
//	proc_log_dmesg("[boardtest]: uart write -> %s \n", serial_out);
	return (int )write(fd, &data, 1);

}
#else
int uart_write(char *data, int len)
{
	char serial_out[255];

	memcpy(serial_out, data, len);
	serial_out[len] = '\0';
//	proc_log_dmesg("[boardtest]: uart write -> %s \n", serial_out);
	return (int )write(fd, serial_out, len+1);

}
#endif
int uart_ter(void)
{

	/* wait for thread to end */
	pthread_join(tid, &tret);
    tcsetattr(fd, TCSANOW, &oldtio);

    return 0;
}
int uart_init(void)
{
	int ret;
    // Load the pin configuration
    //int ret = system("echo uart1 > /sys/devices/bone_capemgr.9/slots");
    /* Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C. */
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
#if 0
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
#else
	newtio.c_cflag = BAUDRATE;
	newtio.c_cflag |= PARENB;
	newtio.c_cflag &= ~PARODD;
	newtio.c_cflag &= ~CSTOPB;
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS7;
#endif

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;


    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
#if 0
    newtio.c_lflag = ICANON;
#else
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
#endif
    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    // NMEA command to ouput all sentences
    // Note that this code & format values in manual are hexadecimal
#if 0
    write(fd, "$PTNLSNM,273F,01*27\r\n", 21);
#endif
    /* terminal settings done, now handle input*/
#if 0
    while (TRUE) {     /* loop continuously */
        /*  read blocks program execution until a line terminating character is
            input, even if more than 255 chars are input. If the number
            of characters read is smaller than the number of chars available,
            subsequent reads will return the remaining chars. res will be set
            to the actual number of characters actually read */
        res = read(fd, buf, 255);
        buf[res] = 0;             /* set end of string, so we can printf */
        printf("%s", buf, res);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
#endif
	set_uart_on_off(ON);
	ret = pthread_create(&tid, NULL, (void *)uart_read, NULL);

	if(ret < 0)
        printf(" Create thread erro \n");

	return 0;
}


