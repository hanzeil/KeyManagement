//
// Created by Hanzeil on 16-9-6.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 负责处理客户端发来的密钥请求
//


#ifndef KEYMANAGEMENT_KEYHANDLER_H
#define KEYMANAGEMENT_KEYHANDLER_H

#include <memory>
#include "global_define.h"
#include "../global/Key.h"
#include "../database/DBProductInterface.h"
#include "../database/mysql/MysqlFactory.h"
#include "../config/ConfigSingleton.h"
#include "../encryption_device/EncryptionDeviceProductInterface.h"
#include <glog/logging.h>

namespace handler {


    /// 客户端的密钥请求处理
    /// 包括：产生新的密钥，根据密钥ID查找密钥
    class KeyHandler {
    public:
        KeyHandler(const KeyHandler &) = delete;

        /// 拷贝构造函数
        /// 阻止拷贝
        KeyHandler &operator=(const KeyHandler &)= delete;

        /// 拷贝赋值函数
        /// 阻止拷贝
        KeyHandler(std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware);

        /// 创建密钥
        Key CreateKey();

        /// 根据密钥ID查找密钥
        Key FindKeyByID(KeyIdType key_id);

    private:
        /// 硬件模块接口
        std::shared_ptr<encryption_device::EncryptionDeviceProductInterface> hardware_;
        /// 数据库模块接口
        std::shared_ptr<database::DBProductInterface> db_;
    };

}

#endif //KEYMANAGEMENT_KEYHANDLER_H
