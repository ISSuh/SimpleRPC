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

namespace srpc {

using namespace boost;

class TcpClient : public std::enable_shared_from_this<TcpClient> {
 public:
  TcpClient(asio::io_context& ioContext,
            const std::string& host,
            const std::string& port) : m_socket(ioContext) {
    asio::ip::tcp::resolver resolver(ioContext);
    asio::ip::tcp::resolver::query query(host, port);
    asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    asio::async_connect(m_socket, endpoint_iterator,
          std::bind(&TcpClient::connectHandle, this, std::placeholders::_1));

    sleep(1);
  }

  ~TcpClient() = default;

 private:
  void connectHandle(const system::error_code& error) {
    std::cout << "---connectHandle---\n";

    if (!error) {
      std::cout << "Connect Success! " << std::endl;

      const std::string msg = "HelloServer!";
      asio::async_write(m_socket, asio::buffer(msg.c_str(), msg.length()),
            std::bind(&TcpClient::writeHandle, this, std::placeholders::_1));
    } else {
      std::cout << "Connect Fail! : " << error.message() << std::endl;
    }
  }

  void writeHandle(const system::error_code& error) {
    std::cout << "---writeHandler---\n";

    if (!error) {
      std::cout << "Write Succes! " << std::endl;

      char msg[100];
      m_socket.async_read_some(asio::buffer(msg, 100),
            std::bind(&TcpClient::readHandle, this, std::placeholders::_1, msg));
    } else {
      std::cout << "Write Fail! : " << error.message() << std::endl;
    }
  }

  void readHandle(const system::error_code& error, const std::string& data) {
    std::cout << "---readHandle---\n";

    if (!error) {
      std::cout << "Read Succes! : " << data << std::endl;
    } else {
      std::cout << "Read Fail! : " << error.message() << std::endl;
    }
  }

 private:
  asio::ip::tcp::socket m_socket;
};

}   // namespace srpc

#endif  // SRPC_TCPCLIENT_HPP_
