
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "../main.h"


ser_fd_t ser_open( const char *devname, int baud )
{
   ser_fd_t fd;
   struct termios tty;

   fd = open( devname, O_RDWR );

   if( tcgetattr( fd, &tty ) )
   {
      exiterror( "setserial can't get attributes", 10 );
   }

   cfsetospeed( &tty, (speed_t)baud );
   cfsetispeed( &tty, (speed_t)baud );

   tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
   tty.c_cflag &= ~CSIZE;           /* clear character size */
   tty.c_cflag |= CS8;              /* 8-bit */
   tty.c_cflag &= ~PARENB;          /* 0 parity bits */
   tty.c_cflag &= ~CSTOPB;          /* 1 stop bit */
   tty.c_cflag &= ~CRTSCTS;         /* no hardware flowcontrol */

   /* setup for non-canonical mode */
   tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
   tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   tty.c_oflag &= ~OPOST;

   /* fetch bytes as they become available */
   tty.c_cc[VMIN] = 1;
   tty.c_cc[VTIME] = 1;

   if( tcsetattr(fd, TCSANOW, &tty) )
   {
      exiterror( "setserial can't set attributes", 10 );
   }
   return fd;
}


int ser_ok( ser_fd_t fd )
{
   return fd >= 0;
}


ssize_t ser_read( ser_fd_t fd, void *buffer, size_t size )
{
   return read( fd, buffer, size );
}


ssize_t ser_write( ser_fd_t fd, void *buffer, size_t size )
{
   return write( fd, buffer, size );
}


void ser_close( ser_fd_t fd )
{
   (void)close( fd );
}
