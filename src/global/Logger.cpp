//
// Created by Hanzeil on 16-10-24.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#include "Logger.h"

namespace logger {
    Logger Logger::GetInstance(std::string user_config_path) {
        static Logger single(user_config_path);
        return single;
    }

    void Logger::SetLogMaxFiles(std::size_t log_max_files) {
        log_max_files_ = log_max_files;
    }

    void Logger::SetLogRotationSize(std::size_t log_rotation_size) {
        log_rotation_size_ = log_rotation_size;
    }

    void Logger::SetFilter(int level) {
        FLAGS_minloglevel = level;
    }

    void Logger::Init() {
        google::InitGoogleLogging(PROJECT_NAME);
        FLAGS_max_log_size = (google::int32) log_rotation_size_;
        FLAGS_minloglevel = 0;
        if (!fs::exists(log_path_)) {
            fs::create_directories(log_path_);
        }
        FLAGS_log_dir = log_path_;
        LOG(INFO) << "Initialization succeeded";
    }

    Logger::Logger(const std::string &user_config_path) {
        log_path_.assign(user_config_path + "/logs/");
    }
}