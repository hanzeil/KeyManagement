set(TCP_SOURCE_FILES
        ThreadTask.cpp
        ThreadTask.h
        connection.cpp
        connection.h
        connection_manager.cpp
        connection_manager.h
        Request.cpp
        Request.h
        Reply.cpp
        Reply.h
        RequestHandler.cpp
        RequestHandler.h
        RequestParser.cpp
        RequestParser.h
        server.cpp
        server.h)

find_library(BOOST_SYSTEM boost_system)

find_library(BOOST_THREAD boost_thread)

add_library(tcp STATIC ${TCP_SOURCE_FILES})

target_link_libraries(tcp
        ${BOOST_SYSTEM}
        ${BOOST_THREAD})
