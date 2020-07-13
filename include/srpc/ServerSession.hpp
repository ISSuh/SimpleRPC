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
#include <boost/uuid/uuid_io.hpp>

#include "Common.hpp"
#include "Session.hpp"

namespace srpc {

template<typename T>
class ServerSession : public Session {
 public:
  explicit ServerSession(IoService& ioContext, T& system)
      : Session(ioContext),
        m_system(system) {}

  ~ServerSession() = default;

  void connect(const TcpResolverIterator& endpointIter) {
    std::cout << "---connectSession---\n";

    asio::async_connect(Session::getSocket(), endpointIter,
                              std::bind(&ServerSession::connectHandler,
                                         this,
                                         std::placeholders::_1));
  }

  void read() {
    std::cout << "[" << to_string(getUUID()) << "] ---read---\n";

    char msg[100];
    asio::async_read(Session::getSocket(), asio::buffer(msg, 100),
                             std::bind(&ServerSession::readHandler,
                                        this,
                                        std::placeholders::_1, std::placeholders::_2, msg));
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    asio::async_write(Session::getSocket(), asio::buffer(test.c_str(), test.size()),
                             std::bind(&ServerSession::writeHandler,
                                        this,
                                        std::placeholders::_1));
  }

  void close() override {
    std::cout << "---close---\n";

    ErrorCode error;
    Session::getSocket().shutdown(TcpSocket::shutdown_both, error);

    if (!error) {
      std::cout << "Socket Shutdown Success! " << std::endl;
    } else {
      std::cout << "Socket Shutdown Fail! " << std::endl;
    }

    Session::getSocket().close();
  }

 private:
  void connectHandler(const ErrorCode& error) {
    std::cout << "---connectHandle---\n";

    if (!error) {
      std::cout << "Connect Success! " << std::endl;

      read();
    } else {
      std::cout << "Connect Fail! : " << error.message() << std::endl;
    }
  }

  void readHandler(const ErrorCode& error, size_t len, const char* data) override {
    std::cout << "---readHandler---\n";

    if (!error) {
      std::cout << "Read Success! : " << data << " / " << len << " - " << Session::getUUID() << std::endl;
      m_system.updateRead(Session::getUUID());
    } else {
      m_system.unRegistMap(Session::getUUID());
      std::cout << "Read Fail! : " << error.message() << std::endl;
    }
  }

  void writeHandler(const ErrorCode& error) override {
    std::cout << "---writeHandler---\n";

    if (!error) {
      std::cout << "Write Success!" << std::endl;
      m_system.updateWrite(Session::getUUID());
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
