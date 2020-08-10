/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_NET_CLIENT_TCPCLIENT_HPP_
#define SRPC_NET_CLIENT_TCPCLIENT_HPP_

#include <iostream>
#include <string>
#include <functional>
#include <thread>

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "Client.hpp"
#include "../../helper/Common.hpp"
#include "../session/ClientSession.hpp"

namespace srpc {

class TcpClient : public Client {
 public:
  TcpClient() : m_resolver(m_ioContext), m_session(m_ioContext, *this) {
    m_ioContextRunner = std::thread(&TcpClient::contextRunner, this);
  }

  ~TcpClient() {
    m_ioContextRunner.join();
  }

  void connect(const std::string& host, const std::string& port) override {
    TcpResolverQuery clientQuery(host, port);
    TcpResolverIterator endpointIter = m_resolver.resolve(clientQuery);

    m_session.connect(endpointIter);
  }

  void close() override {
    m_session.close();
  }

  void request(const std::string& serviceName,
               const std::string& rpcName,
               const std::string& paramTest) override {
    if (m_session.isConnectd()) {
      Message msg(m_session.getUUID(), Command::REQUEST);
      msg.setResquest(serviceName, rpcName, paramTest);

      m_session.write(Command::REQUEST, msg.serialize());
    }
  }

  void onConnect() override {
    BOOST_LOG_TRIVIAL(info) << "onConnect";
  }

  void onRead(std::string serializedMessage) override {
    BOOST_LOG_TRIVIAL(info) << "onRead";

    Message msg(serializedMessage);

    switch (msg.getCommand()) {
    case Command::ACCEPT :
      m_session.setUUID(msg.getUuid());
      msg.printPacketHeader();
      break;
    case Command::REPONSE :
      msg.printReponsePacketHeader();

      int a;
      msg.getResponse(a);
      std::cout << a << std::endl;
      break;
    default:
      break;
    }
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

#endif  // SRPC_NET_CLIENT_TCPCLIENT_HPP_
