
set(SOURCE_FILES main.cpp)
add_executable(KeyManagement ${SOURCE_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#For Boost
find_library(BOOST_LOG boost_log)
add_definitions(-DBOOST_LOG_DYN_LINK)

#For linking libraries..
add_subdirectory(database)
add_subdirectory(hardware)
target_link_libraries(KeyManagement
        database
        hardware
        ${BOOST_LOG})
