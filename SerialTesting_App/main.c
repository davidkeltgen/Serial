/* 
 * File:   main.c
 * Author: david
 *
 * Created on June 20, 2014, 8:40 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

#define PORT   "/dev/ttyS3"
#define DOWNBAUD B9600

/*
 * 
 */
int main(int argc, char** argv) {

    int fdown = init_port();
    
    //send data forever
    while(1)
    {
        sendData(fdown);
    }

    return (EXIT_SUCCESS);
}

int init_port() {
    struct termios oldtio_up, newtio_up;
    int fd = open(PORT, O_WRONLY | O_NOCTTY);
    if (fd < 0) {
        printf("Couldnt connect\n");
        exit(-1);
    }
    /*save current serial port settings*/
    tcgetattr(fd, &oldtio_up);

    /*clear struct for new port settings*/
    bzero(&newtio_up, sizeof (newtio_up));

    /*set flags for non-canonical serial connection*/
    newtio_up.c_cflag |= DOWNBAUD | CS8 | CSTOPB | HUPCL | CLOCAL;
    newtio_up.c_cflag &= ~(PARENB | PARODD);
    newtio_up.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | INLCR | IGNCR | ICRNL | IXON | IXOFF | IUCLC | IXANY | IMAXBEL);
    //newtio_up.c_iflag |= ISTRIP;
    newtio_up.c_oflag &= ~OPOST;
    newtio_up.c_lflag &= ~(ISIG | ICANON | XCASE | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE | IEXTEN);

    /*set non-canonical attributes*/
    newtio_up.c_cc[VTIME] = 1;
    newtio_up.c_cc[VMIN] = 255;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio_up);

    printf("sucessfully connected\n");
    return fd;
}

void sendData(int fd)
{
    int i = 0;
    char data[10] = "AAAAABBBBB";
    while(i < 10)
    {
        if(write(fd,&data[i],1) < 1) printf("Error writing data");
        i++;
    }
}


