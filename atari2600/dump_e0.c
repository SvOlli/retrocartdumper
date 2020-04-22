
#include "dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void dump_e0( ser_fd_t serfd, int outfd )
{
   int bank;

   unsigned char *buffer = malloc( 0x2000 );

   dump( serfd, buffer + 0x1C00, 0xFC00, 0x400 );

   for( bank = 0; bank < 7; ++bank )
   {
      peek( serfd, 0xffe0 + bank );
      dump( serfd, buffer + 0x400 * bank, 0xF000, 0x400 );
   }

   write( outfd, buffer, 0x2000 );
   free( buffer );
}
