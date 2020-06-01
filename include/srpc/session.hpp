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

  void connect(const boost::asio::ip::tcp::resolver::iterator& endpointIter) {
    std::cout << "---connectSession---\n";

    boost::asio::async_connect(m_socket, endpointIter,
                              std::bind(&Session::connectHandler,
                                         this,
                                         std::placeholders::_1));
  }

  void read() {
    std::cout << "---read---\n";

    char msg[100];
    m_socket.async_read_some(boost::asio::buffer(msg, 100),
                             std::bind(&Session::readHandler,
                                        this,
                                        std::placeholders::_1, std::placeholders::_2, msg));
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    boost::asio::async_write(m_socket, boost::asio::buffer(test.c_str(), test.length()),
                             std::bind(&Session::writeHandler,
                                        this,
                                        std::placeholders::_1));
  }

  void close() {
    std::cout << "---close---\n";

    boost::system::error_code error;
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);

    if (!error) {
      std::cout << "Socket Shutdown Success! " << std::endl;
    } else {
      std::cout << "Socket Shutdown Fail! " << std::endl;
    }

    m_socket.close();
  }

  boost::asio::ip::tcp::socket& getSocket() { return m_socket; }

 private:
  void connectHandler(const boost::system::error_code& error) {
    std::cout << "---connectHandle---\n";

    if (!error) {
      std::cout << "Connect Success! " << std::endl;

      read();
    } else {
      std::cout << "Connect Fail! : " << error.message() << std::endl;
    }
  }

  void readHandler(const boost::system::error_code& error, size_t len, const char* data) {
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
