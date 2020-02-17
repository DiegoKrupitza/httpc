CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
  
CFLAGS = -Wall -Wgnu-zero-variadic-macro-arguments -Wno-unused-function -g -std=c99 -pedantic $(DEFS)
SERVEROBJECTS = server.o httpHeaderManager.o permissions.o
.PHONY: all clean

all: clean httpc

httpc: $(SERVEROBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

httpHeaderManager.o: httpHeaderManager.c httpHeaderManager.h httpStatusCodes.h
server.o: server.c server.h httpHeaderManager.o permissions.o
permissions.o: permissions.c permissions.h

clean:
	rm -rf *.o httpc