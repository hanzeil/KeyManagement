#include <iostream>
#include "config.h"
#include "../database/DBFactoryInterface.h"

#ifdef MYSQL

#include "../database/mysql/MysqlFactory.h"

#endif

int main() {

#ifdef MYSQL
    auto dbfactory = std::make_shared<database::MysqlFactory>();
#endif
    auto db = dbfactory->CreateProduct();
    std::string user = "root";
    std::string password;
    std::cout << "警告：初始化操作将会丢失所有数据" << std::endl;
    std::cout << "请输入数据库超级用户名：";
    std::cin >> user;
    std::cout << "请输入密码：";
    std::cin >> password;
    try {
        db->Connect(user, password);
    }
    catch (std::runtime_error e) {
        std::cout << std::endl;
        std::cout << e.what() << std::endl;
        _exit(111);
    }
    std::cout << "正在创建数据库和数据表：" << std::endl;
    db->Init();
    return 0;
}