CC = gcc
CFLAGS = -Wall -Werror -g
DYNLIB = -ldl
TARGET_FOLDER = ./target-build
DYNLIB_FOLDER = dynlib

all: client server

client: client.o
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/client.o -L$(DYNLIB_FOLDER) -lclient $(DYNLIB)
server: server.o
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/server.o -L$(DYNLIB_FOLDER) -lserver $(DYNLIB)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@ $(DYNLIB)

clean:
	rm -f $(TARGET_FOLDER)/*.o $(TARGET_FOLDER)/SecTrans $(TARGET_FOLDER)/client $(TARGET_FOLDER)/server