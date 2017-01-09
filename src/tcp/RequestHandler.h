//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// 处理每次接收到的客户端请求，例如认证或者密钥请求，
// 从而调用相应的业务处理对象进行处理并产生回复流
//


#ifndef KEYMANAGEMENT_REQUESTHANDLER_H
#define KEYMANAGEMENT_REQUESTHANDLER_H

#include <string>
#include <glog/logging.h>
#include "ThreadTask.h"
#include "../handler/KeyHandler.h"
#include "../handler/AuthenticationHandler.h"
#include "DataPacket.h"

namespace tcp {

    struct Reply;
    struct Request;

/// The common handler for all incoming requests.
    class RequestHandler {
    public:
        RequestHandler(const RequestHandler &) = delete;

        RequestHandler &operator=(const RequestHandler &) = delete;

        /// Construct with a directory containing files to be served.
        explicit RequestHandler();

        /// Handle a request and produce a reply.
        void HandleRequest(const Request &req, Reply &rep);

        /// 如果出错，回复全0
        void ReplyError(Reply &rep);

        /// 加载每个线程开启的任务
        void BindThreadTask(std::shared_ptr<ThreadTask> task);

        /// 重置状态
        void Reset();

        /// 当前需要接收的请求的类型
        enum Status {
            authentication_1,
            authentication_2,
            key_handle,
            error
        } status_;


    private:
        /// 将数据封装成通信数据包
        template<typename InputIterator>
        inline DataPacket MakeDataPacket(uint32_t flag,
                                         InputIterator rand_begin,
                                         InputIterator rand_end,
                                         uint32_t len) {

            DataPacket data_packet;
            data_packet.flag = flag;
            for (auto i = 0; i < 16 && rand_begin != rand_end; i++) {
                data_packet.rand[i] = *rand_begin++;
            }
            data_packet.len = len;
            data_packet.point_ignored;
            return data_packet;
        }

        /// 认证处理对象
        std::shared_ptr<handler::AuthenticationHandler> auth_handler;
        /// 密钥处理对象
        std::shared_ptr<handler::KeyHandler> key_handler_;
    };

} // namespace tcp


#endif //KEYMANAGEMENT_REQUESTHANDLER_H
