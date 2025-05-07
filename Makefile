CC=gcc
CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3 -pthread
HEADER=-I./

all: clean server client

server:
	$(CC) $(CFLAGS) $(HEADER) server.c handler.c -o server

client:
	$(CC) $(CFLAGS) $(HEADER) client.c -o client

clean:
	rm -f server client
