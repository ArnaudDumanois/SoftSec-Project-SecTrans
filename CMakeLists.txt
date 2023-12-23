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
        auth.h
        auth.c
)
