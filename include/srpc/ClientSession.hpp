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
#include <atomic>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/log/trivial.hpp>

#include "Common.hpp"
#include "Message.hpp"
#include "Session.hpp"

namespace srpc {

template<typename T>
class ClientSession : public Session {
 public:
  explicit ClientSession(IoService& ioContext, T& system)
      : Session(ioContext),
        m_system(system),
        m_isConnected(false) {
    registReadHandler();
    registWriteHandler();
  }

  ~ClientSession() = default;

  void connect(const TcpResolverIterator& endpointIter) override {
    BOOST_LOG_TRIVIAL(info) << "connectSession";

    asio::async_connect(Session::getSocket(), endpointIter,
                              std::bind(&ClientSession::connectHandler,
                                         this,
                                         std::placeholders::_1));
  }

  void read(Command cmd) override {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Read - " << CommandToString(cmd);

    Session::getSocket().async_read_some(asio::buffer(&m_readBuff[0], MAX_LEN),
                                         readHandler[cmd]);
  }

  void write(Command cmd, const std::string& serializedMessage) override {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write - " << CommandToString(cmd);

    Session::getSocket().async_write_some(asio::buffer(serializedMessage, serializedMessage.size()),
                                          writeHandler[cmd]);
  }

  void close() override {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Close";

    ErrorCode error;
    Session::getSocket().shutdown(TcpSocket::shutdown_both, error);

    if (!error) {
      BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Session Shutdown Success!";
    } else {
      BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Session Shutdown Fail!";
    }

    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Socket Close";
    Session::getSocket().close();
  }

  bool isConnectd() const { return m_isConnected.load(); }

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

  char m_readBuff[MAX_LEN];

  std::atomic<bool> m_isConnected;
};

/**
 * Connect Handler
 */
template<class T>
void ClientSession<T>::connectHandler(const ErrorCode& error) {
  BOOST_LOG_TRIVIAL(info) << "connectHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << "Connect Success!";

    read(Command::ACCEPT);
    m_system.onConnect();
  } else {
    BOOST_LOG_TRIVIAL(error) << "Connect Fail! : " << error.message();
  }
}

/**
 * Read Handler
 */
template<class T>
void ClientSession<T>::acceptReadHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << "acceptReadHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << "Read - " << len;

    // m_isConnected.store(true);
    std::string str(m_readBuff, m_readBuff + len);
    m_system.onRead(str);
  } else {
    BOOST_LOG_TRIVIAL(error) << "Read Fail! - " << error.message();
  }
}

template<class T>
void ClientSession<T>::requestReadHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "requestReadHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << "Read - " << len;

    m_system.onRead(std::string(m_readBuff, m_readBuff + len));
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Read Fail! - " << error.message();
  }
}

template<class T>
void ClientSession<T>::responseReadHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "responseReadHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << "Read - " << len;

    m_system.onRead(std::string(m_readBuff, m_readBuff + len));
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Read Fail! - " << error.message();
  }
}

/**
 * Read Handler
 */
template<class T>
void ClientSession<T>::acceptWriteHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "acceptWriteHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write Success - " << len;

    m_system.onWrite();
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Write Fail! - " << error.message();
  }
}

template<class T>
void ClientSession<T>::requestWriteHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "requestWriteHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write Success - " << len;

    m_system.onWrite();
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Write Fail! - " << error.message();
  }
}

template<class T>
void ClientSession<T>::reponseWriteHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << "reponseWriteHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write Success - " << len;

    m_system.onWrite();
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Write Fail! - " << error.message();
  }
}

}  // namespace srpc

#endif  // SRPC_CLIENTSESSION_HPP_
