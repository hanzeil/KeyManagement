set(HANDLER_SOURCE_FILES
        KeyHandler.cpp
        KeyHandler.h
        AuthenticationHandler.cpp
        AuthenticationHandler.h
        )

add_library(handler STATIC ${HANDLER_SOURCE_FILES})
