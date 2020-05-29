/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVERHANDLE_HPP_
#define SRPC_SERVERHANDLE_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <boost/asio.hpp>

#include "tcpServer.hpp"

namespace srpc {

class ServerHandle {
 public:
  explicit ServerHandle(uint32_t port) : m_server(m_ioContext, port) {}
  ~ServerHandle() = default;

  void run() {
    m_server.accpet();

    m_ioContext.run();
  }

 private:
  boost::asio::io_service m_ioContext;
  TcpServer m_server;
  std::atomic_bool m_running;
};

}  // namespace srpc

#endif  // SRPC_SERVERHANDLE_HPP_
