set(DATABASE_SOURCE_FILES
        DBFactoryInterface.h
        DBFactoryInterface.cpp
        DBProductInterface.h
        DBProductInterface.cpp
        Key.h
        Key.cpp)
if (MYSQL)
    add_subdirectory(mysql)
    set(DATABASE_SOURCE_FILES ${DATABASE_SOURCE_FILES}
            mysql/MysqlFactory.h
            mysql/MysqlFactory.cpp
            mysql/MysqlDB.h
            mysql/MysqlDB.cpp
            )
endif (MYSQL)

add_library(database STATIC ${DATABASE_SOURCE_FILES})

# library
if (MYSQL)
    # Mysql connector
    find_library(MYSQLCPPCONN mysqlcppconn-static)
    find_library(MYSQLCLIENT mysqlclient)

    target_link_libraries(database
            ${MYSQLCPPCONN}
            ${MYSQLCLIENT})
endif (MYSQL)

# UUID
find_library(UUID uuid)
# Pthread
find_library(PTHREAD pthread)

target_link_libraries(database
        ${UUID}
        ${PTHREAD})
