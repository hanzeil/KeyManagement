//
// Created by Hanzeil on 16-10-24.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_LOG_H
#define KEYMANAGEMENT_LOG_H

#include "global_define.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <glog/logging.h>
#include <experimental/filesystem>


namespace logger {

    namespace fs = std::experimental::filesystem;

    class Logger {

    public:

        static Logger GetInstance(std::string user_config_path);

        void SetLogRotationSize(std::size_t log_rotation_size);

        void SetLogMaxFiles(std::size_t log_max_files);

        void SetFilter(int level);

        void Init();


    private:

        Logger(const std::string &user_config_path);

        Logger(Logger &) = default;

        Logger &operator=(Logger &)= default;

        std::size_t log_rotation_size_ = 1;

        std::size_t log_max_files_ = 10;

        fs::path log_path_;
    };
}

#endif //KEYMANAGEMENT_LOG_H
