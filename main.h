
#ifndef MAIN_H
#define MAIN_H MAIN_H

#include <unistd.h>

#ifdef WIN32
#include <windows.h>
typedef HANDLE ser_fd_t;
#define FILE_OPEN_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_BINARY
#else
typedef int ser_fd_t;
#define FILE_OPEN_FLAGS O_WRONLY | O_CREAT | O_TRUNC
#endif

typedef void (*dumper_t)(ser_fd_t,int);

typedef struct {
   const char  *name;
   dumper_t    handler;
} dumper_by_name_t;

ser_fd_t ser_open(  const char *devname, int baud );
int      ser_ok(    ser_fd_t fd );
ssize_t  ser_read(  ser_fd_t fd, void *buf, size_t count );
ssize_t  ser_write( ser_fd_t fd, void *buf, size_t count );
void     ser_close( ser_fd_t fd );

void exiterror( char *message, int retval );
void poke( ser_fd_t serfd, int address, int value );
int peek( ser_fd_t serfd, int address );
void dump( ser_fd_t serfd, unsigned char *buffer, int start, int size );

#endif
