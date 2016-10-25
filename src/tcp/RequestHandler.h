//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_REQUESTHANDLER_H
#define KEYMANAGEMENT_REQUESTHANDLER_H

#include <string>
#include <glog/logging.h>
#include "ThreadTask.h"
#include "../handler/KeyHandler.h"
#include "../handler/AuthenticationHandler.h"

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

        void ReplyError(Reply &rep);

        void BindThreadTask(std::shared_ptr<ThreadTask> task);

        void Reset();

        enum Status {
            authentication_1,
            authentication_2,
            key_handle,
            error
        } status_;


    private:
        std::shared_ptr<handler::AuthenticationHandler> auth_handler;
        std::shared_ptr<handler::KeyHandler> key_handler_;
    };

} // namespace tcp


#endif //KEYMANAGEMENT_REQUESTHANDLER_H
