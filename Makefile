CC = gcc
CFLAGS = -Wall -Werror -g
DYNLIB = -ldl
TARGET_FOLDER = ./target-build
DYNLIB_FOLDER = dynlib
COMMON_DEPENDENCIES = $(TARGET_FOLDER)/file_management.o $(TARGET_FOLDER)/util.o

all: client server

client: client.o $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/client.o $(TARGET_FOLDER)/io_client.o $(COMMON_DEPENDENCIES) -L$(DYNLIB_FOLDER) -lclient $(DYNLIB)

server: server.o $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/server.o $(TARGET_FOLDER)/file_management_server.o $(COMMON_DEPENDENCIES) -L$(DYNLIB_FOLDER) -lserver $(DYNLIB)

client.o: client.c client.h $(TARGET_FOLDER)/io_client.o $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@ $(DYNLIB)

server.o: server.c server.h $(COMMON_DEPENDENCIES) $(TARGET_FOLDER)/file_management_server.o
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@ $(DYNLIB)

$(TARGET_FOLDER)/file_management_server.o: ./file_management/file_management_server.c ./file_management/file_management_server.h actions.h bool_type.h constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/file_management.o: ./file_management/file_management.c ./file_management/file_management.h $(TARGET_FOLDER)/util.o constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/io_client.o: ./file_management/io/io_client.c ./file_management/io/io_client.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/util.o: util.c util.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@

clean:
	rm -f $(TARGET_FOLDER)/*.o $(TARGET_FOLDER)/SecTrans $(TARGET_FOLDER)/client $(TARGET_FOLDER)/server ./files_to_store/*