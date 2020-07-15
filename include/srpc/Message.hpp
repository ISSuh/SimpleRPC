/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_MESSAGE_HPP_
#define SRPC_MESSAGE_HPP_

#include <string>
#include <vector>
#include <algorithm>

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
  uint8_t headerSize;
  uint8_t offset;
  uint8_t serviceNameLen;
  uint8_t rpcNameLen;
  uint8_t serializedJsonLen;
  uint8_t offset;
  uint32_t bodySize;
  uint32_t bodyIndex;
  uint32_t bodyLen;
  char uuid[36];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BodyPacket {
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

  HeaderPacket getHeader() const { return m_header; }
  BodyPacket getBody() const { return m_body; }

  const std::string& serializedHeader() const {
    std::string str(reinterpret_cast<const char*>(&m_header));
    return str;
  }
  const std::string& serializedBody() const {
    std::string str;
    str.append(&m_body.serviceName[0]);
    str.append(&m_body.rpcName[0]);
    str.append(&m_body.serializedJson[0]);

    return str;
  }

  void deserializeHeader(const std::string& header) {
    m_header = *reinterpret_cast<HeaderPacket*>(const_cast<char*>(header.c_str()));
  }

  void deserializeBody(const std::string& body) {
    uint32_t start = 0;
    uint32_t end = m_header.serviceNameLen;

    std::copy(body.begin() + start, body.begin() + (start + end) + 1, std::back_inserter(m_body.rpcName));

    start = end + 1;
    end = m_header.rpcNameLen;
    std::copy(body.begin() + start, body.begin() + (start + end) + 1, std::back_inserter(m_body.serviceName));

    start = end + 1;
    std::copy(body.begin() + start, body.end(), std::back_inserter(m_body.serializedJson));
  }

 private:
  HeaderPacket m_header;
  BodyPacket m_body;
};

}  // namespace srpc

#endif  // SRPC_MESSAGE_HPP_
