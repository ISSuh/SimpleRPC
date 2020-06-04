/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_CLIENTHANDLE_HPP_
#define SRPC_CLIENTHANDLE_HPP_

#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "TcpClient.hpp"

namespace srpc {

class ClientHandle {
 public:
  ClientHandle() : m_client(m_ioContext) {
    m_ioContextRunner = std::thread(&ClientHandle::contextRunner, this);
  }

  ~ClientHandle() {
    m_ioContextRunner.join();
  }

  void connect(const std::string& host, const std::string& port) {
    m_client.connect(host, port);
  }

  void terminate() {
    m_client.close();
  }

  void request(const std::string& msg) {
    m_client.request(msg);
  }

 private:
  void contextRunner() {
    m_ioContext.run();
  }

 private:
  boost::asio::io_service m_ioContext;
  TcpClient m_client;

  std::thread m_ioContextRunner;
};

}   // namespace srpc

#endif  // SRPC_CLIENTHANDLE_HPP_
