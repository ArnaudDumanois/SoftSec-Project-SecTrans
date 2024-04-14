CC = gcc
CFLAGS = -Wall -Werror -g
DYNLIB = -ldl
TARGET_FOLDER = ./target-build
DYNLIB_FOLDER = dynlib
CRYPTOGRAPHY_COMPILATION_OPTION = -L/usr/lib -lcrypto -lssl

COMMON_DEPENDENCIES_UTILS = $(TARGET_FOLDER)/util.o $(TARGET_FOLDER)/array_utils.o
COMMON_DEPENDENCIES_LOAD_LIBRARIES = $(TARGET_FOLDER)/load_libraries_client.o $(TARGET_FOLDER)/load_libraries_server.o
COMMON_DEPENDENCIES_MESSAGE_MANAGEMENT = $(TARGET_FOLDER)/server_message_management.o $(TARGET_FOLDER)/client_message_management.o $(TARGET_FOLDER)/common_message_management.o $(TARGET_FOLDER)/client_cryptomessage_management.o $(TARGET_FOLDER)/server_cryptomessage_management.o
COMMON_DEPENDENCIES_FILE_MANAGEMENT = $(TARGET_FOLDER)/file_management.o $(TARGET_FOLDER)/file_management_server.o
COMMON_DEPENDENCIES_CRYPTOGRAPHY = $(TARGET_FOLDER)/rsa.o $(TARGET_FOLDER)/rsa_manager.o $(TARGET_FOLDER)/aes_256_cbc.o $(TARGET_FOLDER)/aes_manager.o
COMMON_DEPENDENCIES_BASE64 = $(TARGET_FOLDER)/base64encode.o $(TARGET_FOLDER)/base64decode.o

COMMON_DEPENDENCIES = $(COMMON_DEPENDENCIES_UTILS) $(COMMON_DEPENDENCIES_BASE64) $(COMMON_DEPENDENCIES_CRYPTOGRAPHY) $(COMMON_DEPENDENCIES_LOAD_LIBRARIES) $(COMMON_DEPENDENCIES_MESSAGE_MANAGEMENT) $(COMMON_DEPENDENCIES_FILE_MANAGEMENT)

CLIENT_DEPENDENCIES = $(TARGET_FOLDER)/messages.o $(TARGET_FOLDER)/logger_sha.o
SERVER_DEPENDENCIES = $(TARGET_FOLDER)/logger_sha.o
EXTERNAL_LIBS = /usr/lib

all: client server

client: client.o $(CLIENT_DEPENDENCIES) $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/client.o $(CLIENT_DEPENDENCIES) $(COMMON_DEPENDENCIES) -L$(DYNLIB_FOLDER) -lclient $(CRYPTOGRAPHY_COMPILATION_OPTION) $(DYNLIB)

server: server.o $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(TARGET_FOLDER)/$@ $(TARGET_FOLDER)/server.o $(SERVER_DEPENDENCIES) $(COMMON_DEPENDENCIES) -L$(DYNLIB_FOLDER) -lserver $(CRYPTOGRAPHY_COMPILATION_OPTION) $(DYNLIB)

##PARTIE CLIENT

client.o: client.c client.h $(CLIENT_DEPENDENCIES) $(COMMON_DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@ $(DYNLIB)

$(TARGET_FOLDER)/messages.o: ./libs/client_libs/messages.c ./libs/client_libs/messages.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/logger_sha.o: ./libs/logger/logger_sha.c ./libs/logger/logger_sha.h
	 $(CC) $(CFLAGS) -c $< -o $@ -L$(EXTERNAL_LIBS) -lcrypto

##PARTIE SERVEUR
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

$(TARGET_FOLDER)/client_cryptomessage_management.o: message_management/client_message_management/client_cryptomessage_management/client_cryptomessage_management.c message_management/client_message_management/client_cryptomessage_management/client_cryptomessage_management.h $(TARGET_FOLDER)/common_message_management.o $(TARGET_FOLDER)/client_message_management.o $(TARGET_FOLDER)/server_message_management.o client.h server.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/server_message_management.o: message_management/server_message_management/server_message_management.c message_management/server_message_management/server_message_management.h $(TARGET_FOLDER)/common_message_management.o $(TARGET_FOLDER)/logger_sha.o client.h server.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/server_cryptomessage_management.o: message_management/server_message_management/server_cryptomessage_management/server_cryptomessage_management.c message_management/server_message_management/server_cryptomessage_management/server_cryptomessage_management.h $(TARGET_FOLDER)/common_message_management.o $(TARGET_FOLDER)/client_message_management.o $(TARGET_FOLDER)/server_message_management.o client.h server.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/common_message_management.o: message_management/common_message_management.c message_management/common_message_management.h constants.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/rsa_manager.o: rsa/rsa_manager.c $(TARGET_FOLDER)/rsa.o rsa/rsa_manager.h
	$(CC) $(CFLAGS) -c $< -o $@ -L/usr/lib -lcrypto -lssl

$(TARGET_FOLDER)/rsa.o: rsa/rsa.c rsa/rsa.h
	$(CC) $(CFLAGS) -c $< -o $@ -L/usr/lib -lcrypto -lssl

$(TARGET_FOLDER)/aes_manager.o: aes/aes_manager.c $(TARGET_FOLDER)/aes_256_cbc.o
	$(CC) $(CFLAGS) -c $< -o $@ -L/usr/lib -lcrypto -lssl

$(TARGET_FOLDER)/aes_256_cbc.o: aes/aes_256_cbc.c aes/aes_manager.c
	$(CC) $(CFLAGS) -c $< -o $@ -L/usr/lib -lcrypto -lssl

$(TARGET_FOLDER)/base64encode.o: base64/base64encode.c base64/base64encode.h base64/base64constant.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_FOLDER)/base64decode.o: base64/base64decode.c base64/base64decode.h base64/base64constant.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $(TARGET_FOLDER)/$@

clean:
	rm -f $(TARGET_FOLDER)/* ./files_to_store/*