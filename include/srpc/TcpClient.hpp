/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_TCPCLIENT_HPP_
#define SRPC_TCPCLIENT_HPP_

#include <iostream>
#include <string>
#include <functional>
#include <thread>

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "Common.hpp"
#include "Client.hpp"
#include "ClientSession.hpp"

namespace srpc {

class TcpClient : public Client {
 public:
  TcpClient() : m_session(m_ioContext, *this), m_resolver(m_ioContext) {
    m_ioContextRunner = std::thread(&TcpClient::contextRunner, this);
  }

  ~TcpClient() {
    m_ioContextRunner.join();
  }

  void connect(const std::string& host, const std::string& port) {
    TcpResolverQuery clientQuery(host, port);
    TcpResolverIterator endpointIter = m_resolver.resolve(clientQuery);

    m_session.connect(endpointIter);
  }

  void close() {
    m_session.close();
  }

  void request(const std::string& serviceName,
               const std::string& rpcName,
               const std::string& paramTest) {
    if (m_session.isConnectd()) {
      Message msg;
      msg.setUuid(m_session.getUUID());
      msg.setCommand(Command::REQUEST);
      msg.setBody(serviceName, rpcName, paramTest);

      m_session.write(Command::REQUEST, msg.serialize());
    }
  }

  void onConnect() override {
    BOOST_LOG_TRIVIAL(info) << "onConnect";
  }

  void onRead(const std::string& serializedMessage) override {
    BOOST_LOG_TRIVIAL(info) << "onRead";
    Message msg(serializedMessage);

    msg.printPacketforDubugging();
  }

  void onWrite() override {
    BOOST_LOG_TRIVIAL(info) << "onWrite";
  }

 private:
  void contextRunner() {
    IoService::work worker(m_ioContext);
    m_ioContext.run();
  }

 private:
  IoService m_ioContext;
  std::thread m_ioContextRunner;

  TcpResolver m_resolver;
  ClientSession<TcpClient> m_session;
};

}   // namespace srpc

#endif  // SRPC_TCPCLIENT_HPP_
