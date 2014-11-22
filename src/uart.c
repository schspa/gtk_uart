#include "uart.h"
#include "gui_main.h"

#include "package.h"
//#include  <errno.h>
struct SerialPort SerPort;
char pipe_cache[PIPE_LEN];
char port_cache[PIPE_LEN];
pid_t uart_recv_pid;
static int pipe_to_gui;

struct package *pack;

int set_options(struct SerialPort *port)
{
	struct termios options;
	tcgetattr((*port).fd, &options);
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);

	options.c_iflag = IGNPAR | IGNBRK;
	options.c_cflag =  (CLOCAL | CREAD);   ///Enable the receiver and set local mode...
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */
   
	options.c_cflag &= ~PARENB; //No parity (8N1): 
	options.c_cflag |= CSTOPB;
   	options.c_cflag &= ~CSTOPB; 
	//options.c_cflag &= ~CSTOPB2;   //stopbit =1
   
	options.c_cflag &= ~CRTSCTS;  //disable hardware flow control
	options.c_cflag &=  ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|NOFLSH); 
	options.c_oflag &= ~OPOST;

	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

	tcflush((*port).fd, TCIOFLUSH);
	tcsetattr((*port).fd,TCSANOW,&options);
	fcntl((*port).fd, F_SETFL, 0);
	return 0;
}
int open_uart(struct SerialPort *port){
	(*port).fd = open((*port).dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if((*port).fd == -1){
		perror(strerror(errno));
		//printf("dev = %s\r\n",(*port).dev);
		return -1;
	}else{
		//fcntl((*port).fd, F_SETFL, 0);
		set_options(port);
		//printf("port fd:%d\n",(*port).fd);
	}
	return 0;	
}

void send_req(int fd){
	int i;
	int n;
	char buf[3]= {'F',' ',':'};
	for(i = 0; i<10; i++){
		buf[1] = '0'+i;
		n = write(fd, buf, 3);
		//write(1,buf, 3);
		usleep(20000);
		if (n < 0) {
		        fputs("write() of 4 bytes failed!\n", stderr);
		}
	} //for(i = 0; i<10; i++)
}
/*
 *  used to get out useful data to char * ptr;
 */
char *get_data(char *buf, int len){
	char *ptr;
	ptr = buf;
	if(*ptr != 0xffffffaa){
		while(*ptr != 0xffffffaa)
			ptr++;
	}
	while((*ptr) == 0xffffffaa)
		ptr++;
	return ptr;
}
static void *handlerreadport(void *arg){
	int n;
	char buf[20];
	while(1){
		n = read(*(int *)arg, port_cache, PIPE_LEN);
		
		if(n > 0){
			tcflush(*(int *)arg, TCIFLUSH);
			printf("received %d char\n", n);
			
			pack = (struct package *)get_data(port_cache, n);
			(*pack).channel -= '0';
			if(pack != NULL){
				//sprintf(buf,"F%c:%d",port_cache[2], *(int *)(&port_cache[4]));
				sprintf(buf, "F%d:%08x",(*pack).channel, (*pack).value1);
				printf(buf);
				write(pipe_to_gui,buf, strlen(buf));
				/*
				usleep(1000*50);
				sprintf(buf, "F%d:%08x",((*pack).channel<<1)+1, (*pack).value2);
				printf(buf);
				write(pipe_to_gui,buf, strlen(buf));
				*/
			}
		}
	}
	return NULL;
}
static void *handlerreadpipe(void *arg){
	int n;
	while(1){
		n = read(*(int *)arg, pipe_cache, PIPE_LEN);
		if(n > 0){
			//printf("pipe %d char:%s\n",n,pipe_cache);
			if(pipe_cache[0] == 'S'){
				exit(0);
			}
		}
	}
	return NULL;
}
static void send_t(struct SerialPort *ser,int pipe_r,int pipe_w){
	int res;
	pthread_t thread_rdpipe,thread_rdport;
	//fcntl((*ser).fd, F_SETFL, FNDELAY);	//如果未读取到数据也立即返回。。
	//fcntl((*ser).fd, F_SETFL, 0);
	res = pthread_create (&thread_rdpipe, NULL, handlerreadpipe, (void *)&pipe_r);
	if(res != 0){
		perror("Thread thread_rdpipe create failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_create (&thread_rdport, NULL, handlerreadport, (void *)&(*ser).fd);
	if(res != 0){
		perror("Thread thread_rdport create failed");
		exit(EXIT_FAILURE);
	}
	while(1){
		send_req((*ser).fd);
		printf("uart_fd:%d..\n",(*ser).fd);
		sleep(1);
	}
}


void uart_thread(struct SerialPort *ser,int pipe_r, int pipe_w){
	//int n;
	//int i = 0;
	//int flags;
	//flags = fcntl(pipe_r, F_GETFL, 0); //获取文件的flags值。
      	//fcntl(pipe_r, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；
	pipe_to_gui = pipe_w;
	send_t(ser,pipe_r,pipe_w);
}



