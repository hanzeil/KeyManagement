//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "RequestParser.h"
#include "Request.h"

namespace http {
    namespace server {
        RequestParser::RequestParser()
                : state_(method) {
        }

        void RequestParser::reset() {
            state_ = method;
        }

        RequestParser::result_type RequestParser::consume(Request &req, char input) {
            switch (state_) {
                case method:
                    if (!is_char(input)) {
                        return bad;
                    }
                    else {
                        if (input == 'a') {
                            state_ = length_1;
                            req.method = "CreateKey";
                            return good;
                        }
                        else if (input == 'b') {
                            state_ = length_1;
                            req.method = "FindKeyByID";
                            return indeterminate;
                        }
                        else if (input == 'c') {
                            state_ = length_1;
                            req.method = "CertAuthority";
                            return indeterminate;
                        }
                        return bad;
                    }
                case length_1:
                    tmp_c = input;
                    state_ = length_2;
                    return indeterminate;
                case length_2:
                    state_ = data;
                    req.length = (std::size_t) tmp_c * 256 + (std::size_t) input;
                    return indeterminate;
                case data:
                    if (req.data.size() == req.length) {
                        return good;
                    }
                    req.data.push_back(input);
                    return indeterminate;
                default:
                    return bad;
            }

        }

        bool RequestParser::is_char(int c) {
            return c >= 0 && c <= 127;
        }
    }
}
