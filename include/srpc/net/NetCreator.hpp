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

  virtual T* createSystem(ProtocolType protocolType, FunctionType funcType) = 0;
};

class ServerCreator : public Creator<Server> {
 public:
  ServerCreator() = default;
  virtual ~ServerCreator() = default;

  Server* createSystem(ProtocolType protocolType, FunctionType funcType) override {
    switch (protocolType) {
      case ProtocolType::TCP : {
        switch (funcType) {
        case FunctionType::ASYNC :
          return static_cast<Server*>(new TcpServer());
        case FunctionType::SYNC :
          return static_cast<Server*>(new TcpServer());
        }
      }
      case ProtocolType::UDP : {
        switch (funcType) {
        case FunctionType::ASYNC :
          return static_cast<Server*>(new TcpServer());
        case FunctionType::SYNC :
          return static_cast<Server*>(new TcpServer());
        }
      }
    }

    return nullptr;
  }
};

class ClientCreator : public Creator<Client> {
 public:
  ClientCreator() = default;
  virtual ~ClientCreator() = default;

  Client* createSystem(ProtocolType protocolType, FunctionType funcType) override {
    switch (protocolType) {
      case ProtocolType::TCP : {
        switch (funcType) {
        case FunctionType::ASYNC :
          return static_cast<Client*>(new TcpClient());
        case FunctionType::SYNC :
          return static_cast<Client*>(new TcpClient());
        }
      }
      case ProtocolType::UDP : {
        switch (funcType) {
        case FunctionType::ASYNC :
          return static_cast<Client*>(new TcpClient());
        case FunctionType::SYNC :
          return static_cast<Client*>(new TcpClient());
        }
      }
    }

    return nullptr;
  }
};

}  // namespace srpc

#endif  // SRPC_NET_NETCREATOR_HPP_

