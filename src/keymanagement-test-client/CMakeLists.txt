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
        DataPacket.h
        )
find_library(STDC++FS stdc++fs)
find_library(SKF SKF)
find_library(BOOST_SYSTEM boost_system)
find_library(BOOST_THREAD boost_thread)
find_library(PTHREAD pthread)

add_executable(keymanagement-test-client
        ${SOURCE_FILES}
        )

target_link_libraries(keymanagement-test-client
        ${BOOST_SYSTEM}
        ${STDC++FS}
        ${SKF}
        ${BOOST_THREAD}
        ${PTHREAD}
        config
        global
        )

install(TARGETS keymanagement-test-client
        RUNTIME DESTINATION bin)
