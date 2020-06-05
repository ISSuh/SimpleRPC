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

class TcpServer : public Server {
 public:
  TcpServer() {}
  ~TcpServer() {}

  void configure(const uint32_t port) {
    m_acceptor = new TcpAcceptor(m_ioContext, TcpEndPoint(asio::ip::tcp::v4(), port));
  }

  void run() override {
    accpet();

    std::thread t([&](){ m_ioContext.run(); });

    while (true) {
      for (const auto& iter : m_sessionMap) {
        iter.second->read();
      }

      usleep(100000);
    }

    if (t.joinable()) {
      t.join();
    }
  }

  void updateRead() override {}
  void updateWrite() override {}

  void unRegistMap(const Uuid& uuid) {
    m_sessionMap[uuid].release();
    m_sessionMap.erase(uuid);
  }

 private:
  void accpet() override {
    ServerSession<TcpServer>* newSession = new ServerSession<TcpServer>(m_ioContext, *this);

    m_acceptor->async_accept(newSession->getSocket(),
                             std::bind(&TcpServer::acceptHandle, this,
                                      std::placeholders::_1,
                                      newSession));
  }

  void acceptHandle(const ErrorCode& error, ServerSession<TcpServer>* session) {
    if (!error) {
      Uuid newUUID = boost::uuids::random_generator()();
      std::cout << "Accept New Client - " << newUUID << " - " << m_sessionMap.size() << '\n';

      m_sessionMap.insert(std::make_pair(newUUID, session));
      std::cout << m_sessionMap[newUUID].get() << '\n';

      m_sessionMap[newUUID]->setUUID(newUUID);
      m_sessionMap[newUUID]->write(to_string(newUUID));
    } else {
      std::cout << "Accept Error!\n";
    }

    accpet();
  }

 private:
  IoService m_ioContext;
  TcpAcceptor* m_acceptor = nullptr;

  std::map<Uuid, std::unique_ptr<ServerSession<TcpServer>>> m_sessionMap;

  std::atomic_bool m_running;
};

}   // namespace srpc

#endif  // SRPC_TCPSERVER_HPP_
