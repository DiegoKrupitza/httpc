CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
  
CFLAGS = -Wall -g -std=c99 -pedantic $(DEFS)
SERVEROBJECTS = server.o
.PHONY: all clean

all: clean httpc

httpc: $(SERVEROBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

server.o: server.c server.h

clean:
	rm -rf *.o httpc