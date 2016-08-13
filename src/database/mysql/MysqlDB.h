//
// Created by root on 12-10-7.
//

#ifndef KEYMANAGEMENT_MYSQL_DB_H
#define KEYMANAGEMENT_MYSQL_DB_H

#include "../DBProductInterface.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <stdexcept>
#include <boost/log/trivial.hpp>
#include <string>
#include <cstring>
#include <sstream>
#include <time.h>
#include <cstdlib>

class MysqlDB : public DBProductInterface {
public:
    explicit MysqlDB() = default;

    ~MysqlDB();

    bool connect(std::string username, std::string password);

    bool insertKey(Key k);

    Key *getKey(unsigned char *key_id);

private:
    static std::string unixTime2MysqlTime(time_t unix_timestamp);

    static std::time_t mysqlTime2UnixTime(std::string mysql_time);

    sql::mysql::MySQL_Driver *driver_ = nullptr;
    sql::Connection *con_ = nullptr;
    std::string db_name_ = "symmetric_key";
    std::string key_table_name_ = "test";

};


#endif //KEYMANAGEMENT_MYSQL_DB_H
