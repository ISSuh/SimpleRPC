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

class ClientHandle {
 public:
  explicit ClientHandle(const std::string& host,
                        const std::string& port) : m_client(m_ioContext, host, port) {}

  ~ClientHandle() = default;

  void connect() {
    m_client.connect();

    m_ioContext.run();
  }

  void terminate() {
    m_client.close();
  }

  void request(const std::string& msg) {
    m_client.request(msg);
  }

 private:
  boost::asio::io_service m_ioContext;
  // asio::io_context m_ioContext;
  TcpClient m_client;
};

}   // namespace srpc

#endif  // SRPC_CLIENTHANDLE_HPP_
