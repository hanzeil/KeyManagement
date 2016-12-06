//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//

#include "Server.h"
#include <signal.h>
#include <utility>

namespace tcp {
    Server::Server(const std::string &address, const std::string &port,
                   size_t thread_pool_size)
            : io_service_(),
              signals_(io_service_),
              acceptor_(io_service_),
              connection_manager_(),
              thread_pool_size_(thread_pool_size) {
        // Register to handle the signals that indicate when the server should exit.
        // It is safe to register for the same signal multiple times in a program,
        // provided all registration for the specified signal is made through Asio.
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
#if defined(SIGQUIT)
        signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

        DoAwaitStop();

        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver(io_service_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        DoAccept();
    }

    void Server::Run() {
        // Create a pool of threads to run all of the thread_task
        std::vector<std::shared_ptr<boost::thread> > threads;
        for (std::size_t i = 0; i < thread_pool_size_; i++) {
            auto thread_task = std::make_shared<ThreadTask>(io_service_);
            std::shared_ptr<boost::thread> thread(new boost::thread(
                    boost::bind(&ThreadTask::Run,
                                thread_task)
            ));
            threads.push_back(thread);
            thread_tasks_.insert({thread->get_id(), thread_task});
        }

        // Wait for all threads in the pool to exit
        for (size_t i = 0; i < threads.size(); i++) {
            threads[i]->join();
        }
    }

    void Server::DoAccept() {
        new_connection_ = std::make_shared<Connection>(io_service_, connection_manager_);
        acceptor_.async_accept(new_connection_->socket_,
                               [this](boost::system::error_code ec) {
                                   // Check whether the server was stopped by a signal before this
                                   // completion handler had a chance to run.
                                   if (!acceptor_.is_open()) {
                                       return;
                                   }

                                   if (!ec) {
                                       auto thread_task = thread_tasks_.at(
                                               boost::this_thread::get_id());
                                       connection_manager_.Start(new_connection_, thread_task);
                                   }

                                   DoAccept();
                               });
    }

    void Server::DoAwaitStop() {
        signals_.async_wait(
                [this](boost::system::error_code /*ec*/, int /*signo*/) {
                    // The server is stopped by cancelling all outstanding asynchronous
                    // operations. Once all operations have finished the io_service::run()
                    // call will exit.
                    acceptor_.close();
                    connection_manager_.StopAll();
                });
    }

} // namespace tcp
