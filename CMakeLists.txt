cmake_minimum_required(VERSION 3.2)
project(keymanagement)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_SOURCE_DIR src/)
set(PROJECT_BINARY_DIR bin/)

# 宏参数设置
set(CONFIG_FILE_NAME keymanagement.conf)
# 默认编译选项
set(SIMULATION ON)
set(SJK_1238 OFF)
if (ENCRYPTION_DEVICE MATCHES SIMULATION)
    set(SIMULATION ON)
    set(SJK_1238 OFF)
    option(SIMULATION
            "HARDWARE: SIMULATION SELECTED" ON)
endif (ENCRYPTION_DEVICE MATCHES SIMULATION)

if (ENCRYPTION_DEVICE MATCHES SJK1238)
    set(SJK_1238 ON)
    set(SIMULATION OFF)
    option(SJK_1238
            "HARDWARE: SJK1238 SELECTED" ON)
endif (ENCRYPTION_DEVICE MATCHES SJK1238)


set(MYSQL ON)
if (DATABASE MATCHES MYSQL)
    set(DATABASE MYSQL)
    set(MYSQL ON)
    option(MYSQL
            "DATABASE: MYSQL SELECTED" ON)
endif (DATABASE MATCHES MYSQL)

configure_file(
        "${PROJECT_SOURCE_DIR}/global_define.h.in"
        "${PROJECT_BINARY_DIR}/global_define.h"
)

add_subdirectory(src bin)

