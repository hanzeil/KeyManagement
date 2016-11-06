cmake_minimum_required(VERSION 3.2)


set(SOURCE_FILES main.cpp)

find_library(BOOST_SYSTEM boost_system)
find_library(STDC++FS stdc++fs)

add_executable(keymanagement-loglevel
        ${SOURCE_FILES}
        )

target_link_libraries(keymanagement-loglevel
        config
        ${BOOST_SYSTEM}
        ${STDC++FS}
        )

install(TARGETS keymanagement-loglevel
        RUNTIME DESTINATION bin)
