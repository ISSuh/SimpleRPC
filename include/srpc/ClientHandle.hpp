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

#include "Common.hpp"
#include "Creator.hpp"
#include "Client.hpp"

namespace srpc {

class ClientHandle {
 public:
  explicit ClientHandle(ProtocolType protocolType, FunctionType funcType)
    : m_client(std::move(m_creator.createSystem(protocolType, funcType))) {}

  ~ClientHandle() {}

  void connect(const std::string& host, const std::string& port) {
    m_client->connect(host, port);
  }

  void terminate() {
    m_client->close();
  }

  void request(const std::string& msg) {
    m_client->request(msg);
  }

 private:
  IoService m_ioContext;

  ProtocolType m_protocolType;
  FunctionType m_funcType;

  ClientCreator m_creator;
  std::unique_ptr<Client> m_client;

  std::thread t;
};

}   // namespace srpc

#endif  // SRPC_CLIENTHANDLE_HPP_
