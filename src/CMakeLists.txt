
set(SOURCE_FILES main.cpp)
add_executable(keymanagement ${SOURCE_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#For Boost
include_directories("/usr/local/lib/boost-1.61/")
find_library(BOOST_LOG boost_log)
add_definitions(-DBOOST_LOG_DYN_LINK)


add_subdirectory(database)
add_subdirectory(encryption_device)
add_subdirectory(tcp)
add_subdirectory(handler)
add_subdirectory(keymanagement-test-client)
add_subdirectory(keymanagement-init)

#For linking libraries.
target_link_libraries(keymanagement
        tcp
        handler
        database
        encryption_device
        ${BOOST_LOG})

install(TARGETS keymanagement
        RUNTIME DESTINATION bin)
