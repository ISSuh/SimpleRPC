/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_CLIENTSESSION_HPP_
#define SRPC_CLIENTSESSION_HPP_

#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

#include "Common.hpp"
#include "Session.hpp"

namespace srpc {

template<typename T>
class ClientSession : public Session {
 public:
  explicit ClientSession(IoService& ioContext, T& system)
      : Session(ioContext),
        m_system(system) {}

  ~ClientSession() = default;

  void connect(const TcpResolverIterator& endpointIter) {
    std::cout << "---connectSession---\n";

    asio::async_connect(Session::getSocket(), endpointIter,
                              std::bind(&ClientSession::connectHandler,
                                         this,
                                         std::placeholders::_1));
  }

  void read() {
    std::cout << "---read---\n";

    char msg[100];
    Session::getSocket().async_read_some(asio::buffer(msg, 100),
                             std::bind(&ClientSession::readHandler,
                                        this,
                                        std::placeholders::_1, std::placeholders::_2, msg));
  }

  void write(const std::string& test) {
    std::cout << "---write---\n";

    asio::async_write(Session::getSocket(), asio::buffer(test.c_str(), test.length()),
                             std::bind(&ClientSession::writeHandler,
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
      std::cout << "Read Success! : " << data << " / " << len << std::endl;
      m_system.updateRead();
    } else {
      std::cout << "Read Fail! : " << error.message() << std::endl;
    }
  }

  void writeHandler(const ErrorCode& error) override {
    std::cout << "---writeHandler---\n";

    if (!error) {
      std::cout << "Write Success!" << std::endl;
      m_system.updateWrite();
    } else {
      std::cout << "Write Fail! : " << error.message() << std::endl;
    }
  }

 private:
  T& m_system;
};

}  // namespace srpc

#endif  // SRPC_CLIENTSESSION_HPP_
