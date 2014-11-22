#ifndef __UART_H__
#define __UART_H__
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> /* File control definitions */
#include <errno.h>
#include <termios.h> /* POSIX terminal control definitions */
	
#define PIPE_LEN 64

struct SerialPort{
	char *dev;
	int speed;
	int nbit;
	int sbit;
	//
	int fd;
};
extern struct SerialPort SerPort;
int set_options(struct SerialPort *port);
int open_uart(struct SerialPort *port);
void uart_thread(struct SerialPort *ser,int pipe_r, int pipe_w);
#endif

