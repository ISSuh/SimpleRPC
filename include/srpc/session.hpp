/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SESSION_HPP_
#define SRPC_SESSION_HPP_

#include <iostream>
#include <string>
#include <utility>
#include <functional>

#include <boost/asio.hpp>

namespace srpc {

using namespace boost;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(asio::ip::tcp::socket socket) : m_socket(std::move(socket)) {}
  ~Session() = default;

  void connectSession() {
    std::cout << "---connectSession---\n";
    read();
    write("WelCome!");
  }

  void read() {
    std::cout << "---read---\n";

    char msg[100];
    m_socket.async_read_some(asio::buffer(msg, 100),
          std::bind(&Session::readHandler, shared_from_this(),
                    std::placeholders::_1, std::placeholders::_2, msg));
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    asio::async_write(m_socket, asio::buffer(test.c_str(), test.length()),
          std::bind(&Session::writeHandler, shared_from_this(), std::placeholders::_1));
  }

 private:
  void readHandler(const system::error_code& error, size_t len, const std::string& data) {
    std::cout << "---readHandler---\n";

    if (!error) {
      std::cout << "Read Success! : " << data << " / " << len << std::endl;
    } else {
      std::cout << "Read Fail! : " << error.message() << std::endl;
    }
  }

  void writeHandler(const system::error_code& error) {
    std::cout << "---writeHandler---\n";

    if (!error) {
      std::cout << "Write Success!" << std::endl;
    } else {
      std::cout << "Write Fail! : " << error.message() << std::endl;
    }
  }

 private:
  asio::ip::tcp::socket m_socket;
};

}  // namespace srpc

#endif  // SRPC_SESSION_HPP_
