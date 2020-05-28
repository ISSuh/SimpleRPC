/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVER_HANDLE_HPP
#define SRPC_SERVER_HANDLE_HPP

#include <memory>
#include <boost/asio.hpp>

#include "tcpServer.hpp"

namespace srpc {

using namespace boost;

class ServerHandle {
 public:
  explicit ServerHandle(int32_t port) : m_server(m_ioContext, port) {}

  ~ServerHandle() = default;

  void run() {
    m_ioContext.run();
  }

 private:
  asio::io_service m_ioContext;
  // asio::io_context m_ioContext;
  TcpServer m_server;
};

} // namespace sprc

#endif // SRPC_SERVER_HANDLE_HPP
