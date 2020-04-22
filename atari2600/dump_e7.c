
#include "dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void dump_e7( ser_fd_t serfd, int outfd )
{
   int bank;

   unsigned char *buffer = malloc( 0x4000 );

   poke( serfd, 0xF800, 0xFF );
   dump( serfd, buffer + 0x3800, 0xF800, 0x800 );
   for( bank = 0; bank < 7; ++bank )
   {
      peek( serfd, 0xffe0 + bank );
      dump( serfd, buffer + 0x800 * bank, 0xF000, 0x800 );
   }

   write( outfd, buffer, 0x4000 );
   free( buffer );
}
