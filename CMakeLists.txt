cmake_minimum_required(VERSION 3.2)
project(KeyManagement)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_SOURCE_DIR src/)
set(PROJECT_BINARY_DIR bin/)

# 默认编译选项
set(SIMULATION ON)
set(SJK1238 OFF)
if (HARDWARE MATCHES SIMULATION)
    set(HARDWARE SIMULATION)
    set(SIMULATION ON)
    set(SJK1238 OFF)
    option(SIMULATION
            "HARDWARE: SIMULATION SELECTED" ON)
endif (HARDWARE MATCHES SIMULATION)

if (HARDWARE MATCHES SJK1238)
    set(HARDWARE SJK1238)
    set(SJK1238 ON)
    set(SIMULATION OFF)
    option(SJK1238
            "HARDWARE: SJK1238 SELECTED" ON)
endif (HARDWARE MATCHES SJK1238)

set(MYSQL ON)
if (DATABASE MATCHES MYSQL)
    set(DATABASE MYSQL)
    set(MYSQL ON)
    option(MYSQL
            "DATABASE: MYSQL SELECTED" ON)
endif (DATABASE MATCHES MYSQL)


configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

add_subdirectory(src bin)


