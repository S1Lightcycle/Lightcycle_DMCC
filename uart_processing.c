
#include "uart_processing.h"

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyO1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;
static int fd;
//static int c;
static int res = 0;
struct termios oldtio,newtio;
static char buf[2];
lc_protocol_t package = {};

void initUART() {
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	//if (fd <0) {perror(MODEMDEVICE); exit(-1); }

	tcgetattr(fd,&oldtio); /* save current port settings */

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 2;   /* blocking read until 2 chars received */

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);
}

lc_protocol_t getCommand()
{
	/*while (res <= 1) {
		res = read(fd,buf,2);   // returns after 2 chars have been input
		buf[res]=0;               // so we can printf...
	}*/
	read(fd, buf, 255);
	printf("received: HI: %x, LO: %x\n", buf[HI], buf[LO]);

	package = getProtocolStruct(buf[HI], buf[LO]);
	printf("package: address = %d, command = %d, parameter = %d\n", package.address, package.command, package.parameter);
	res = 0;
	return package;
}

void closeUART() {
	tcsetattr(fd,TCSANOW,&oldtio);
}
