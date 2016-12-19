//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#include "Connection.h"
#include <utility>
#include <vector>

namespace tcp {

    Connection::Connection(boost::asio::io_service &io_service)
            : socket_(io_service) {
    }

    void Connection::Start(std::shared_ptr<ThreadTask> task) {
        request_handler_.BindThreadTask(task);
        DoRead();
    }

    void Connection::Reset() {
        request_.Reset();
        reply_.Reset();
    }

    void Connection::DoRead() {
        auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(buffer_),
                                [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                                    if (!ec) {
                                        RequestParser::ResultType result;
                                        Reset();
                                        std::tie(result, std::ignore) = request_parser_.Parse(
                                                request_, buffer_.data(), buffer_.data() + bytes_transferred);
                                        if (request_handler_.status_ == RequestHandler::authentication_1) {
                                            if (request_.method != "Authentication1") {
                                                result = RequestParser::bad;
                                            }
                                        } else if (request_handler_.status_ == RequestHandler::authentication_2) {
                                            if (request_.method != "Authentication2") {
                                                result = RequestParser::bad;
                                            }
                                        } else if (request_handler_.status_ == RequestHandler::key_handle) {
                                            if (request_.method != "CreateKey" &&
                                                request_.method != "FindKeyByID") {
                                                result = RequestParser::bad;
                                            }
                                        }
                                        if (result == RequestParser::good) {
                                            request_handler_.HandleRequest(request_, reply_);
                                        } else {
                                            request_handler_.ReplyError(reply_);
                                        }
                                        DoWrite();
                                    } else if (ec != boost::asio::error::operation_aborted) {
                                        LOG(ERROR) << "Connection closed unexpectedly";
                                    }
                                });
        // 如果有错误发生，那么不会执行更多的异步操作，这意味着该函数执行完毕后，
        // 控制该对象的智能指针将会自行销毁，该类的析构函数将会关闭socket
    }

    void Connection::DoWrite() {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, reply_.ToBuffers(),
                                 [this, self](boost::system::error_code ec, std::size_t len) {
                                     if (!ec) {
                                         if (request_handler_.status_ == RequestHandler::error ||
                                             request_handler_.status_ == RequestHandler::key_handle) {
                                             LOG(INFO) << "Connection closed successfully";
                                         } else {
                                             if (request_handler_.status_ == RequestHandler::authentication_1) {
                                                 request_handler_.status_ = RequestHandler::authentication_2;
                                             } else if (request_handler_.status_ == RequestHandler::authentication_2) {
                                                 request_handler_.status_ = RequestHandler::key_handle;
                                             }
                                             DoRead();
                                         }
                                     } else if (ec != boost::asio::error::operation_aborted) {
                                         LOG(ERROR) << "Connection closed unexpectedly";
                                     }
                                 });
        // 如果有错误发生，那么不会执行更多的异步操作，这意味着该函数执行完毕后，
        // 控制该对象的智能指针将会自行销毁，该类的析构函数将会关闭socket
    }

} // namespace tcp
