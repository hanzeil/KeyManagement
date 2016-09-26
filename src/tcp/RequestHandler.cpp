//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "RequestHandler.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Reply.h"
#include "Request.h"

namespace tcp {
    RequestHandler::RequestHandler()
            : status_(authentication_1) {
    }

    void RequestHandler::HandleRequest(const Request &req, Reply &rep) {
        if (req.method == "CreateKey") {
            try {
                auto key = key_handler_->CreateKey();
                rep.ToContent(key);
            }
            catch (std::runtime_error e) {
                BOOST_LOG_TRIVIAL(error) << e.what();
                rep.ErrorContent();
                status_ = error;
            }
        } else if (req.method == "FindKeyByID") {
            KeyIdType key_id;
            for (std::size_t i = 0; i < Key::kKeyIdLen; i++) {
                key_id[i] = req.data[i];
            }
            try {
                auto key = key_handler_->FindKeyByID(key_id);
                rep.ToContent(key);
            }
            catch (std::runtime_error e) {
                BOOST_LOG_TRIVIAL(error) << e.what();
                rep.ErrorContent();
                status_ = error;
            }
        } else if (req.method == "Authentication1") {

        } else if (req.method == "Authentication2") {

        }
    }

    void RequestHandler::ReplyError(Reply &rep) {
        BOOST_LOG_TRIVIAL(error) << "TCP:: Request Error";
        rep.ErrorContent();
        status_ = error;
    }

    void RequestHandler::BindThreadTask(std::shared_ptr<ThreadTask> task) {
        key_handler_ = std::make_shared<handler::KeyHandler>(task->db_, task->hardware_);
    }

    void RequestHandler::Reset() {
        status_ = authentication_1;
    }
}
