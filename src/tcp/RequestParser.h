//
// Created by Hanzeil on 16-9-5.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_REQUESTPARSER_H
#define KEYMANAGEMENT_REQUESTPARSER_H

#include <iostream>
#include <tuple>
#include "Request.h"

namespace tcp {

    struct Request;

/// Parser for incoming requests.
    class RequestParser {
    public:
        /// Construct ready to parse the request method.
        RequestParser();

        /// Reset to initial parser state.
        void Reset();

        /// Result of parse.
        enum ResultType {
            good, bad, indeterminate
        };

        /// Parse some data. The enum return value is good when a complete request has
        /// been parsed, bad if the data is invalid, indeterminate when more data is
        /// required. The InputIterator return value indicates how much of the input
        /// has been consumed.
        template<typename InputIterator>
        std::tuple<ResultType, InputIterator> Parse(Request &req,
                                                    InputIterator begin,
                                                    InputIterator end) {
            while (begin <= end) {
                ResultType result = Consume(req, *begin++);
                if (result == good || result == bad)
                    return std::make_tuple(result, begin);
            }
            return std::make_tuple(indeterminate, begin);
        };

    private:
        /// Handle the next character of input.
        ResultType Consume(Request &req, unsigned char input);

        /// The current state of the parser.
        enum State {
            method_1,
            method_2,
            length_1,
            length_2,
            data,
            data_alternate,
        } state_;

        /// 暂存解析的字符结果
        char tmp_c_;
    };
}
#endif //KEYMANAGEMENT_REQUESTPARSER_H
