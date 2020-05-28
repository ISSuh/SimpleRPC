/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_TCPSERVER_HPP_
#define SRPC_TCPSERVER_HPP_

#include <memory>
#include <utility>

#include <boost/asio.hpp>

#include "server.hpp"
#include "session.hpp"

namespace srpc {

using namespace boost;

class TcpServer {
 public:
  explicit TcpServer(asio::io_service& ioContext, uint32_t port) :
      m_socket(ioContext),
      m_acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
  }
  ~TcpServer() = default;

  void requestCall() {}
  void responseCall() {}
  void run() {}
 private:


 private:
  asio::ip::tcp::socket m_socket;
  asio::ip::tcp::acceptor m_acceptor;
};

}   // namespace srpc

#endif  // SRPC_TCPSERVER_HPP_
