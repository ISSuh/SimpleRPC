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

#include "Common.hpp"

namespace srpc {

const size_t HEADER_SIZE = 57;

#pragma pack(push, 1)
struct HeaderPacket {
  HeaderPacket() : command(0),
                   reserve(0),
                   headerSize(HEADER_SIZE),
                   bodySize(0),
                   size(HEADER_SIZE),
                   serviceNameLen(0),
                   rpcNameLen(0),
                   serializedJsonLen(0),
                   offset(0),
                   bodyLen(0),
                   bodyIndex(0),
                   uuid("\n") {}

  uint8_t command;
  uint8_t reserve;
  uint8_t headerSize;
  uint8_t bodySize;
  uint32_t size;
  uint8_t serviceNameLen;
  uint8_t rpcNameLen;
  uint8_t serializedJsonLen;
  uint8_t offset;
  uint32_t bodyLen;
  uint32_t bodyIndex;
  char uuid[37];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BodyPacket {
  std::string serviceName;
  std::string rpcName;
  std::string serializedJson;
};
#pragma pack(pop)

class Message {
 public:
  Message() {}
  explicit Message(Command cmd) {
    m_header.command = static_cast<uint8_t>(cmd);
  }

  ~Message() = default;

  size_t getSize() const { m_header.size; }
  size_t getHeaderSize() const { return HEADER_SIZE; }
  size_t getBodySize() const { return m_header.bodySize; }

  const HeaderPacket& getHeader() const { return m_header; }
  const BodyPacket& getBody() const { return m_body; }

  void setHeader(const HeaderPacket header) {
    std::copy(&header, &(header) + HEADER_SIZE, m_header);
  }

  void setBody(const BodyPacket body) {
    std::copy(&body, &(body) + HEADER_SIZE, m_body);
  }

  void setUuid(const std::string& uuid) {
    uuid.copy(m_header.uuid, uuid.size()+1);
    // std::copy(&uuid, &(uuid) + 36, m_header.uuid);
  }

  void setCommand(Command cmd) {
    m_header.command = static_cast<uint8_t>(cmd);
  }

  void setBodySize(uint32_t size) {
    m_header.bodySize = size;
    m_header.size = m_header.headerSize + m_header.bodySize;
  }

  const std::string& serialize() {
    if (m_header.bodySize == 0) {
      return serializedHeader();
    }

    return serializedHeader() + serializedBody();
  }

  void deserialize(const std::string& serializedMessage) {
    if (serializedMessage.size() <= HEADER_SIZE) {
      deserializeHeader(serializedMessage);
      return;
    }

    deserializeHeader(serializedMessage.substr(0, HEADER_SIZE));
    deserializeBody(serializedMessage.substr(HEADER_SIZE + 1));
  }

 private:
  const std::string& serializedHeader() const {
    std::string str(reinterpret_cast<const char*>(&m_header));
    return str;
  }
  const std::string& serializedBody() const {
    std::string str;
    str.append(m_body.serviceName);
    str.append(m_body.rpcName);
    str.append(m_body.serializedJson);

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
