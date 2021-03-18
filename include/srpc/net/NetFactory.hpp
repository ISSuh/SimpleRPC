/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_NET_NETCREATOR_HPP_
#define SRPC_NET_NETCREATOR_HPP_

#include <iostream>

#include "../helper/Common.hpp"

#include "server/TcpServer.hpp"
#include "client/TcpClient.hpp"

namespace srpc {

template<typename T>
class Creator {
 public:
  Creator() = default;
  virtual ~Creator() = default;

  virtual T* createSystem(FunctionType funcType) = 0;
};


class TcpServerFactory : public Creator<Server> {
 public:
  TcpServerFactory() = default;
  virtual ~TcpServerFactory() = default;

  Server* createSystem(FunctionType funcType) override {
    switch (funcType) {
    case FunctionType::ASYNC :
      return static_cast<Server*>(new TcpServer());
    case FunctionType::SYNC :
      return static_cast<Server*>(new TcpServer());
    }
    return nullptr;
  }
};

class UdpServerFactory : public Creator<Server> {
 public:
  UdpServerFactory() = default;
  virtual ~UdpServerFactory() = default;

  Server* createSystem(FunctionType funcType) override {
    switch (funcType) {
    case FunctionType::ASYNC :
      return static_cast<Server*>(new TcpServer());
    case FunctionType::SYNC :
      return static_cast<Server*>(new TcpServer());
    }
    return nullptr;
  }
};

class ServerFactory {
 public:
  static Server* createServer(ProtocolType protocolType, FunctionType funcType) {
    switch (protocolType) {
      case ProtocolType::TCP : {
        TcpServerFactory tcpServerFactory;
        return tcpServerFactory.createSystem(funcType);
      }
      case ProtocolType::UDP : {
        UdpServerFactory udpServerFactory;
        return udpServerFactory.createSystem(funcType);
      }
    }
    return nullptr;
  }
};

class TcpClientFactory : public Creator<Client> {
 public:
  TcpClientFactory() = default;
  virtual ~TcpClientFactory() = default;

  Client* createSystem(FunctionType funcType) override {
    switch (funcType) {
    case FunctionType::ASYNC :
      return static_cast<Client*>(new TcpClient());
    case FunctionType::SYNC :
      return static_cast<Client*>(new TcpClient());
    }
    return nullptr;
  }
};

class UdpClientFactory : public Creator<Client> {
 public:
  UdpClientFactory() = default;
  virtual ~UdpClientFactory() = default;

  Client* createSystem(FunctionType funcType) override {
    switch (funcType) {
    case FunctionType::ASYNC :
      return static_cast<Client*>(new TcpClient());
    case FunctionType::SYNC :
      return static_cast<Client*>(new TcpClient());
    }
    return nullptr;
  }
};

class ClientFactory {
 public:
  static Client* createServer(ProtocolType protocolType, FunctionType funcType) {
    switch (protocolType) {
      case ProtocolType::TCP : {
        TcpClientFactory tcpClientFactory;
        return tcpClientFactory.createSystem(funcType);
      }
      case ProtocolType::UDP : {
        UdpClientFactory udpClientFactory;
        return udpClientFactory.createSystem(funcType);
      }
    }
    return nullptr;
  }
};

}  // namespace srpc

#endif  // SRPC_NET_NETCREATOR_HPP_

