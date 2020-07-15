/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_MESSAGE_HPP_
#define SRPC_MESSAGE_HPP_

#include <string>
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

#pragma pack(push, 1)
struct HeaderPacket {
  uint8_t command;
  uint8_t reserve;
  uint8_t offset;
  uint8_t headerSize;
  uint32_t bodySize;
  uint32_t bodyIndex;
  uint32_t bodyLen;
  char uuid[36];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BodyPacket {
  uint8_t serviceNameLen;
  uint8_t rpcNameLen;
  uint8_t argCount;
  uint8_t offset;
  std::vector<char> serviceName;
  std::vector<char> rpcName;
  std::vector<char> serializedJson;
};
#pragma pack(pop)

class Message {
 public:
  Message() = default;
  ~Message() = default;

  void makeMessage(NET_COMMAND cmd) {}

  size_t getHeaderSize() const { return sizeof(m_header); }
  size_t getBodySize() const { return sizeof(m_body); }

  void setHeader() {}
  void setBody() {}

  HeaderPacket getHeader() const { return m_header; }
  BodyPacket getBody() const { return m_body; }

  const std::string& getSerializedHeader() const {
    std::string str(reinterpret_cast<const char*>(&m_header));
    return str;
  }
  const std::string& getSerializedBody() const {
    std::string str(reinterpret_cast<const char*>(&m_body));
    return str;
  }

 private:
  HeaderPacket m_header;
  BodyPacket m_body;
};

}  // namespace srpc

#endif  // SRPC_MESSAGE_HPP_
