/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVER_HPP
#define SRPC_SERVER_HPP

#include <memory>
#include <boost/asio.hpp>

#include "session.hpp"

using boost::asio::ip::tcp;

class Server {
 public:
  Server(boost::asio::io_service& io_service, short port)
  : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), socket_(io_service) {
    do_accept();
  }

 private:
  void do_accept()
  {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
          std::make_shared<Session>(std::move(socket_))->start();
        }

        do_accept();
      }
    );
  }
 
 private:
  tcp::acceptor acceptor_;
  tcp::socket socket_;
};

#endif // SRPC_SERVER_HPP
