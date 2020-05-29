/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_TCPSERVER_HPP_
#define SRPC_TCPSERVER_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <utility>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "server.hpp"
#include "session.hpp"

namespace srpc {

class TcpServer {
 public:
  explicit TcpServer(boost::asio::io_service& ioContext, uint32_t port)
      : m_ioContext(ioContext),
        m_acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

  ~TcpServer() = default;

  void accpet() {
    Session<TcpServer>* newSession = new Session<TcpServer>(m_ioContext, this);

    m_acceptor.async_accept(newSession->getSocket(),
                            std::bind(&TcpServer::acceptHandle, this,
                                      newSession,
                                      std::placeholders::_1));
  }
  void updateRead() {}
  void updateWrite() {}

 private:
  void acceptHandle(Session<TcpServer>* session, const boost::system::error_code& error) {
    if (!error) {
      boost::uuids::uuid newUUID = boost::uuids::random_generator()();
      std::cout << "Accept New Client - " << newUUID << " - " << m_sessionMap.size() << '\n';

      m_sessionMap[newUUID] = session;

      m_sessionMap[newUUID]->write(to_string(newUUID));
    } else {
      std::cout << "Accept Error!\n";
    }

    accpet();

  }

 private:
  boost::asio::io_service& m_ioContext;
  boost::asio::ip::tcp::acceptor m_acceptor;

  std::map<boost::uuids::uuid, Session<TcpServer>*> m_sessionMap;
};

}   // namespace srpc

#endif  // SRPC_TCPSERVER_HPP_
