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
                LOG(INFO) << "TCP:: A client requests to create a key";
            }
            catch (std::runtime_error e) {
                LOG(ERROR) << e.what();
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
                LOG(INFO) << "TCP:: A client requests to find a key by key id";
            }
            catch (std::runtime_error e) {
                LOG(ERROR) << e.what();
                rep.ErrorContent();
                status_ = error;
            }
        } else if (req.method == "Authentication1") {
            auto status = auth_handler->
                    HandleAuthentication1(req.data_alternate,
                                          req.data);
            if (!status) {
                LOG(ERROR) << "TCP:: Bad authentication step 1";
                rep.ErrorContent();
                status_ = error;
                return;
            }
            auto data_pack = auth_handler->GetAuthentication1();
            rep.ToContent(data_pack);
            LOG(INFO) << "TCP:: A client requests to authenticate for 1st step";
        } else if (req.method == "Authentication2") {
            auto status = auth_handler->
                    HandleAuthentication2(req.data);
            if (!status) {
                LOG(ERROR) << "TCP:: Bad authentication step 2";
                rep.ErrorContent();
                status_ = error;
                return;
            }
            auto data_pack = auth_handler->GetAuthentication2();
            rep.ToContent(data_pack);
            LOG(INFO) << "TCP:: A client requests to authenticate for 2nd step";
        }
    }

    void RequestHandler::ReplyError(Reply &rep) {
        LOG(ERROR) << "TCP:: Bad Request";
        rep.ErrorContent();
        status_ = error;
    }

    void RequestHandler::BindThreadTask(std::shared_ptr<ThreadTask> task) {
        auth_handler = std::make_shared<handler::AuthenticationHandler>();
        key_handler_ = std::make_shared<handler::KeyHandler>(task->db_, task->hardware_);
    }

    void RequestHandler::Reset() {
        status_ = authentication_1;
    }
}
