//
// Created by Hanzeil on 16-10-24.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 日志管理类


#ifndef KEYMANAGEMENT_LOG_H
#define KEYMANAGEMENT_LOG_H

#include "global_define.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <glog/logging.h>
#include <experimental/filesystem>
#include <algorithm>
#include <vector>


namespace fs = std::experimental::filesystem;

/// 该类使用单例模式保证只有一个实例的存在
/// 该类具有一些日志管理的功能
/// Sample usage:
/// Logger::GetInstance().Init(user_config_path)
class Logger {

public:

    /// 拷贝构造函数
    /// 阻止拷贝
    Logger(Logger &) = default;

    /// 拷贝赋值函数
    /// 阻止赋值
    Logger &operator=(Logger &)= default;

    /// 获取实例
    static Logger &GetInstance();

    /// 设置每个日志文件的大小上限 单位MB
    void SetLogRotationSize(std::size_t log_rotation_size);

    /// 设置日志文件个数上限
    void SetLogMaxFiles(std::size_t log_max_files);

    /// 设置filter，使其只输出指定的日志类型
    void SetFilter(int level);

    /// 初始化LOG
    void Init(std::string user_config_path);

    /// 扫描日志个数，如果个数超过了阈值，则删除一半
    void ScanRotation();

private:

    Logger() = default;


    std::size_t log_rotation_size_ = 1;

    std::size_t log_max_files_ = 10;

    fs::path log_path_;
};

#endif //KEYMANAGEMENT_LOG_H
