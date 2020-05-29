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

#include "server.hpp"
#include "session.hpp"

namespace srpc {

class TcpServer : public std::enable_shared_from_this<TcpServer> {
 public:
  explicit TcpServer(boost::asio::io_service* ioContext, uint32_t port)
      : m_ioContext(ioContext),
        m_acceptor(*ioContext, boost::asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

  ~TcpServer() = default;

  void accpet() {
    Session<TcpServer>* newSession = new Session<TcpServer>(m_ioContext, shared_from_this());

    m_acceptor.async_accept(newSession->getSocket(),
                            std::bind(&TcpServer::acceptHandle, this, newSession,
                                      boost::asio::placeholders::error));
  }
  void updateRead() {}
  void updateWrite() {}

 private:
  void acceptHandle(Session<TcpServer>* session, const boost::system::error_code& error) {
    if (!error) {
      boost::uuids::uuid newUUID = boost::uuids::random_generator()();
      std::cout << "Accept New Client - " << newUUID <<'\n';

      m_sessionMap[newUUID] = session;

      accpet();
    } else {
      std::cout << "Accept Error!\n";
    }
  }

 private:
  const boost::asio::io_service* m_ioContext;
  asio::ip::tcp::acceptor m_acceptor;

  std::map<boost::uuids::uuid, Session<TcpServer>*> m_sessionMap;
};

}   // namespace srpc

#endif  // SRPC_TCPSERVER_HPP_
