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

namespace http {
    namespace server {

        struct Request;

/// Parser for incoming requests.
        class RequestParser {
        public:
            /// Construct ready to parse the request method.
            RequestParser();

            /// Reset to initial parser state.
            void reset();

            /// Result of parse.
            enum result_type {
                good, bad, indeterminate
            };

            /// Parse some data. The enum return value is good when a complete request has
            /// been parsed, bad if the data is invalid, indeterminate when more data is
            /// required. The InputIterator return value indicates how much of the input
            /// has been consumed.
            template<typename InputIterator>
            std::tuple<result_type, InputIterator> parse(Request &req,
                                                         InputIterator begin,
                                                         InputIterator end) {
                while (begin != end) {
                    result_type result = consume(req, *begin++);
                    if (result == good || result == bad)
                        return std::make_tuple(result, begin);
                }
                return std::make_tuple(indeterminate, begin);
            };

        private:
            /// Handle the next character of input.
            result_type consume(Request &req, char input);

            /// Check if a byte is an protocol character.
            static bool is_char(int c);

            /// The current state of the parser.
            enum state {
                method,
                length_1,
                length_2,
                data,
                expecting_newline_1,
                expecting_newline_2
            } state_;

            /// 暂存解析的字符结果
            char tmp_c;
        };
    }
}
#endif //KEYMANAGEMENT_REQUESTPARSER_H
