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

  void request(const std::string& msg) {
    m_session.write(msg);
  }

  void updateConnect() {
    std::cout << "---updateConnect---\n";
  }

  void updateRead() override {
    std::cout << "---updateRead---\n";
  }

  void updateWrite() override {
    std::cout << "---updateWrite---\n";
  }

 private:
  void contextRunner() {
    IoService::work worker(m_ioContext);
    m_ioContext.run();
  }

 private:
  IoService m_ioContext;
  std::thread m_ioContextRunner;
  ClientSession<TcpClient> m_session;

  TcpResolver m_resolver;
};

}   // namespace srpc

#endif  // SRPC_TCPCLIENT_HPP_
