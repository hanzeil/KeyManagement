cmake_minimum_required(VERSION 3.6)
project(KeyManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lswsds -DBOOST_LOG_DYN_LINK")

set(SOURCE_FILES main.cpp DataStructure.cpp DataStructure.h DBFactoryInterface.cpp DBFactoryInterface.h DBProductInterface.cpp DBProductInterface.h MysqlDB.cpp MysqlDB.h MysqlFactory.cpp MysqlFactory.h HardwareFactoryInterface.cpp HardwareFactoryInterface.h HardwareProductInterface.cpp HardwareProductInterface.h SJK1238.cpp SJK1238.h SJK1238Factory.cpp SJK1238Factory.h)
add_executable(KeyManagement ${SOURCE_FILES})
#For Boost
include_directories(/usr/local/lib/boost-1.61/)
#For mysql connector include
include_directories(/usr/local/include/)
link_directories(/usr/local/mysql/lib/)
link_directories(/usr/local/lib64/)
#For linking libraries..
find_library(MYSQLCPPCONN mysqlcppconn-static)
find_library(MYSQLCLIENT mysqlclient)
find_library(BOOST_LOG boost_log)
target_link_libraries (KeyManagement ${MYSQLCPPCONN} ${MYSQLCLIENT} ${BOOST_LOG})
