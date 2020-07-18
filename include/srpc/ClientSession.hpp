/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_CLIENTSESSION_HPP_
#define SRPC_CLIENTSESSION_HPP_

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

#include "Common.hpp"
#include "Message.hpp"
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

  void read(Command cmd) override {
    std::cout << "---read---\n";

    Session::getSocket().async_read_some(asio::buffer(m_readBuff, MAX_LEN),
                                         readHandler[cmd]);
  }

  void write(Command serialized, const std::string& serialized) override {
    std::cout << "---write--- " << serialized << std::endl;

    Session::getSocket().async_write_some(asio::buffer(serialized, serialized.length()),
                                          writeHandler[cmd]);
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
  void registReadHandler() {
    readHandler = {
      {Command::ACCEPT, std::bind(&ClientSession::acceptReadHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REQUEST, std::bind(&ClientSession::requestReadHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REPONSE, std::bind(&ClientSession::responseReadHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)}
    };
  }

  void registWriteHandler() {
    writeHandler = {
      {Command::ACCEPT, std::bind(&ClientSession::acceptWriteHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REQUEST, std::bind(&ClientSession::requestWriteHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REPONSE, std::bind(&ClientSession::reponseWriteHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)}
    };
  }

  // Connect Handler
  void connectHandler(const ErrorCode& error);

  // Read Handler
  void acceptReadHandler(const ErrorCode& error, size_t len);
  void requestReadHandler(const ErrorCode& error, size_t len);
  void responseReadHandler(const ErrorCode& error, size_t len);

  // Write Handler
  void acceptWriteHandler(const ErrorCode& error, size_t len);
  void requestWriteHandler(const ErrorCode& error, size_t len);
  void reponseWriteHandler(const ErrorCode& error, size_t len);

 private:
  T& m_system;
  std::map<Command, std::function<void(const ErrorCode&, size_t)>> readHandler;
  std::map<Command, std::function<void(const ErrorCode&, size_t)>> writeHandler;

  Message m_msg;
  char m_readBuff[MAX_LEN];
};

/**
 * Connect Handler
 */
template<class T>
void ClientSession<T>::connectHandler(const ErrorCode& error) {
  std::cout << "---connectHandle---\n";

  if (!error) {
    std::cout << "Connect Success! " << std::endl;

    read(Command::ACCEPT);
  } else {
    std::cout << "Connect Fail! : " << error.message() << std::endl;
  }
}

/**
 * Read Handler
 */
template<class T>
void ClientSession<T>::acceptReadHandler(const ErrorCode& error, size_t len) {
  std::cout << "---acceptHandler---\n";

  if (!error) {
    std::cout << "Read Success! : " << m_readBuff << " / " << len << std::endl;
    read(Command::ACCEPT);
  } else {
    std::cout << "Read Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ClientSession<T>::requestReadHandler(const ErrorCode& error, size_t len) {
  std::cout << "---requestHandler---\n";

  if (!error) {
    std::cout << "Read Success! : " << m_readBuff << " / " << len << std::endl;
    m_system.updateRead();
  } else {
    std::cout << "Read Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ClientSession<T>::responseReadHandler(const ErrorCode& error, size_t len) {
  std::cout << "---requestHandler---\n";

  if (!error) {
    std::cout << "Read Success! : " << m_readBuff << " / " << len << std::endl;
    m_system.updateRead();
  } else {
    std::cout << "Read Fail! : " << error.message() << std::endl;
  }
}

/**
 * Read Handler
 */
template<class T>
void ClientSession<T>::acceptWriteHandler(const ErrorCode& error, size_t len) {
  std::cout << "---acceptWriteHandler---\n";

  if (!error) {
    std::cout << "Write Success!" << std::endl;
    m_system.updateWrite();
  } else {
    std::cout << "Write Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ClientSession<T>::requestWriteHandler(const ErrorCode& error, size_t len) {
  std::cout << "---acceptWriteHandler---\n";

  if (!error) {
    std::cout << "Write Success!" << std::endl;
    m_system.updateWrite();
  } else {
    std::cout << "Write Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ClientSession<T>::reponseWriteHandler(const ErrorCode& error, size_t len) {
  std::cout << "---acceptWriteHandler---\n";

  if (!error) {
    std::cout << "Write Success!" << std::endl;
    m_system.updateWrite();
  } else {
    std::cout << "Write Fail! : " << error.message() << std::endl;
  }
}

}  // namespace srpc

#endif  // SRPC_CLIENTSESSION_HPP_
