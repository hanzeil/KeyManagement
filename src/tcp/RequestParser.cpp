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

    RequestParser::ResultType RequestParser::Consume(Request &req, char input) {
        switch (state_) {
            case method_1:
                if (!IsChar(input)) {
                    return bad;
                }
                else {
                    if (input == 'c') {
                        req.method = "CreateKey";
                        return good;
                    }
                    else if (input == 'f') {
                        state_ = method_2;
                        req.method = "FindKeyByID";
                        return indeterminate;
                    }
                    return bad;
                }
            case method_2:
                state_ = length_1;
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

    bool RequestParser::IsChar(int c) {
        return c >= 0 && c <= 127;
    }
}
