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
                            return indeterminate;
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
                    if (!is_char(input)) {
                        return bad;
                    }
                    else {
                        tmp_c = input;
                        state_ = length_2;
                        return indeterminate;
                    }
                case length_2:
                    if (!is_char(input)) {
                        return bad;
                    }
                    else {
                        state_ = data;
                        req.length = (unsigned int) tmp_c * 256 + (unsigned int) input;
                        return indeterminate;
                    }
                case data:
                    if (!is_char(input)) {
                        return bad;
                    }
                    if (input == '\r') {
                        std::cout<<"~~"<<std::endl;
                        state_ = expecting_newline_2;
                        return indeterminate;
                    }
                    req.data.push_back(input);
                    return indeterminate;
                case expecting_newline_2:
                    if (input == '\n') {
                        std::cout<<"~~~~"<<std::endl;
                        return good;
                    }
                    else {
                        return bad;
                    }
                default:
                    return bad;
            }

        }

        bool RequestParser::is_char(int c) {
            return c >= 0 && c <= 127;
        }
    }
}
