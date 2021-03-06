CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
  
CFLAGS = -Wall -Wno-unused-function -g -std=c99 -pedantic $(DEFS)
SERVEROBJECTS = server.o httpHeaderManager.o permissions.o mimeTypeManager.o messageHandler.o compressionHandler.o
.PHONY: all clean

all: clean httpc

httpc: $(SERVEROBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

server.o: server.c server.h httpHeaderManager.o permissions.o mimeTypeManager.o messageHandler.o
httpHeaderManager.o: httpHeaderManager.c httpHeaderManager.h httpStatusCodes.h
messageHandler.o: messageHandler.c messageHandler.h httpHeaderManager.c httpHeaderManager.h
permissions.o: permissions.c permissions.h httpStatusCodes.h httpHeaderManager.h
mimeTypeManager.o: mimeTypeManager.c mimeTypeManager.h permissions.h
compressionHandler.o: compressionHandler.c compressionHandler.h

clean:
	rm -rf *.o httpc