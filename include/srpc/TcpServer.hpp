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

#include "Server.hpp"
#include "ServerSession.hpp"

namespace srpc {

class TcpServer {
  using UniqueSessionPtr = std::unique_ptr<ServerSession<TcpServer>>;

 public:
  explicit TcpServer(boost::asio::io_service& ioContext, const uint32_t port)
      : m_ioContext(ioContext),
        m_acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

  ~TcpServer() {}

  void accpet() {
    std::unique_ptr<ServerSession<TcpServer>> newSession =
                  std::unique_ptr<ServerSession<TcpServer>>(new ServerSession<TcpServer>(m_ioContext, *this));

    m_acceptor.async_accept(newSession->getSocket(),
                            std::bind(&TcpServer::acceptHandle, this,
                                      std::move(newSession),
                                      std::placeholders::_1));
  }

  void run() {
    accpet();

    std::thread t([&](){ m_ioContext.run(); });

    while (true) {
      for (const auto& iter : m_sessionMap) {
        iter.second->read();
      }

      sleep(1);
    }

    if (t.joinable()) {
      t.join();
    }
  }

  void updateRead() {}
  void updateWrite() {}
  void unRegistMap(const boost::uuids::uuid& uuid) {
    m_sessionMap[uuid].release();

    m_sessionMap.erase(uuid);
  }

 private:
  void acceptHandle(ServerSession<TcpServer>&& session, const boost::system::error_code& error) {
    if (!error) {
      boost::uuids::uuid newUUID = boost::uuids::random_generator()();
      std::cout << "Accept New Client - " << newUUID << " - " << m_sessionMap.size() << '\n';

      m_sessionMap[newUUID] = std::move(session);
      m_sessionMap[newUUID]->setUUID(newUUID);

      m_sessionMap[newUUID]->write(to_string(newUUID));
    } else {
      std::cout << "Accept Error!\n";
    }

    accpet();
  }

 private:
  boost::asio::io_service& m_ioContext;
  boost::asio::ip::tcp::acceptor m_acceptor;

  std::map<boost::uuids::uuid, std::unique_ptr<ServerSession<TcpServer>>> m_sessionMap;

  std::atomic_bool m_running;
};

}   // namespace srpc

#endif  // SRPC_TCPSERVER_HPP_
