set(HANDLER_SOURCE_FILES
        KeyHandler.cpp
        KeyHandler.h
        AuthenticationHandler.cpp
        AuthenticationHandler.h
        )


add_library(handler STATIC ${HANDLER_SOURCE_FILES})

target_link_libraries(handler
        database
        encryption_device
        global
        )
