
#include "dump.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static void dump_hotspots( ser_fd_t serfd, int outfd, int bufferstart, int buffersize, int hotspotstart, int hotspotsize )
{
   int bank;
   int bankdata;
   int i;

   unsigned char *buffer = malloc( buffersize * hotspotsize );

   if( (bufferstart < hotspotstart) && (bufferstart + buffersize >= hotspotstart) )
   {
      /* hotspots are inside buffer */
      for( bank = 0; bank < hotspotsize; ++bank )
      {
         bankdata = peek( serfd, hotspotstart + bank );
         for( i = 0; i < hotspotsize; ++i )
         {
            buffer[i*buffersize+hotspotstart-bufferstart+bank] = bankdata & 0xFF;
         }
         poke( serfd, 0xF000, 0xFF ); /* neat trick to clean up superchip ram during dump */
         dump( serfd, buffer + bank * buffersize, bufferstart, hotspotstart - bufferstart );
         dump( serfd, buffer + bank * buffersize + hotspotstart + hotspotsize - bufferstart,
               hotspotstart + hotspotsize, bufferstart + buffersize - hotspotstart - hotspotsize );
      }
   }
   else
   {
      /* hotspots are outside buffer */
      for( bank = 0; bank < hotspotsize; ++bank )
      {
         peek( serfd, hotspotstart + bank );
         dump( serfd, buffer + bank * buffersize, bufferstart, buffersize );
      }
   }

   write( outfd, buffer, buffersize * hotspotsize );
   free( buffer );
}


void dump_f8( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0xFFF8, 0x02 );
}


void dump_f6( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0xFFF6, 0x04 );
}


void dump_f4( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0xFFF4, 0x08 );
}


void dump_ef( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0xFFE0, 0x10 );
}


void dump_df( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0xFFC0, 0x20 );
}


void dump_bf( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0xFF80, 0x40 );
}


void dump_sb128( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0x0800, 0x20 );
}


void dump_sb256( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0x0800, 0x40 );
}


void dump_fa( ser_fd_t serfd, int outfd )
{
   dump_hotspots( serfd, outfd, 0xF000, 0x1000, 0x1FF8, 0x03 );
}
