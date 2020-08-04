/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_HELPER_COMMON_HPP_
#define SRPC_HELPER_COMMON_HPP_

#include <string>
#include <map>
#include <memory>

#include <cxxabi.h>

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
  NONE,
  CONNECT,
  ACCEPT,
  CLOSE,
  REQUEST,
  REPONSE,
  ACK,
  PING
};

const std::map<Command, std::string> CommandToStringMap {
  {Command::NONE, "NONE"},
  {Command::CONNECT, "CONNECT"},
  {Command::ACCEPT, "ACCEPT"},
  {Command::CLOSE, "CLOSE"},
  {Command::REQUEST, "REQUEST"},
  {Command::REPONSE, "REPONSE"},
  {Command::ACK, "ACK"},
  {Command::PING, "PING"}
};

std::string CommandToString(Command cmd) {
  return CommandToStringMap.at(cmd);
}

std::string demangle(const char* mangled) {
  int status;
  std::unique_ptr<char[], void (*)(void*)> result(
    abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
  return result.get() ? std::string(result.get()) : "error occurred";
}

}  // namespace srpc

#endif  // SRPC_HELPER_COMMON_HPP_
