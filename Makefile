CC=gcc
CFLAGS=-Wall -pedantic-errors -std=gnu89 -O3 -pthread
HEADER=-I./

all: clean client server

server:
	$(CC) $(CFLAGS) $(HEADER) main.c server.c handler.c threadpool.c -o server

client:
	$(CC) $(CFLAGS) $(HEADER) client.c -o client

clean:
	rm -f server client
