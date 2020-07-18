/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVERSESSION_HPP_
#define SRPC_SERVERSESSION_HPP_

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Common.hpp"
#include "Message.hpp"
#include "Session.hpp"

namespace srpc {

template<typename T>
class ServerSession : public Session {
 public:
  explicit ServerSession(IoService& ioContext, T& system)
      : Session(ioContext),
        m_system(system) {
      registReadHandler();
      registWriteHandler();
  }

  ~ServerSession() = default;

  void read(Command cmd) override {
    std::cout << "[" << to_string(getUUID()) << "] ---read---\n";

    Session::getSocket().async_read_some(asio::buffer(m_readBuff, MAX_LEN),
                                         readHandler[cmd]);
  }

  void write(Command cmd, const std::string& serialized) override {
    std::cout << "---write---\n";

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
      {Command::REQUEST, std::bind(&ServerSession::requestReadHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REPONSE, std::bind(&ServerSession::responseReadHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)}
    };
  }

  void registWriteHandler() {
    writeHandler = {
      {Command::ACCEPT, std::bind(&ServerSession::acceptWriteHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REQUEST, std::bind(&ServerSession::requestWriteHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)},
      {Command::REPONSE, std::bind(&ServerSession::reponseWriteHandler, this,
                                   std::placeholders::_1, std::placeholders::_2)}
    };
  }

  // Read Handler
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
 * Read Handler
 */
template<class T>
void ServerSession<T>::requestReadHandler(const ErrorCode& error, size_t len) {
  std::cout << "---requesHandler---\n";

  if (!error) {
    std::cout << "Read Success! : " << m_readBuff << " / " << len << " - " << Session::getUUID() << std::endl;
    m_system.updateRead(Session::getUUID());
    read(Command::REQUEST);
  } else {
    m_system.unRegistMap(Session::getUUID());
    std::cout << "Read Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ServerSession<T>::responseReadHandler(const ErrorCode& error, size_t len) {
  std::cout << "---requesHandler---\n";

  if (!error) {
    std::cout << "Read Success! : " << m_readBuff << " / " << len << " - " << Session::getUUID() << std::endl;
    m_system.updateRead(Session::getUUID());
    read(Command::REQUEST);
  } else {
    m_system.unRegistMap(Session::getUUID());
    std::cout << "Read Fail! : " << error.message() << std::endl;
  }
}

/**
 * Write Handler
 */
template<class T>
void ServerSession<T>::acceptWriteHandler(const ErrorCode& error, size_t len) {
  std::cout << "---acceptHandler---\n";

  if (!error) {
    std::cout << "Write Success!" << std::endl;
    m_system.updateWrite(Session::getUUID());
  } else {
    m_system.unRegistMap(Session::getUUID());
    std::cout << "Write Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ServerSession<T>::requestWriteHandler(const ErrorCode& error, size_t len) {
  std::cout << "---reponseHandler---\n";

  if (!error) {
    std::cout << "Write Success!" << std::endl;
    m_system.updateWrite(Session::getUUID());
  } else {
    m_system.unRegistMap(Session::getUUID());
    std::cout << "Write Fail! : " << error.message() << std::endl;
  }
}

template<class T>
void ServerSession<T>::reponseWriteHandler(const ErrorCode& error, size_t len) {
  std::cout << "---reponseHandler---\n";

  if (!error) {
    std::cout << "Write Success!" << std::endl;
    m_system.updateWrite(Session::getUUID());
  } else {
    m_system.unRegistMap(Session::getUUID());
    std::cout << "Write Fail! : " << error.message() << std::endl;
  }
}

}  // namespace srpc

#endif  // SRPC_SERVERSESSION_HPP_
