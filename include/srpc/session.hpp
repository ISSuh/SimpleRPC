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

template<typename T>
class Session : public std::enable_shared_from_this<Session<T>> {
 public:
  explicit Session(boost::asio::io_service& ioContext, T* system)
      : m_socket(ioContext),
        m_system(system) {}
  ~Session() = default;

  void connectSession() {
    std::cout << "---connectSession---\n";
  }

  void read() {
    std::cout << "---read---\n";

    char msg[100];
    m_socket.async_read_some(boost::asio::buffer(msg, 100),
                             std::bind(&Session::readHandler,
                                      this,
                                      std::placeholders::_1, std::placeholders::_2));
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    boost::asio::async_write(m_socket, boost::asio::buffer(test.c_str(), test.length()),
                             std::bind(&Session::writeHandler,
                                       this,
                                       std::placeholders::_1));
  }

  boost::asio::ip::tcp::socket& getSocket() { return m_socket; }

 private:
    void readHandler(const boost::system::error_code& error, size_t len, const std::string& data) {
    std::cout << "---readHandler---\n";

    if (!error) {
      std::cout << "Read Success! : " << data << " / " << len << std::endl;
      m_system->updateRead();
    } else {
      std::cout << "Read Fail! : " << error.message() << std::endl;
    }
  }

  void writeHandler(const boost::system::error_code& error) {
    std::cout << "---writeHandler---\n";

    if (!error) {
      std::cout << "Write Success!" << std::endl;
      m_system->updateWrite();
    } else {
      std::cout << "Write Fail! : " << error.message() << std::endl;
    }
  }

 private:
  boost::asio::ip::tcp::socket m_socket;
  T* m_system;
};

}  // namespace srpc

#endif  // SRPC_SESSION_HPP_
