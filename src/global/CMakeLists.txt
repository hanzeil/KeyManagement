set(GLOBAL_SOURCE_FILES
        Logger.cpp
        Logger.h
        Key.cpp
        Key.h
        LoggerServer.cpp
        LoggerServer.h)


find_library(GLOG glog)
find_library(BOOST_DATE_TIME boost_date_time)
find_library(BOOST_THREAD boost_thread)
find_library(PTHREAD pthread)
find_library(STDC++FS stdc++fs)
find_library(UUID uuid)

add_library(global STATIC ${GLOBAL_SOURCE_FILES})

target_link_libraries(global
        ${GLOG}
        ${BOOST_DATE_TIME}
        ${BOOST_THREAD}
        ${PTHREAD}
        ${STDC++FS}
	${UUID}
        )
