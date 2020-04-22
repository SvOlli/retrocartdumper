
#include "dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static int checksize( unsigned char *buffer, int size )
{
   if( memcmp( buffer, buffer + (size/2), size/2 ) )
   {
      return size;
   }
   return checksize( buffer, size/2 );
}


static void dump_simple( ser_fd_t serfd, int outfd, int start, int size )
{
   int readsize;

   unsigned char *buffer = malloc( size );

   dump( serfd, buffer, start, size );

   readsize = checksize( buffer, size );
   if( readsize < size )
   {
      printf( "Detected ROM to be smaller ($%04X) than expected ($%04X)\n", readsize, size );
      size = readsize;
   }

   write( outfd, buffer, size );
   free( buffer );
}


void dump_4k( ser_fd_t serfd, int outfd )
{
   dump_simple( serfd, outfd, 0xF000, 0x1000 );
}


void dump_2k( ser_fd_t serfd, int outfd )
{
   dump_simple( serfd, outfd, 0xF800, 0x0800 );
}
