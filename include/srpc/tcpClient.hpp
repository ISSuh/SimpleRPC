/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_TCPCLIENT_HPP_
#define SRPC_TCPCLIENT_HPP_

#include <iostream>
#include <string>
#include <functional>

#include <boost/asio.hpp>

#include "session.hpp"

namespace srpc {

class TcpClient {
 public:
  TcpClient(boost::asio::io_service& ioContext,
            const std::string& host,
            const std::string& port) : m_ioContext(ioContext),
                                       m_session(ioContext, this),
                                       m_resolver(ioContext),
                                       m_query(host, port) {}

  ~TcpClient() = default;

  void connect() {
    boost::asio::ip::tcp::resolver::iterator endpointIter = m_resolver.resolve(m_query);
    m_session.connect(endpointIter);
  }

  void close() {
    m_session.close();
  }

  void request(const std::string& msg) {
    m_session.write(msg);
  }

  void updateConnect() {}
  void updateRead() {}
  void updateWrite() {}

 private:
  boost::asio::io_service& m_ioContext;
  Session<TcpClient> m_session;

  boost::asio::ip::tcp::resolver m_resolver;
  boost::asio::ip::tcp::resolver::query m_query;
};

}   // namespace srpc

#endif  // SRPC_TCPCLIENT_HPP_
