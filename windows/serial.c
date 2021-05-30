
#include <windows.h>
#include <string.h>

#include "../main.h"


ser_fd_t ser_open( const char *devname, int baud )
{
   ser_fd_t fd;
   DCB dcb;
   COMMTIMEOUTS cto = { 0, 0, 0, 0, 0 };

   memset( &dcb, 0, sizeof( dcb ) );

   dcb.DCBlength       = sizeof(dcb);                   

   dcb.BaudRate        = baud;
   dcb.Parity          = NOPARITY;
   dcb.StopBits        = ONESTOPBIT;
   dcb.ByteSize        = 8;

   fd = CreateFile( devname, GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING, 0, NULL );

   if( SetCommMask( fd, 0) )
   {
      if( SetCommTimeouts( fd, &cto) )
      {
         if( SetCommState( fd, &dcb) )
         {
            return fd;
         }
      }
   }
   if( fd != INVALID_HANDLE_VALUE )
   {
      CloseHandle( fd );
      fd = INVALID_HANDLE_VALUE;
   }
   return fd;
}


int ser_ok( ser_fd_t fd )
{
   return fd != INVALID_HANDLE_VALUE;
}


ssize_t ser_read( ser_fd_t fd, void *buffer, size_t size )
{
   DWORD retval;
   ReadFile( fd, buffer, size, &retval, NULL );
   return (ssize_t)retval;
}


ssize_t ser_write( ser_fd_t fd, void *buffer, size_t size )
{
   DWORD retval;
   WriteFile( fd, buffer, size, &retval, NULL );
   return (ssize_t)retval;
}


void ser_close( ser_fd_t fd )
{
   (void)CloseHandle( fd );
}
