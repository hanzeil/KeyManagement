cmake_minimum_required(VERSION 3.2)


set(SOURCE_FILES
        main.cpp
        ServerAuthen.cpp
        ServerAuthen.h
        Signature.cpp
        Signature.h
        SKF.h
        skf_type.h
        base_type.h
        Reply.cpp
        Reply.h
        )
find_library(STDC++FS stdc++fs)

find_library(SKF SKF)

add_executable(keymanagement-test-client
        ${SOURCE_FILES}
        )

find_library(BOOST_SYSTEM boost_system)

target_link_libraries(keymanagement-test-client
        config
        ${BOOST_SYSTEM}
        ${STDC++FS}
        ${SKF}
        global
        )

install(TARGETS keymanagement-test-client
        RUNTIME DESTINATION bin)
