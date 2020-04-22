
#ifndef DUMP_H
#define DUMP_H DUMP_H

#include "../main.h"

void dump_2k( ser_fd_t serfd, int outfd );
void dump_3f( ser_fd_t serfd, int outfd );
void dump_4k( ser_fd_t serfd, int outfd );
void dump_e0( ser_fd_t serfd, int outfd );
void dump_e7( ser_fd_t serfd, int outfd );
void dump_f4( ser_fd_t serfd, int outfd );
void dump_f6( ser_fd_t serfd, int outfd );
void dump_f8( ser_fd_t serfd, int outfd );

#if CREATE_LIST
const dumper_by_name_t dumpers[] =
{
   { "2k", dump_2k },
   { "4k", dump_4k },
   { "f8", dump_f8 },
   { "f6", dump_f6 },
   { "f4", dump_f4 },
   { "3f", dump_3f },
   { "e0", dump_e0 },
   { "e7", dump_e7 },
   { 0, 0 }
};
#endif

#endif
