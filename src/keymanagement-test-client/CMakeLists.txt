cmake_minimum_required(VERSION 3.2)


set(SOURCE_FILES main.cpp)
find_library(STDC++FS stdc++fs)

add_executable(keymanagement-test-client
        ${SOURCE_FILES}
        )

target_link_libraries(keymanagement-test-client
        config
        ${STDC++FS}
        global
        )

install(TARGETS keymanagement-test-client
        RUNTIME DESTINATION bin)
