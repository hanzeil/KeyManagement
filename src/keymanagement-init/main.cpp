//
// Created by Hanzeil on 16-8-15.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 初始化数据库，并为该数据库创建用户名和密码(可选)
//

#include <iostream>
#include "global_define.h"
#include <unistd.h>
#include <termios.h>
#include "../database/DBFactoryInterface.h"

#ifdef MYSQL

#include "../database/mysql/MysqlFactory.h"

#endif

char getch() {
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("*");
    return buf;
}

int main() {

#ifdef MYSQL
    auto dbfactory = std::make_shared<database::MysqlFactory>();
#endif
    auto db = dbfactory->CreateProduct();
    std::string url;
    std::string port;
    std::string user;
    std::string password;
    std::cout << "Warning: Initialization will lose all data" << std::endl;
    char c;
    std::cout << "Please enter the database url(default 127.0.0.1):";
    while ((c = (char) getchar()) && c != '\n') {
        url.push_back(c);
    }
    if (url.size() == 0) {
        url = "127.0.0.1";
    }
    std::cout << "Please enter the database port(default 3306):";
    while ((c = (char) getchar()) && c != '\n') {
        port.push_back(c);
    }
    if (port.size() == 0) {
        port = "3306";
    }
    std::cout << "Please enter the database superuser name(default root):";
    while ((c = (char) getchar()) && c != '\n') {
        user.push_back(c);
    }
    if (user.size() == 0) {
        user = "root";
    }

    std::cout << "Please enter the password (default empty):";
    while ((c = getch()) && c != '\n') {
        password.push_back(c);
    }
    std::cout << std::endl;
    try {
        db->Connect(url, port, user, password);
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
        _exit(111);
    }
    try {
        std::cout << "Creating database and data tables" << std::endl;
        db->Init();
    }
    catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
        _exit(112);
    }

    std::cout << "Whether to create a user for this database and data tables?(Y/N):";
    c = (char) getchar();
    if (c == 'y' || c == 'Y' || c == '\n') {
        while (1) {
            std::string user_new;
            std::string password_new;
            std::string password_repeat;

            std::cout << "Please enter a new database user name:";
            std::cin >> user_new;

            std::cout << "Please enter the password:";
            while ((c = getch()) && c != '\n') {
                password_new.push_back(c);
            }
            std::cout << std::endl;

            std::cout << "Please confirm the password:";
            while ((c = getch()) && c != '\n') {
                password_repeat.push_back(c);
            }
            std::cout << std::endl;

            if (password_new != password_repeat) {
                std::cout << "The new password and confirming password do not match, please re-enter" << std::endl;
                continue;
            }

            try {
                db->CreateUser(user_new, password_new);
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
                _exit(113);
            }
            break;
        }
    } else {
        std::cout << "Initialization is complete" << std::endl;
        return 0;
    }
    std::cout << "Initialization is complete" << std::endl;
    return 0;
}