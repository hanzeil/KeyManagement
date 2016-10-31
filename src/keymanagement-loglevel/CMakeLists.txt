cmake_minimum_required(VERSION 3.2)


set(SOURCE_FILES main.cpp)

find_library(BOOST_DATE_TIME boost_date_time)
find_library(BOOST_SYSTEM boost_system)
find_library(BOOST_THREAD boost_thread)
find_library(PTHREAD pthread)

add_executable(keymanagement-loglevel
        ${SOURCE_FILES}
        )

target_link_libraries(keymanagement-loglevel
        ${BOOST_DATE_TIME}
        ${BOOST_SYSTEM}
        ${BOOST_THREAD}
        ${PTHREAD}
        )

install(TARGETS keymanagement-loglevel
        RUNTIME DESTINATION bin)
