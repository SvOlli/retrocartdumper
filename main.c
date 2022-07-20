
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* i/o */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "main.h"

#define CREATE_LIST 1
#ifdef ATARI2600
#include "atari2600/dump.h"
#endif


void exiterror( char *message, int retval )
{
   perror( message );
   exit( retval );
}

static int readhexbyte( ser_fd_t serfd )
{
   char tmp[3];
   memset( tmp, 0, sizeof(tmp) );

   if( ser_read( serfd, &tmp[0], 2 ) != 2 )
   {
      exiterror(  "hex read did not return two ascii bytes", 10 );
   }
   return strtol( tmp, 0, 16 );
}

static ssize_t readhex( ser_fd_t serfd, unsigned char *buffer, ssize_t size )
{
   ssize_t readsize = 0;
   char tmp[3];
   memset( tmp, 0, sizeof(tmp) );

   while( readsize < size )
   {
      printf( "\r%04X", (int)readsize );
      fflush( stdout );

      switch( ser_read( serfd, &tmp[0], 2 ) )
      {
         case 0: /* eof */
            return readsize;
         case 1: /* short read */
            exiterror( "short read", 2 );
         case 2: /* expected */
            *(buffer++) = strtol( tmp, 0, 16 );
            ++readsize;
            break;
         default: /* error */
            exiterror( "read error", 2 );
      }
   }
   printf( "\r\n" );
   return readsize;
}


void poke( ser_fd_t serfd, int address, int value )
{
   int retval;
   char sendbuffer[16];
   snprintf( sendbuffer, sizeof(sendbuffer)-1,
             "W%04X %02X\r\n", address & 0xFFFF, value & 0xFF );
   retval = ser_write( serfd, sendbuffer, strlen(sendbuffer) );
   if( retval < 0 )
   {
      exiterror( "poke failed writing to serial", 10 );
   }
}


int peek( ser_fd_t serfd, int address )
{
   int retval;
   char sendbuffer[16];
   snprintf( sendbuffer, sizeof(sendbuffer)-1,
             "R%04X\r\n", address & 0xFFFF );
   retval = ser_write( serfd, sendbuffer, strlen(sendbuffer) );
   if( retval < 0 )
   {
      exiterror( "peek failed writing to serial", 10 );
   }
   retval = readhexbyte( serfd );
   if( retval < 0 )
   {
      exiterror( "peek did not get value", 10 );
   }
   
   return retval;
}


void dump( ser_fd_t serfd, unsigned char *buffer, int start, int size )
{
   int retval;
   char sendbuffer[16];

   snprintf( sendbuffer, sizeof(sendbuffer)-1,
             "D%04X %04X\r\n", start  & 0xFFFF, size & 0xFFFF );
   retval = ser_write( serfd, sendbuffer, strlen(sendbuffer) );
   if( retval < 0 )
   {
      exiterror( "dump failed writing to serial", 10 );
   }
   if( readhex( serfd, buffer, size ) != size )
   {
      exiterror( "dump did not get values", 10 );
   }
}


int main( int argc, char *argv[] )
{
   ser_fd_t serfd;
   int outfd;

   dumper_t dumper = 0;
   const dumper_by_name_t *list = dumpers;

   if( argc != 4 )
   {
      fprintf( stderr, "usage: %s <device> <carttype> <filename>\n", argv[0] );
      fprintf( stderr, "known carttypes are: %s", list->name );
      while( (++list)->name && list->handler )
      {
         fprintf( stderr, ", %s", list->name );
      }
      fprintf( stderr, "\n" );
      return 1;
   }

   while( list->name && list->handler )
   {
      if( !strcasecmp( list->name, argv[2] ) )
      {
         dumper = list->handler;
         break;
      }
      ++list;
   }
   if( !dumper )
   {
      fprintf( stderr, "dumper '%s' unknown\n", argv[2] );
      return 2;
   }

   serfd = ser_open( argv[1], 38400 );
   outfd = open( argv[3], FILE_OPEN_FLAGS, 0666 );
   if( outfd < 0 )
   {
      perror("can't open output file");
      return 11;
   }

   dumper( serfd, outfd );
   peek( serfd, 0x1000 ); /* hack: turn off led on Teensy++ 2.0 */

   ser_close( serfd );
   close( outfd );

   return 0;
}
