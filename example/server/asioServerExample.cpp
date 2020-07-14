/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>
#include <functional>

#include <boost/bind.hpp>
#include <boost/asio.hpp>


using boost::asio::ip::tcp;
using namespace std;

class session {
 public:
  session(boost::asio::io_service& io_service): socket_(io_service) {}

  tcp::socket& socket() { return socket_; }

  void start() {
    cout << "connected" << endl;

    char data_[1024];
    socket_.async_read_some(boost::asio::buffer(data_, 1024),
        std::bind(&session::handle_read, this, std::placeholders::_1, std::placeholders::_2));
  }

 private:
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
      cout << data_ << endl;
    } else {
      delete this;
    }
  }

 private:
  tcp::socket socket_;
  char data_[1024];
};

class server {
 public:
  server(boost::asio::io_service& io_service, short port)
      : io_service_(io_service),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
  }

 private:
  void start_accept() {
    session* new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(session* new_session, const boost::system::error_code& error) {
    if (!error) {
      new_session->start();
    } else {
      delete new_session;
    }

    start_accept();
  }

 private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
  std::cout << "ASIO Server Example\n";

  boost::asio::io_service io_service;

  server s(io_service, 33669);
  io_service.run();
}
