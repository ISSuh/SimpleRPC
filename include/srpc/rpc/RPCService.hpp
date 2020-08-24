/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_RPC_RPCSERVICE_HPP_
#define SRPC_RPC_RPCSERVICE_HPP_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "../helper/Common.hpp"
#include "../helper/TaskRunner.hpp"

namespace srpc {

class RPCServcie {
 public:
  RPCServcie() = default;
  ~RPCServcie() = default;

  void addRpc(const std::string& service, const std::string& rpc) {
    return;
  }

 private:
  std::map<std::string, std::string> m_serviceMap;


};

}  // namespace srpc

#endif  // SRPC_RPC_RPCSERVICE_HPP_
