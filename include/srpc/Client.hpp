/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_CLIENT_HPP_
#define SRPC_CLIENT_HPP_

#include <string>

namespace srpc {

class Client {
 public:
  Client() = default;
  virtual ~Client() = default;

  virtual void connect(const std::string& host, const std::string& port) = 0;
  virtual void close() = 0;

  virtual void request(const std::string& msg) = 0;

  virtual void updateConnect() = 0;
  virtual void updateRead() = 0;
  virtual void updateWrite() = 0;
};

}  // namespace srpc

#endif  // SRPC_CLIENT_HPP_
