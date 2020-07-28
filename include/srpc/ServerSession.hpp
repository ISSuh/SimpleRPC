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
#include <atomic>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/log/trivial.hpp>

#include "Common.hpp"
#include "Message.hpp"
#include "Session.hpp"

namespace srpc {

template<typename T>
class ServerSession : public Session {
 public:
  explicit ServerSession(IoService& ioContext, T& system)
      : Session(ioContext),
        m_system(system),
        m_isConnected(false) {
      registReadHandler();
      registWriteHandler();
  }

  ~ServerSession() = default;

  void read(Command cmd) override {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Read - " << CommandToString(cmd);

    Session::getSocket().async_read_some(asio::buffer(m_readBuff, MAX_LEN),
                                         readHandler[cmd]);
  }

  void write(Command cmd, const std::string& serializedMessage) override {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write - " << CommandToString(cmd);

    Message msg(serializedMessage);

    msg.printPacketforDubugging();

    Session::getSocket().async_write_some(
                                asio::buffer(serializedMessage, serializedMessage.size()),
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

  char m_readBuff[MAX_LEN];

  std::atomic<bool> m_isConnected;
};

/**
 * Read Handler
 */
template<class T>
void ServerSession<T>::requestReadHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "requestReadHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << "Read - " << len;

    m_system.onRead(Session::getUUID(), std::string(m_readBuff));
    read(Command::REQUEST);
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Read Fail! - " << error.message();

    m_system.unRegistMap(Session::getUUID());
  }
}

template<class T>
void ServerSession<T>::responseReadHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "responseReadHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << "Read - " << len;

    m_system.onRead(Session::getUUID(), std::string(m_readBuff));
    read(Command::REQUEST);
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Read Fail! - " << error.message();

    m_system.unRegistMap(Session::getUUID());
  }
}

/**
 * Write Handler
 */
template<class T>
void ServerSession<T>::acceptWriteHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  ": acceptWriteHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write Success - " << len;

    m_isConnected.store(true);
    m_system.onWrite(Session::getUUID());
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Write Fail! - " << error.message();

    m_system.unRegistMap(Session::getUUID());
  }
}

template<class T>
void ServerSession<T>::requestWriteHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << Session::getUUID() <<  "requestWriteHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write Success - " << len;

    m_system.onWrite(Session::getUUID());
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Write Fail! - " << error.message();

    m_system.unRegistMap(Session::getUUID());
  }
}

template<class T>
void ServerSession<T>::reponseWriteHandler(const ErrorCode& error, size_t len) {
  BOOST_LOG_TRIVIAL(info) << "reponseWriteHandler";

  if (!error) {
    BOOST_LOG_TRIVIAL(info) << Session::getUUID() << ": Write Success - " << len;

    m_system.onWrite(Session::getUUID());
  } else {
    BOOST_LOG_TRIVIAL(error) << Session::getUUID() << ": Write Fail! - " << error.message();

    m_system.unRegistMap(Session::getUUID());
  }
}

}  // namespace srpc

#endif  // SRPC_SERVERSESSION_HPP_
