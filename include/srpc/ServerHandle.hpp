/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVERHANDLE_HPP_
#define SRPC_SERVERHANDLE_HPP_

#include <iostream>
#include <string>
#include <memory>
#include <utility>

#include <boost/asio.hpp>

#include "helper/Common.hpp"
#include "net/NetCreator.hpp"
#include "net/server/Server.hpp"

namespace srpc {

class ServerHandle {
 public:
  explicit ServerHandle(ProtocolType protocolType, FunctionType funcType)
    : m_server(std::move(m_creator.createSystem(protocolType, funcType))),
      m_protocolType(protocolType),
      m_funcType(funcType) {}

  ~ServerHandle() = default;

  void setServer(const uint32_t port) {
    m_server->configure(port);
  }

  void run() {
    m_server->run();
  }

  ProtocolType getProtocolType() const { return m_protocolType; }
  FunctionType getFunctionType() const { return m_funcType; }

 private:
  ServerCreator m_creator;
  std::unique_ptr<Server> m_server;

  ProtocolType m_protocolType;
  FunctionType m_funcType;
};

}  // namespace srpc

#endif  // SRPC_SERVERHANDLE_HPP_
