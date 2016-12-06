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
                auto data_packet =
                        MakeDataPacket(
                                0xccccdddd,
                                key.key_id_.cbegin(),
                                key.key_id_.cend(),
                                (uint32_t) key.key_value_.size());
                rep.ToContent(data_packet);
                rep.ToContent(key.key_value_.cbegin(),
                              key.key_value_.cend());
                LOG(INFO) << "TCP:: CreateKey request is successful";
            }
            catch (std::runtime_error e) {
                LOG(WARNING) << "TCP:: CreateKey request is failed";
                LOG(WARNING) << e.what();
                rep.ErrorContent();
                status_ = error;
            }
        } else if (req.method == "FindKeyByID") {
            KeyIdType key_id;
            for (std::size_t i = 0; i < Key::kKeyIdLen; i++) {
                key_id[i] = req.rand[i];
            }
            try {
                auto key = key_handler_->FindKeyByID(key_id);
                auto data_packet =
                        MakeDataPacket(
                                0xccccdddd,
                                key.key_id_.cbegin(),
                                key.key_id_.cend(),
                                (uint32_t) key.key_value_.size());
                rep.ToContent(data_packet);
                rep.ToContent(key.key_value_.cbegin(),
                              key.key_value_.cend());
                LOG(INFO) << "TCP:: FindKeyByID request is successful";
            }
            catch (std::runtime_error e) {
                LOG(WARNING) << "TCP:: FindKeyByID request is failed";
                LOG(WARNING) << e.what();
                rep.ErrorContent();
                status_ = error;
            }
        } else if (req.method == "Authentication1") {
            auto status = auth_handler->
                    HandleAuthentication1(req.rand,
                                          req.data);
            if (!status) {
                LOG(WARNING) << "TCP:: 1st-authentication is failed";
                rep.ErrorContent();
                status_ = error;
                return;
            }
            std::vector<unsigned char> rand_server;
            std::vector<unsigned char> cert_server;

            std::tie(rand_server, cert_server) =
                    auth_handler->GetAuthentication1();
            auto data_packet =
                    MakeDataPacket(
                            0xaabb0000,
                            rand_server.cbegin(),
                            rand_server.cend(),
                            (uint32_t) cert_server.size());
            rep.ToContent(data_packet);
            rep.ToContent(cert_server.cbegin(),
                          cert_server.cend());
            LOG(INFO) << "1st-authenticate is successful";
        } else if (req.method == "Authentication2") {
            auto status = auth_handler->
                    HandleAuthentication2(req.data);
            if (!status) {
                LOG(WARNING) << "TCP:: 2nd-authentication is failed";
                rep.ErrorContent();
                status_ = error;
                return;
            }
            auto signed_data = auth_handler->GetAuthentication2();
            auto data_packet =
                    MakeDataPacket(
                            0xaabbccdd,
                            signed_data.cbegin(), //暂时不用数据,临时填充
                            signed_data.cbegin() + 16, //暂时不用数据,临时填充
                            (uint32_t) signed_data.size());
            rep.ToContent(data_packet);
            rep.ToContent(signed_data.cbegin(),
                          signed_data.cend());
            LOG(INFO) << "TCP:: 2nd-authenticate is successful";
        }
    }

    void RequestHandler::ReplyError(Reply &rep) {
        LOG(WARNING) << "TCP:: Bad Request";
        rep.ErrorContent();
        status_ = error;
    }

    void RequestHandler::BindThreadTask(std::shared_ptr<ThreadTask> task) {
        auth_handler = std::make_shared<handler::AuthenticationHandler>(task->hardware_);
        key_handler_ = std::make_shared<handler::KeyHandler>(task->hardware_);
    }

    void RequestHandler::Reset() {
        status_ = authentication_1;
    }
}
