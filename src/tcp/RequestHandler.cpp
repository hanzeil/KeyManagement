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

namespace http {
    namespace server {
        RequestHandler::RequestHandler() {
        }

        void RequestHandler::handle_request(const Request &req, Reply &rep) {
            if (req.method == "CreateKey") {
                auto key = key_handler_.CreateKey();
                rep.to_content(key);
            }
            else if (req.method == "FindKeyByID") {
                KeyIdType key_id;
                for (std::size_t i = 0; i < Key::kKeyIdLen; i++) {
                    key_id[i] = req.data[i];
                }
                auto key = key_handler_.FindKeyByID(key_id);
                rep.to_content(key);
            }
            else if (req.method == "CertAuthority") {
            }
        }
    }
}
