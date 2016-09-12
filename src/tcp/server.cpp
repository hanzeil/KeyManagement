//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#include "server.h"
#include <signal.h>
#include <utility>

namespace http {
    namespace server {
        server::server(const std::string &address, const std::string &port,
                       const std::string &doc_root, size_t thread_pool_size)
                : io_service_(),
                  signals_(io_service_),
                  acceptor_(io_service_),
                  connection_manager_(),
                  socket_(io_service_),
                  request_handler_(),
                  thread_pool_size_(thread_pool_size) {
            // Register to handle the signals that indicate when the server should exit.
            // It is safe to register for the same signal multiple times in a program,
            // provided all registration for the specified signal is made through Asio.
            signals_.add(SIGINT);
            signals_.add(SIGTERM);
#if defined(SIGQUIT)
            signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

            do_await_stop();

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();

            do_accept();
        }

        void server::run() {
            // Create a pool of threads to run all of the io_service
            std::vector<std::shared_ptr<boost::thread> > threads;
            std::cout << thread_pool_size_ << std::endl;
            for (size_t i = 0; i < thread_pool_size_; i++) {
                std::shared_ptr<boost::thread> thread(new boost::thread(
                        boost::bind(&boost::asio::io_service::run,
                                    &io_service_)
                ));
                threads.push_back(thread);
            }

            // Wait for all threads in the pool to exit
            for (size_t i = 0; i < threads.size(); i++) {
                threads[i]->join();
            }
        }

        void server::do_accept() {
            new_connection_.reset();
            acceptor_.async_accept(socket_,
                                   [this](boost::system::error_code ec) {
                                       // Check whether the server was stopped by a signal before this
                                       // completion handler had a chance to run.
                                       if (!acceptor_.is_open()) {
                                           return;
                                       }

                                       if (!ec) {
                                           std::cout << getpid() << std::endl;
                                           sleep(5);
                                           connection_manager_.start(std::make_shared<connection>(
                                                   std::move(socket_), connection_manager_, request_handler_));
                                       }

                                       do_accept();
                                   });
        }

        void server::do_await_stop() {
            signals_.async_wait(
                    [this](boost::system::error_code /*ec*/, int /*signo*/) {
                        // The server is stopped by cancelling all outstanding asynchronous
                        // operations. Once all operations have finished the io_service::run()
                        // call will exit.
                        acceptor_.close();
                        connection_manager_.stop_all();
                    });
        }

    } // namespace server
} // namespace http
