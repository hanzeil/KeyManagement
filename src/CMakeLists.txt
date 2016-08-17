
set(SOURCE_FILES main.cpp)
add_executable(KeyManagement ${SOURCE_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#For Boost
include_directories("/usr/local/lib/boost-1.61/")
find_library(BOOST_LOG boost_log)
add_definitions(-DBOOST_LOG_DYN_LINK)


#For linking libraries.
add_subdirectory(database)
add_subdirectory(encryption_device)
target_link_libraries(KeyManagement
        database
        encryption_device
        ${BOOST_LOG})
