//
// Created by Hanzeil on 16-10-31.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#include "LoggerServer.h"

LoggerServer::LoggerServer(const std::string &address, const std::string &port)
        : io_service_(),
          signals_(io_service_),
          acceptor_(io_service_),
          deadline_timer_(io_service_) {

    try {
        Config config_settings;

        config_settings.ReadFile(config_settings.GetConifgPath(CONFIG_FILE_NAME));

        log_rotation_size_ = config_settings.Read<std::size_t>("LOG_ROTATION_SIZE");
        log_max_files_ = config_settings.Read<std::size_t>("LOG_MAX_FILES");
        log_scan_duration_ = config_settings.Read<std::size_t>("LOG_SCAN_DURATION");
    }
    catch (std::exception &e) {
        LOG(FATAL) << e.what() << "\n";
    }

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

    DoDeadlineTimer();

    DoAccept();
}

void LoggerServer::Run() {

    Logger::GetInstance().Init(USER_CONFIG_PATH);
    Logger::GetInstance().SetLogRotationSize(log_rotation_size_);
    Logger::GetInstance().SetLogMaxFiles(log_max_files_);

    std::shared_ptr<boost::thread> thread(new boost::thread(
            boost::bind(&boost::asio::io_service::run,
                        &io_service_)
    ));
}

void LoggerServer::DoDeadlineTimer() {
    deadline_timer_.expires_from_now(boost::posix_time::hours(log_scan_duration_));
    deadline_timer_.async_wait(
            [this](boost::system::error_code ec) {
                Logger::GetInstance().ScanRotation();
                DoDeadlineTimer();
            }
    );
}

void LoggerServer::DoAccept() {

    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_service_);

    acceptor_.async_accept(*socket,
                           [this, socket](boost::system::error_code ec) {
                               // Check whether the server was stopped by a signal before this
                               // completion handler had a chance to run.
                               if (!acceptor_.is_open()) {
                                   return;
                               }

                               if (!ec) {
                                   DoRead(socket);
                               }

                               DoAccept();
                           });
}

void LoggerServer::DoAwaitStop() {
    signals_.async_wait(
            [this](boost::system::error_code /*ec*/, int /*signo*/) {
                // The server is stopped by cancelling all outstanding asynchronous
                // operations. Once all operations have finished the io_service::run()
                // call will exit.
                acceptor_.close();
            });
}

void LoggerServer::DoRead(socket_ptr socket) {

    std::array<char, 10> buffer;

    socket->async_read_some(boost::asio::buffer(buffer),
                            [this, socket, &buffer](boost::system::error_code ec, std::size_t bytes_transferred) {
                                if (!ec && bytes_transferred == 1) {
                                    if (buffer[0] == '0') {
                                        Logger::GetInstance().SetFilter(0);
                                    } else if (buffer[0] == '1') {
                                        Logger::GetInstance().SetFilter(1);
                                    } else if (buffer[0] == '2') {
                                        Logger::GetInstance().SetFilter(2);
                                    } else if (buffer[0] == '3') {
                                        Logger::GetInstance().SetFilter(3);
                                    }
                                    socket->close();
                                } else if (ec != boost::asio::error::operation_aborted) {
                                    socket->close();
                                }
                            });
}

