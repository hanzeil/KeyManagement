//
// Created by root on 12-10-7.
//

#ifndef KEYMANAGEMENT_MYSQL_DB_H
#define KEYMANAGEMENT_MYSQL_DB_H

#include "DBProductInterface.h"
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
#include <sstream>
#include <time.h>

class MysqlDB : public DBProductInterface {
public:
    explicit MysqlDB() = default;

    ~MysqlDB();

    bool connect(std::string username, std::string password);

    bool insert_key(Key k);

private:

    sql::mysql::MySQL_Driver *driver_ = nullptr;
    sql::Connection *con_ = nullptr;
    std::string db_name_ = "symmetric_key";
    std::string key_table_name_ = "test";

    static std::string from_unix_time(time_t unix_timestamp);
};


#endif //KEYMANAGEMENT_MYSQL_DB_H
