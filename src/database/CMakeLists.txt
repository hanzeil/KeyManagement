set(DATABASE_LIB
        DBFactoryInterface.h
        DBFactoryInterface.cpp
        DBProductInterface.h
        DBProductInterface.cpp
        Key.h
        Key.cpp)

if (DATABASE MATCHES MYSQL)
    add_subdirectory(mysql)
    set(DATABASE_LIB ${DATABASE_LIB}
            mysql/MysqlFactory.h
            mysql/MysqlFactory.cpp
            mysql/MysqlDB.h
            mysql/MysqlDB.cpp
            )
endif (DATABASE MATCHES MYSQL)

add_library(database STATIC ${DATABASE_LIB})

if (DATABASE MATCHES MYSQL)
    # Mysql connector
    find_library(MYSQLCPPCONN mysqlcppconn-static)
    find_library(MYSQLCLIENT mysqlclient)

    target_link_libraries(database
            ${MYSQLCPPCONN}
            ${MYSQLCLIENT})
endif (DATABASE MATCHES MYSQL)

# UUID
find_library(UUID uuid)
# Pthread
find_library(PTHREAD pthread)

target_link_libraries(database
        ${UUID}
        ${PTHREAD})
