/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVERSESSION_HPP_
#define SRPC_SERVERSESSION_HPP_

#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

#include "Session.hpp"

namespace srpc {

template<typename T>
class ServerSession : public Session {
 public:
  explicit ServerSession(boost::asio::io_service& ioContext, T& system)
      : Session(ioContext),
        m_system(system) {}

  ~ServerSession() = default;

  void connect(const boost::asio::ip::tcp::resolver::iterator& endpointIter) {
    std::cout << "---connectSession---\n";

    boost::asio::async_connect(Session::getSocket(), endpointIter,
                              std::bind(&ServerSession::connectHandler,
                                         this,
                                         std::placeholders::_1));
  }

  void read() {
    std::cout << "---read---\n";

    char msg[100];
    Session::getSocket().async_read_some(boost::asio::buffer(msg, 100),
                             std::bind(&ServerSession::readHandler,
                                        this,
                                        std::placeholders::_1, std::placeholders::_2, msg));
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    boost::asio::async_write(Session::getSocket(), boost::asio::buffer(test.c_str(), test.length()),
                             std::bind(&ServerSession::writeHandler,
                                        this,
                                        std::placeholders::_1));
  }

  void close() override {
    std::cout << "---close---\n";

    boost::system::error_code error;
    Session::getSocket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);

    if (!error) {
      std::cout << "Socket Shutdown Success! " << std::endl;
    } else {
      std::cout << "Socket Shutdown Fail! " << std::endl;
    }

    Session::getSocket().close();
  }

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

  void readHandler(const boost::system::error_code& error, size_t len, const char* data) override {
    std::cout << "---readHandler---\n";

    if (!error) {
      std::cout << "Read Success! : " << data << " / " << len << std::endl;
      m_system.updateRead();
    } else {
      m_system.unRegistMap(Session::getUUID());
      std::cout << "Read Fail! : " << error.message() << std::endl;
    }
  }

  void writeHandler(const boost::system::error_code& error) override {
    std::cout << "---writeHandler---\n";

    if (!error) {
      std::cout << "Write Success!" << std::endl;
      m_system.updateWrite();
    } else {
      m_system.unRegistMap(Session::getUUID());
      std::cout << "Write Fail! : " << error.message() << std::endl;
    }
  }

 private:
  T& m_system;
};

}  // namespace srpc

#endif  // SRPC_SERVERSESSION_HPP_
