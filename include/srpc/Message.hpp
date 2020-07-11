/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_MESSAGE_HPP_
#define SRPC_MESSAGE_HPP_

#include <vector>

namespace srpc {

enum class NET_COMMAND : uint8_t {
  REQUEST,
  REPONSE,
  ACCEPT,
  CONNECT,
  CLOSE,
  ACK,
  PING
};

class Message {
 public:
  Message() = default;
  ~Message() = default;

 private:
  void makeMessage() {}

 private:
  struct PacketHeader {
    uint8_t command;
    uint8_t reserve;
    uint8_t offset;
    uint8_t headerLen;
    uint32_t bodyLen;
    char uuid[33];
  };

  struct PacketBody {
    uint8_t serviceNameLen;
    uint8_t rpcNameLen;
    uint8_t argCount;
    uint8_t offset;
    std::vector<char> serviceName;
    std::vector<char> rpcName;
    std::vector<char> serializedJson;
  };

  struct Packet_t {
    PacketHeader header;
    PacketBody body;
  };

 private:
  Packet_t m_packet;
};

}  // namespace srpc

#endif  // SRPC_MESSAGE_HPP_
