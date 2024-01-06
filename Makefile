CC = gcc
CFLAGS = -Wall -Werror -g
DYNLIB = -ldl
TARGET_FOLDER = ./target-build
DYNLIB_FOLDER = dynlib
CRYPTOGRAPHY_COMPILATION_OPTION = -L/usr/lib -lcrypto -lssl

COMMON_DEPENDENCIES_UTILS = $(TARGET_FOLDER)/util.o $(TARGET_FOLDER)/array_utils.o
COMMON_DEPENDENCIES_LOAD_LIBRARIES = $(TARGET_FOLDER)/load_libraries_client.o $(TARGET_FOLDER)/load_libraries_server.o
COMMON_DEPENDENCIES_MESSAGE_MANAGEMENT = $(TARGET_FOLDER)/server_message_management.o $(TARGET_FOLDER)/client_message_management.o $(TARGET_FOLDER)/common_message_management.o
COMMON_DEPENDENCIES_FILE_MANAGEMENT = $(TARGET_FOLDER)/file_management.o $(TARGET_FOLDER)/file_management_server.o
COMMON_DEPENDENCIES_CRYPTOGRAPHY = $(TARGET_FOLDER)/rsa.o $(TARGET_FOLDER)/rsa_manager.o

COMMON_DEPENDENCIES = $(COMMON_DEPENDENCIES_UTILS) $(COMMON_DEPENDENCIES_CRYPTOGRAPHY) $(COMMON_DEPENDENCIES_LOAD_LIBRARIES) $(COMMON_DEPENDENCIES_MESSAGE_MANAGEMENT) $(COMMON_DEPENDENCIES_FILE_MANAGEMENT)

all: client server

client: client.o $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/client.o $(COMMON_DEPENDENCIES) -L$(DYNLIB_FOLDER) -lclient $(CRYPTOGRAPHY_COMPILATION_OPTION) $(DYNLIB)

server: server.o $(COMMON_DEPENDENCIES) $(TARGET_FOLDER)/rsa.o
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/server.o $(COMMON_DEPENDENCIES) -L$(DYNLIB_FOLDER) -lserver $(CRYPTOGRAPHY_COMPILATION_OPTION) $(DYNLIB)

client.o: client.c client.h $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@ $(DYNLIB)

server.o: server.c server.h $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@ $(DYNLIB)

$(TARGET_FOLDER)/file_management_server.o: ./file_management/file_management_server.c ./file_management/file_management_server.h actions.h utils/bool_type.h constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/file_management.o: ./file_management/file_management.c ./file_management/file_management.h $(TARGET_FOLDER)/util.o constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/util.o: utils/util.c utils/util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/array_utils.o: utils/array_utils.c utils/array_utils.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/load_libraries_server.o: ./load_libraries/load_libraries_server.c server.h
	$(CC) $(CFLAGS) -c $< -o $@ $(DYNLIB)

$(TARGET_FOLDER)/load_libraries_client.o: ./load_libraries/load_libraries_client.c client.h
	$(CC) $(CFLAGS) -c $< -o $@ $(DYNLIB)

$(TARGET_FOLDER)/client_message_management.o: message_management/client_message_management/client_message_management.c message_management/client_message_management/client_message_management.h $(TARGET_FOLDER)/common_message_management.o client.h server.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/server_message_management.o: message_management/server_message_management/server_message_management.c message_management/server_message_management/server_message_management.h $(TARGET_FOLDER)/common_message_management.o client.h server.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/common_message_management.o: message_management/common_message_management.c message_management/common_message_management.h constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/rsa_manager.o: rsa/rsa_manager.c
	$(CC) $(CFLAGS) -c $< -o $@ -L/usr/lib -lcrypto -lssl

$(TARGET_FOLDER)/rsa.o: rsa/rsa.c
	$(CC) $(CFLAGS) -c $< -o $@ -L/usr/lib -lcrypto -lssl

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@

clean:
	rm -f $(TARGET_FOLDER)/* ./files_to_store/*