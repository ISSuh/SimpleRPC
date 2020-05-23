/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_TCP_SERVER_HPP
#define SRPC_TCP_SERVER_HPP

#include <memory>
#include <boost/asio.hpp>

#include "server.hpp"
#include "session.hpp"

namespace sprc {

using namespace boost;

class TcpServer {
 public:
  TcpServer(asio::io_context& ioContext, uint32_t port) :
      m_socket(ioContext),
      m_acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    accept();
  }

  ~TcpServer() = default;

 private:
  void accept() {
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code ec) {
        if (!ec) {
          std::make_shared<Session>(std::move(m_socket))->start();
        }

        accept();
      }
    );
  }
 
 private:
  asio::ip::tcp::socket m_socket;
  asio::ip::tcp::acceptor m_acceptor;
};

} // namespace sprc

#endif // SRPC_TCP_SERVER_HPP
