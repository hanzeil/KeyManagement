cmake_minimum_required(VERSION 3.5)
project(KeyManagement_test_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_library(BOOST_SYSTEM boost_system)
find_library(BOOST_THREAD boost_thread)
find_library(PTHREAD pthread)

set(SOURCE_FILES main.cpp)
add_executable(KeyManagement_test_client
        ${SOURCE_FILES}
        )

target_link_libraries(KeyManagement_test_client
        ${BOOST_SYSTEM}
        ${BOOST_THREAD}
        ${PTHREAD})
