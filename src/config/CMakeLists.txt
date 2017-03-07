set(CONFIG_SOURCE_FILES
        Config.cpp
        Config.h
        ConfigSingleton.cpp
        ConfigSingleton.h
        )

add_library(config STATIC ${CONFIG_SOURCE_FILES})

target_link_libraries(config
        global
        )

