CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -L. -lclient -lserver

all: main

main: main.c libclient.so libserver.so
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)

libclient.so: client.c client.h
	$(CC) $(CFLAGS) -fPIC -shared -o libclient.so client.c

libserver.so: server.c server.h
	$(CC) $(CFLAGS) -fPIC -shared -o libserver.so server.c

clean:
	rm -f main libclient.so libserver.so
