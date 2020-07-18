/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SESSION_HPP_
#define SRPC_SESSION_HPP_

#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

#include "Common.hpp"

namespace srpc {

class Session {
 public:
  virtual ~Session() = default;

  virtual void connect(const TcpResolverIterator& endpointIter) { UNUSED(endpointIter); }
  virtual void read(Command cmd) = 0;
  virtual void write(Command cmd, const std::string& test) = 0;
  virtual void close() = 0;

  virtual TcpSocket& getSocket() { return m_socket; }

  virtual void setUUID(const Uuid& uuid ) { m_uuid = uuid; }
  virtual const Uuid& getUUID() const { return m_uuid; }

 protected:
  explicit Session(IoService& ioContext) : m_socket(ioContext) {}

 private:
  TcpSocket m_socket;
  Uuid m_uuid;
};

}  // namespace srpc

#endif  // SRPC_SESSION_HPP_
