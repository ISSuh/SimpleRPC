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

#include "Common.hpp"
#include "Client.hpp"
#include "ClientSession.hpp"

namespace srpc {

class TcpClient : public Client {
 public:
  TcpClient(IoService& ioContext) : m_ioContext(ioContext),
                                    m_session(ioContext, *this),
                                    m_resolver(ioContext) {}

  ~TcpClient() = default;

  void connect(const std::string& host, const std::string& port) {
    TcpResolverQuery clientQuery(host, port);
    TcpResolverIterator endpointIter = m_resolver.resolve(clientQuery);

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
  IoService& m_ioContext;
  ClientSession<TcpClient> m_session;

  TcpResolver m_resolver;
};

}   // namespace srpc

#endif  // SRPC_TCPCLIENT_HPP_
