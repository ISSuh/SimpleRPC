/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SYNCSESSION_HPP_
#define SRPC_SYNCSESSION_HPP_

#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

#include "Common.hpp"

namespace srpc {

class SyncSession {
 public:
  virtual ~SyncSession() = default;

  virtual void connect(const TcpResolverIterator& endpointIter) { UNUSED(endpointIter); }
  virtual void read() {}
  virtual void write(const std::string& test) { UNUSED(test); }
  virtual void close() = 0;

  virtual TcpSocket& getSocket() { return m_socket; }

  virtual void setUUID(const Uuid& uuid ) { m_uuid = uuid; }
  virtual const Uuid& getUUID() const { return m_uuid; }

 protected:
  explicit SyncSession(IoService& ioContext) : m_socket(ioContext) {}

  virtual void connectHandler(const ErrorCode& error) { UNUSED(error); }
  virtual void readHandler(const ErrorCode& error, size_t len) = 0;
  virtual void writeHandler(const ErrorCode& error) = 0;

 private:
  TcpSocket m_socket;
  Uuid m_uuid;
};

}  // namespace srpc

#endif  // SRPC_SYNCSESSION_HPP_
