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
#include <memory>

#include <boost/asio.hpp>

namespace srpc {

using namespace boost;

template<typename T>
class Session : public std::enable_shared_from_this<Session<T>> {
 public:
  explicit Session(const asio::ip::tcp::socket& socket)
      : m_socket(socket) {}
  ~Session() = default;

  void registAsyncAcceptHandler() {}
  void registAsyncConnectHandler() {}
  void registAsyncReadHandler(std::function<void(const system::error_code& error,
                                                 size_t len)> readHandler,
                              T* system) {
    m_readHandler = std::bind(readHandler,
                    system,
                    std::placeholders::_1, std::placeholders::_2);
  }
  void registAsyncWriteHandler(std::function<void(const system::error_code& error)> writeHandler) {
    m_writeHandler = std::bind(writeHandler,
                    system,
                    std::placeholders::_1);
  }

  void connectSession() {
    std::cout << "---connectSession---\n";
  }

  void read() {
    std::cout << "---read---\n";

    char msg[100];
    m_socket.async_read_some(asio::buffer(msg, 100), m_readHandler);
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    asio::async_write(m_socket, asio::buffer(test.c_str(), test.length()), m_writeHandler);
  }

//  private:
//   void readHandler(const system::error_code& error, size_t len, const std::string& data) {
//     std::cout << "---readHandler---\n";

//     if (!error) {
//       std::cout << "Read Success! : " << data << " / " << len << std::endl;
//     } else {
//       std::cout << "Read Fail! : " << error.message() << std::endl;
//     }
//   }

//   void writeHandler(const system::error_code& error) {
//     std::cout << "---writeHandler---\n";

//     if (!error) {
//       std::cout << "Write Success!" << std::endl;
//     } else {
//       std::cout << "Write Fail! : " << error.message() << std::endl;
//     }
//   }

 private:
  const asio::ip::tcp::socket& m_socket;
  std::function<void(const system::error_code& error, size_t len, const std::string& data)> m_readHandler;
  std::function<void(const system::error_code& error)> m_writeHandler;
};

}  // namespace srpc

#endif  // SRPC_SESSION_HPP_
