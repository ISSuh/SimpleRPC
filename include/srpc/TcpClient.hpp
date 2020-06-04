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

#include "Client.hpp"
#include "ClientSession.hpp"

namespace srpc {

class TcpClient : public Client {
 public:
  TcpClient(boost::asio::io_service& ioContext) : m_ioContext(ioContext),
                                       m_session(ioContext, *this),
                                       m_resolver(ioContext) {}

  ~TcpClient() = default;

  void connect(const std::string& host, const std::string& port) {
    boost::asio::ip::tcp::resolver::query clientQuery(host, port);
    boost::asio::ip::tcp::resolver::iterator endpointIter = m_resolver.resolve(clientQuery);

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
  ClientSession<TcpClient> m_session;

  boost::asio::ip::tcp::resolver m_resolver;
};

}   // namespace srpc

#endif  // SRPC_TCPCLIENT_HPP_
