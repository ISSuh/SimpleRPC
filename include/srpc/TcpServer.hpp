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
#include <boost/log/trivial.hpp>

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

    std::thread t([&](){
      IoService::work worker(m_ioContext);
      m_ioContext.run();
    });

    // while (true) {
    //   for (const auto& iter : m_sessionMap) {
    //     iter.second->read(Command::REQUEST);
    //   }

    //   usleep(100000);
    // }

    if (t.joinable()) {
      t.join();
    }
  }

  void onRead(const std::string& uuid, const std::string& serializedMessage) override {
    BOOST_LOG_TRIVIAL(info) << "onRead - " <<  uuid;
  }

  void onWrite(const std::string& uuid) override {
    BOOST_LOG_TRIVIAL(info) << "onWrite - " <<  uuid;
  }

  void unRegistMap(const std::string& uuid) {
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
      std::string strUUID = to_string(newUUID);

      m_sessionMap.insert(std::make_pair(strUUID, session));
      session->setUUID(strUUID);

      BOOST_LOG_TRIVIAL(info) << "Accept New Client - " << strUUID << " / " << m_sessionMap.size();

      Message msg(strUUID);
      msg.setCommand(Command::ACCEPT);

      session->write(Command::ACCEPT, msg);
    } else {
      BOOST_LOG_TRIVIAL(error) << "Accept Error! - " << error.message();
      delete session;
    }

    accpet();
  }

 private:
  IoService m_ioContext;

  TcpAcceptor* m_acceptor = nullptr;
  std::map<std::string, std::unique_ptr<ServerSession<TcpServer>>> m_sessionMap;
};

}   // namespace srpc

#endif  // SRPC_TCPSERVER_HPP_
