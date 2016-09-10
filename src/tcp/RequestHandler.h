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
#include "../handler/KeyHandler.h"

namespace http {
    namespace server {

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
            void handle_request(const Request &req, Reply &rep);

            enum status {
                ca1,
                ca2,
                key_handle,
            };

        private:
        };

    } // namespace server
} // namespace http


#endif //KEYMANAGEMENT_REQUESTHANDLER_H
