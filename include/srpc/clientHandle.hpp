/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_CLIENTHANDLE_HPP_
#define SRPC_CLIENTHANDLE_HPP_

#include <string>

#include <boost/asio.hpp>

#include "tcpClient.hpp"

namespace srpc {

using namespace boost;

class ClientHandle {
 public:
  explicit ClientHandle(const std::string& host,
                        const std::string& port) : m_client(m_ioContext, host, port) {}

  ~ClientHandle() = default;

  void run() {
    m_ioContext.run();
  }

 private:
  asio::io_service m_ioContext;
  // asio::io_context m_ioContext;
  TcpClient m_client;
};

}   // namespace srpc

#endif  // SRPC_CLIENTHANDLE_HPP_
