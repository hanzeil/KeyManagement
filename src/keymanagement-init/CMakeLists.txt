cmake_minimum_required(VERSION 3.2)


set(SOURCE_FILES main.cpp)
add_executable(keymanagement-init
        ${SOURCE_FILES}
        )

target_link_libraries(keymanagement-init
        database
        ${BOOST_LOG}
        )
install(TARGETS keymanagement-init
        RUNTIME DESTINATION bin)
