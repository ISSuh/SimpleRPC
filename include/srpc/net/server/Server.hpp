/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_NET_SERVER_SERVER_HPP_
#define SRPC_NET_SERVER_SERVER_HPP_

#include <string>

#include "../../helper/Common.hpp"
#include "../session/Session.hpp"

namespace srpc {

class Server {
 public:
  Server() = default;
  virtual ~Server() = default;

  virtual void configure(const uint32_t port) = 0;
  virtual void run() = 0;

  virtual void onRead(const std::string& uuid, const std::string& serializedMessage) = 0;
  virtual void onWrite(const std::string& uuid) = 0;

 protected:
  virtual void accpet() = 0;
};

}  // namespace srpc

#endif  // SRPC_NET_SERVER_SERVER_HPP_
