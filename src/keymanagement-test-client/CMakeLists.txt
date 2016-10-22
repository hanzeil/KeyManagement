cmake_minimum_required(VERSION 3.2)

find_library(BOOST_SYSTEM boost_system)
find_library(BOOST_THREAD boost_thread)
find_library(PTHREAD pthread)

set(SOURCE_FILES main.cpp)
add_executable(keymanagement-test-client
        ${SOURCE_FILES}
        )

target_link_libraries(keymanagement-test-client
        ${BOOST_SYSTEM}
        ${BOOST_THREAD}
        ${PTHREAD})
install(TARGETS keymanagement-test-client
        RUNTIME DESTINATION bin)
