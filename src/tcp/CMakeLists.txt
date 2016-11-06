set(TCP_SOURCE_FILES
        ThreadTask.cpp
        ThreadTask.h
        Connection.cpp
        Connection.h
        ConnectionManager.cpp
        ConnectionManager.h
        Request.cpp
        Request.h
        Reply.cpp
        Reply.h
        RequestHandler.cpp
        RequestHandler.h
        RequestParser.cpp
        RequestParser.h
        Server.cpp
        Server.h
        )

find_library(BOOST_SYSTEM boost_system)

find_library(BOOST_THREAD boost_thread)

find_library(PTHREAD pthread)

add_library(tcp STATIC ${TCP_SOURCE_FILES})

target_link_libraries(tcp
        global
        handler
        config
        ${BOOST_SYSTEM}
        ${BOOST_THREAD}
        ${PTHREAD}
        )
