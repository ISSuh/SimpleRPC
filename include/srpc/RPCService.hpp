/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_RPCSERVICE_HPP_
#define SRPC_RPCSERVICE_HPP_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "Common.hpp"

namespace srpc {

class RPCServcie {
 public:
  RPCServcie() = default;
  ~RPCServcie() = default;

  template<typename T>
  void addRPCFunction(const std::string& functionName, const RPCFunction& rpcFunc) {
    if (m_rpcMap.find(functionName) != m_rpcMap.end()) {
      m_rpcMap[functionName] = rpcFunc;
    }
  }

  void callFunction() {}

 private:
  std::map<std::string, RPCFunction> m_rpcMap;
  const std::string m_serviceName;
};

}  // namespace srpc

#endif  // SRPC_RPCSERVICE_HPP_
