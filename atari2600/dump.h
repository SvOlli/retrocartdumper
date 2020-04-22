
#ifndef DUMP_H
#define DUMP_H DUMP_H

#include "../main.h"

void dump_2k( ser_fd_t serfd, int outfd );
void dump_3f( ser_fd_t serfd, int outfd );
void dump_4k( ser_fd_t serfd, int outfd );
void dump_bf( ser_fd_t serfd, int outfd );
void dump_df( ser_fd_t serfd, int outfd );
void dump_e0( ser_fd_t serfd, int outfd );
void dump_e7( ser_fd_t serfd, int outfd );
void dump_ef( ser_fd_t serfd, int outfd );
void dump_f4( ser_fd_t serfd, int outfd );
void dump_f6( ser_fd_t serfd, int outfd );
void dump_f8( ser_fd_t serfd, int outfd );

#if CREATE_LIST
#define DUMPER(NAME) { #NAME, dump_ ## NAME }
const dumper_by_name_t dumpers[] =
{
   DUMPER(2k), DUMPER(4k),
   DUMPER(f8), DUMPER(f6), DUMPER(f4), DUMPER(ef), DUMPER(df), DUMPER(bf),
   DUMPER(3f),
   DUMPER(e0),
   DUMPER(e7),
   { 0, 0 }
};
#undef DUMPER
#endif

#endif
