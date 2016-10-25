set(GLOBAL_SOURCE_FILES
        Logger.cpp
        Logger.h
        Key.cpp
        Key.h
        )


find_library(GLOG glog)

add_library(global STATIC ${GLOBAL_SOURCE_FILES})

target_link_libraries(global
        ${GLOG}
        )
