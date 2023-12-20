CC = gcc
CFLAGS = -Wall -Werror -g

all: SecTrans

SecTrans: main.o libclient.so libserver.so
	$(CC) main.o -o SecTrans -L. -lclient -lserver -Wl,-rpath=.

main.o: main.c client.h server.h
	$(CC) $(CFLAGS) -c main.c -o main.o

libclient.so: client.o
	$(CC) -shared -o libclient.so client.o

libserver.so: server.o
	$(CC) -shared -o libserver.so server.o

client.o: client.c client.h
	$(CC) $(CFLAGS) -fPIC -c client.c -o client.o

server.o: server.c server.h
	$(CC) $(CFLAGS) -fPIC -c server.c -o server.o

clean:
	rm -f *.o SecTrans