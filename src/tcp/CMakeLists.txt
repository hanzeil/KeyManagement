set(TCP_SOURCE_FILES
        connection.cpp
        connection.h
        connection_manager.cpp
        connection_manager.h
        header.h
        mime_types.cpp
        mime_types.h
        reply.cpp
        reply.h
        request.h
        Request.h
        RequestHandler.cpp
        RequestHandler.h
        request_handler.cpp
        request_handler.h
        RequestParser.cpp
        RequestParser.h
        request_parser.cpp
        request_parser.h
        server.cpp
        server.h)

add_library(tcp STATIC ${TCP_SOURCE_FILES})
