/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_COMMON_HPP
#define SRPC_COMMON_HPP

namespace srpc {

using IoService = boost::asio::io_service;
using TcpSocket = boost::asio::ip::tcp::socket;
using Uuid = boost::uuids::uuid;


enum class ProtocolType : uint8_t {
  TCP,
  UDP
};

enum class FunctionType : uint8_t {
  SYNC,
  ASYNC
};

} // srpc

#endif