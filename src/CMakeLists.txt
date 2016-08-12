
add_subdirectory(database)
add_subdirectory(hardware)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -luuid -lpthread -DBOOST_LOG_DYN_LINK")
set(SOURCE_FILES main.cpp)
add_executable(KeyManagement ${SOURCE_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#For Boost
#include_directories(/usr/local/lib/boost-1.61)
#For mysql connector include
#include_directories(/usr/local/include/)
#link_directories(/usr/local/mysql/lib/)
#link_directories(/usr/local/lib64/)
#For linking libraries..
find_library(MYSQLCPPCONN mysqlcppconn-static)
find_library(MYSQLCLIENT mysqlclient)
find_library(BOOST_LOG boost_log)
target_link_libraries (KeyManagement
        database
        hardware
        ${MYSQLCPPCONN}
        ${MYSQLCLIENT}
        ${BOOST_LOG})
