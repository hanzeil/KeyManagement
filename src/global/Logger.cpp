//
// Created by Hanzeil on 16-10-24.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#include "Logger.h"

Logger &Logger::GetInstance() {
    static Logger single;
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

void Logger::Init(std::string user_config_path) {
    log_path_.assign(user_config_path + "/logs/");
    google::InitGoogleLogging(PROJECT_NAME);
    FLAGS_max_log_size = (google::int32) log_rotation_size_;
    FLAGS_minloglevel = 0;
    if (!fs::exists(log_path_)) {
        fs::create_directories(log_path_);
    }
    FLAGS_log_dir = log_path_;
    FLAGS_stop_logging_if_full_disk = true;
    FLAGS_logbufsecs = 1;

    LOG(INFO) << "Initialization succeeded";
}

void Logger::ScanRotation() {
    // 该目录下有多少INFO日志文件
    LOG(INFO) << "Logger:: Checking if the number of log files is overflowed";
    auto cnt = std::count_if(
            fs::directory_iterator(log_path_),
            fs::directory_iterator(),
            [](const fs::directory_entry &d) {
                return fs::is_regular_file(d.path()) && !fs::is_symlink(d.path())
                       && d.path().string().find("INFO") != std::string::npos;
            });
    // 如果日志文件个数大于最高文件个数, 对日志文件的新旧进行排序, 只留阈值的一半, 过旧的日志全部删除
    if (cnt > log_max_files_) {
        std::vector<std::pair<fs::path, fs::file_time_type> > files;
        for (auto &d : fs::directory_iterator(log_path_)) {
            if (fs::is_regular_file(d.path()) && !fs::is_symlink(d.path())
                && d.path().string().find("INFO") != std::string::npos) {
                files.push_back({d.path(), fs::last_write_time(d.path())});
            }
        }
        // sort
        std::sort(files.begin(), files.end(),
                  [](std::pair<fs::path, fs::file_time_type> &item1,
                     std::pair<fs::path, fs::file_time_type> &item2) {
                      return item1.second < item2.second;
                  });
        // 只留一半
        for (std::size_t i = 0; i < files.size() - log_max_files_ / 2; i++) {
            fs::remove(files[i].first);
            LOG(INFO) << "Logger:: Removed log file: " << files[i].first;
        }

    }
}

