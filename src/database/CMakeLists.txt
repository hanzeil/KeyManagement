set(DATABASE_LIB
        DBFactoryInterface.h
        DBFactoryInterface.cpp
        MysqlFactory.h
        MysqlFactory.cpp
        DBProductInterface.h
        DBProductInterface.cpp
        MysqlDB.h
        MysqlDB.cpp
        Key.h
        Key.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -luuid -lpthread -DBOOST_LOG_DYN_LINK")
add_library(database STATIC ${DATABASE_LIB})
