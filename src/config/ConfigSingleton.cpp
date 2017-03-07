//
// Created by Hanzeil on 16-12-1.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "ConfigSingleton.h"

namespace config {
    ConfigSingleton::ConfigSingleton(){
        Config config_settings;

        config_settings.ReadFile(config_settings.GetConifgPath(CONFIG_FILE_NAME));

        // Server
        port_ = config_settings.Read<std::string>("PORT");
        port_int_ = config_settings.Read<std::size_t>("PORT");
        address_ = config_settings.Read<std::string>("ADDRESS");
        num_threads_ = config_settings.Read<std::size_t>("THREADS");

        // Log
        log_server_port_ = config_settings.Read<std::string>("LOG_SERVER_PORT");
        log_server_port_int_ = config_settings.Read<std::size_t>("LOG_SERVER_PORT");
        log_rotation_size_ = config_settings.Read<std::size_t>("LOG_ROTATION_SIZE");
        log_max_files_ = config_settings.Read<std::size_t>("LOG_MAX_FILES");
        log_scan_duration_ = config_settings.Read<std::size_t>("LOG_SCAN_DURATION");

        // Database
        db_url_ = config_settings.Read<std::string>("DATABASE_URL");
        db_port_ = config_settings.Read<std::string>("DATABASE_PORT");
        db_port_int_ = config_settings.Read<std::size_t>("DATABASE_PORT");
        db_user_name_ = config_settings.Read<std::string>("DATABASE_USER_NAME");
        db_user_password_ = config_settings.Read<std::string>("DATABASE_PASSWORD");

    }
    ConfigSingleton &ConfigSingleton::GetInstance() {
        static ConfigSingleton single;


        return single;
    }

}