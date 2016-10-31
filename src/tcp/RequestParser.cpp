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

namespace tcp {
    RequestParser::RequestParser()
            : state_(method_1) {
    }

    void RequestParser::Reset() {
        state_ = method_1;
    }

    RequestParser::ResultType RequestParser::Consume(Request &req, unsigned char input) {
        switch (state_) {
            case method_1:
                if (input == 'c') {
                    req.method = "CreateKey";
                    return good;
                } else if (input == 'f') {
                    state_ = method_2;
                    req.method = "FindKeyByID";
                    return indeterminate;
                } else if (input == 'a') {
                    state_ = method_2;
                    req.method = "Authentication";
                    return indeterminate;
                }
                return bad;
            case method_2:
                state_ = length_1;
                if (req.method == "Authentication") {
                    if (input == '1') {
                        req.method = "Authentication1";
                        state_ = data_alternate;
                        return indeterminate;
                    } else if (input == '2') {
                        state_ = length_1;
                        req.method = "Authentication2";
                        return indeterminate;
                    } else {
                        return bad;
                    }
                }
                return indeterminate;
            case data_alternate:
                req.data_alternate.push_back(input);
                if (req.data_alternate.size() == handler::AuthenticationHandler::kRandLen) {
                    state_ = length_1;
                }
                return indeterminate;
            case length_1:
                tmp_c_ = input;
                state_ = length_2;
                return indeterminate;
            case length_2:
                state_ = data;
                req.length = (std::size_t) input * 256 + (std::size_t) tmp_c_;
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
}
