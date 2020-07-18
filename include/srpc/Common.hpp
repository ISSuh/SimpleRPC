/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_COMMON_HPP_
#define SRPC_COMMON_HPP_

#include <string>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

namespace srpc {

namespace asio = boost::asio;

using ErrorCode = boost::system::error_code;

using IoService = boost::asio::io_service;

using TcpSocket = boost::asio::ip::tcp::socket;
using TcpAcceptor = boost::asio::ip::tcp::acceptor;
using TcpEndPoint = boost::asio::ip::tcp::endpoint;
using TcpResolver = boost::asio::ip::tcp::resolver;
using TcpResolverQuery = boost::asio::ip::tcp::resolver::query;
using TcpResolverIterator = boost::asio::ip::tcp::resolver::iterator;

using Uuid = boost::uuids::uuid;

const size_t MAX_LEN = 1024;

template<typename T>
void UNUSED(T&&) {}

enum class Status : uint8_t {
  OK
};

enum class ProtocolType : uint8_t {
  TCP,
  UDP
};

enum class FunctionType : uint8_t {
  SYNC,
  ASYNC
};

enum class Command : uint8_t {
  CONNECT,
  ACCEPT,
  CLOSE,
  REQUEST,
  REPONSE,
  ACK,
  PING
};

}  // namespace srpc

#endif  // SRPC_COMMON_HPP_
