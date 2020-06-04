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

namespace srpc {

class Session {
 public:
  virtual ~Session() = default;

  virtual void connect(const boost::asio::ip::tcp::resolver::iterator& endpointIter) {}
  virtual void read() {}
  virtual void write(const std::string& test) {}
  virtual void close() = 0;

  virtual boost::asio::ip::tcp::socket& getSocket() { return m_socket; }

  virtual void setUUID(const boost::uuids::uuid& uuid ) { m_uuid = uuid; }
  virtual const boost::uuids::uuid& getUUID() const { return m_uuid; }

 protected:
  explicit Session(boost::asio::io_service& ioContext) : m_socket(ioContext) {}

  virtual void connectHandler(const boost::system::error_code& error) {}
  virtual void readHandler(const boost::system::error_code& error, size_t len, const char* data) = 0;
  virtual void writeHandler(const boost::system::error_code& error)  = 0;

 private:
  boost::asio::ip::tcp::socket m_socket;
  boost::uuids::uuid m_uuid;
};

}  // namespace srpc

#endif  // SRPC_SESSION_HPP_
