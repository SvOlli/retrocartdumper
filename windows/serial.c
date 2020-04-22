
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

   dcb.fOutxCtsFlow    = 0;
   dcb.fOutxDsrFlow    = 0;
   dcb.fDtrControl     = DTR_CONTROL_HANDSHAKE;
   dcb.fDsrSensitivity = 0;
   dcb.fRtsControl     = RTS_CONTROL_DISABLE;
   dcb.fOutX           = 0;
   dcb.fInX            = 0;

   dcb.fErrorChar      = 0;
   dcb.fBinary         = 1;
   dcb.fNull           = 0;
   dcb.fAbortOnError   = 0;
   dcb.wReserved       = 0;
   dcb.XonLim          = 2;
   dcb.XoffLim         = 4;
   dcb.XonChar         = 0x13;
   dcb.XoffChar        = 0x19;
   dcb.EvtChar         = 0;

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
