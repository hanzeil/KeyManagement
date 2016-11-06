set(CONFIG_SOURCE_FILES
        Config.cpp
        Config.h
        )

add_library(config STATIC ${CONFIG_SOURCE_FILES})

target_link_libraries(config
        global
        )

