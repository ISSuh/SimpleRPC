/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SESSION_HPP
#define SRPC_SESSION_HPP

#include <iostream>
#include <memory>
#include <functional>
#include <boost/asio.hpp>

namespace sprc {

using namespace boost;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(asio::ip::tcp::socket socket) : m_socket(std::move(socket)) {}
  ~Session() = default;

  void start() {
    read();
    write("TEST");
  }

private:
  void read() {
    char data[100];

    m_socket.async_read_some(asio::buffer(data, 101),
                std::bind(&Session::readHandler, this, asio::placeholders::error, data));
  }
  void write(const std::string& test) {
    async_write(m_socket,
                asio::buffer(test.c_str(), test.size() + 1),
                std::bind(&Session::writeHandler, this, asio::placeholders::error));
  }

 private:
  void readHandler(const system::error_code& error, char* data) {
    if(!error) {
      std::cout << data << std::endl;
    }
  }

  void writeHandler(const system::error_code& error) {}

 private:
  asio::ip::tcp::socket m_socket;
};

} // srpc

#endif // SRPC_SESSION_HPP
