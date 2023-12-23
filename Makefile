CC = gcc
CFLAGS = -Wall -Werror -g
DYNLIB = -ldl

all: SecTrans

SecTrans: main.o client.o server.o
	$(CC) $(CFLAGS) -o SecTrans main.o client.o server.o -L. -lclient -lserver $(DYNLIB)

main.o: main.c client.o server.o
	$(CC) $(CFLAGS) -c main.c -o main.o $(DYNLIB)

client.o: client.c client.h
	$(CC) $(CFLAGS) -c client.c -o client.o $(DYNLIB)

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c -o server.o $(DYNLIB)

clean:
	rm -f *.o SecTrans