cmake_minimum_required(VERSION 3.27)
project(SoftSec_Project_SecTrans C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl  -std=c++11")

include_directories(.)

add_executable(SoftSec_Project_SecTrans
        client.h
        server.h
        client.c
        server.c
        auth/auth.h
        auth/auth.c
        file_management/file_management.c
        file_management/file_management.h
        time_management/time_management.h
        time_management/time_management.c
        constants.h
        file_management/file_management_server.h
        file_management/file_management_server.c
        load_libraries/load_libraries_server.c
        load_libraries/load_libraries_client.c
        message_management/client_message_management/client_message_management.c
        message_management/client_message_management/client_message_management.h
        message_management/server_message_management/server_message_management.c
        message_management/server_message_management/server_message_management.h
        utils/array_utils.c
        utils/array_utils.h
        utils/array_utils.c
        utils/array_utils.c
        utils/array_utils.c
        message_management/common_message_management.c
        message_management/common_message_management.h
        rsa/rsa.c
        rsa/rsa_manager.c
        rsa/rsa_manager.h
        aes/aes_256_cbc.c
        aes/aes_manager.c
        aes/aes_256_cbc.h
        aes/aes_manager.h
)
