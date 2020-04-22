
#include "dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void dump_3f( ser_fd_t serfd, int outfd )
{
   int bank;
   int readsize;

   unsigned char *buffer = malloc( 0x80000 ); /* maximum size: 256 2k banks  */

   dump( serfd, buffer + 0x7F800, 0xF800, 0x800 );

   for( bank = 0; bank < 0x100; ++bank )
   {
      poke( serfd, 0x3f, bank );
      dump( serfd, buffer + 0x800 * bank, 0xF000, 0x800 );
      /* check, if it's the last bank */
      if( !memcmp( buffer + 0x800 * bank, buffer + 0x7F800, 0x800 ) )
      {
         break;
      }
   }

   readsize = 0x800 * (bank+1);

   printf( "Detected ROM size is %05X\n", readsize );

   write( outfd, buffer, readsize );
   free( buffer );
}
