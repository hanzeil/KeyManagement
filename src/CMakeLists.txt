
set(SOURCE_FILES
        main.cpp
        )

add_executable(keymanagement
        ${SOURCE_FILES}
        ${CMAKE_SOURCE_DIR}/config/keymanagement.conf)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#For Boost
include_directories("/usr/local/lib/boost-1.61/")


add_subdirectory(global)
add_subdirectory(encryption_device)
add_subdirectory(tcp)
add_subdirectory(handler)
add_subdirectory(database)
add_subdirectory(config)
add_subdirectory(keymanagement-test-client)
add_subdirectory(keymanagement-init)
add_subdirectory(keymanagement-loglevel)

#For linking libraries.
target_link_libraries(keymanagement
        tcp
        global
        config
        )


install(TARGETS keymanagement
        RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/config/keymanagement.conf
        DESTINATION /etc/keymanagement/
        COMPONENT config)