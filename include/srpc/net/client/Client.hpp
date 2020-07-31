/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_NET_CLIENT_CLIENT_HPP_
#define SRPC_NET_CLIENT_CLIENT_HPP_

#include <string>

namespace srpc {

class Client {
 public:
  Client() = default;
  virtual ~Client() = default;

  virtual void connect(const std::string& host, const std::string& port) = 0;
  virtual void close() = 0;

  virtual void request(const std::string& serviceName,
                       const std::string& rpcName,
                       const std::string& params) = 0;

  virtual void onConnect() = 0;
  virtual void onRead(std::string serializedMessage) = 0;
  virtual void onWrite() = 0;
};

}  // namespace srpc

#endif  // SRPC_NET_CLIENT_CLIENT_HPP_
