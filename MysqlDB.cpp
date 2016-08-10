//
// Created by root on 12-10-7.
//

#include "MysqlDB.h"

MysqlDB::~MysqlDB() {
    delete driver_;
    delete con_;
}

bool MysqlDB::connect(std::string username, std::string password) {
    driver_ = sql::mysql::get_mysql_driver_instance();
    try {
        con_ = driver_->connect("tcp://127.0.0.1:3306", username, password);
        con_->setSchema(db_name_);
    }
    catch (std::runtime_error e) {
        BOOST_LOG_TRIVIAL(error) << "Database: " << e.what();
        return false;
    }
    BOOST_LOG_TRIVIAL(info) << "Database: Connect Mysql";
    return true;
}

bool MysqlDB::insert_key(Key k) {
    try {
        auto pstmt = con_->prepareStatement("INSERT INTO " + this->key_table_name_ + " VALUES (?,?,?)");
        pstmt->setString(1, k.key_id_);
        pstmt->setString(2, k.key_value_);
        pstmt->setString(3, from_unix_time(k.generated_time_));
        pstmt->execute();
        delete pstmt;
    }
    catch (std::runtime_error e) {
        BOOST_LOG_TRIVIAL(error) << "Database: " << e.what();
        return false;
    }
    BOOST_LOG_TRIVIAL(info) << "Database: Insert key";
    return true;
}

std::string MysqlDB::from_unix_time(time_t unix_timestamp) {
    std::string result;
    struct tm t_tm = *localtime(&unix_timestamp);
    char s[20] = {0};
    std::strftime(s, sizeof(s), "%Y/%m/%d %H:%M:%S", &t_tm);
    result = s;
    return result;
}


