
MXE_PATH = /opt/mxe
MXE_TARGET = i686-w64-mingw32.static
SUFFIX ?= 
SERIAL ?= posix/serial.c
CFLAGS += -Wall

WIN_CC = $(MXE_PATH)/usr/bin/$(MXE_TARGET)-gcc

all: dump2600$(SUFFIX)

native:

win32:
	make CC=$(WIN_CC) SUFFIX=.exe SERIAL=windows/serial.c all

dump2600$(SUFFIX): main.c $(SERIAL) $(wildcard atari2600/dump_*.c)
	$(CC) $(CFLAGS) -DATARI2600=1 -o $@ $^ $(LDFLAGS)

