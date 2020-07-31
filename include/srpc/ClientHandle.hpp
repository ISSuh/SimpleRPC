/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_CLIENTHANDLE_HPP_
#define SRPC_CLIENTHANDLE_HPP_

#include <iostream>
#include <string>
#include <memory>
#include <utility>

#include <boost/asio.hpp>

#include "helper/Common.hpp"
#include "net/NetCreator.hpp"
#include "net/client/Client.hpp"

namespace srpc {

class ClientHandle {
 public:
  explicit ClientHandle(ProtocolType protocolType, FunctionType funcType)
    : m_client(std::move(m_creator.createSystem(protocolType, funcType))),
      m_protocolType(protocolType),
      m_funcType(funcType) {}

  ~ClientHandle() {}

  void connect(const std::string& host, const std::string& port) {
    m_client->connect(host, port);
  }

  void terminate() {
    m_client->close();
  }

  void request(const std::string& serviceName,
               const std::string& rpcName,
               const std::string& params) {
    m_client->request(serviceName, rpcName, params);
  }

  ProtocolType getProtocolType() const { return m_protocolType; }
  FunctionType getFunctionType() const { return m_funcType; }

 private:
  ClientCreator m_creator;
  std::unique_ptr<Client> m_client;

  ProtocolType m_protocolType;
  FunctionType m_funcType;
};

}   // namespace srpc

#endif  // SRPC_CLIENTHANDLE_HPP_
