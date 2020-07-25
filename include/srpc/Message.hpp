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

const size_t HEADER_SIZE = 65;

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
                   bodyLen(0),
                   bodyIndex(0),
                   uuid("00000000-0000-0000-0000-000000000000") {}


  uint8_t command;
  uint8_t reserve;
  uint8_t headerSize;
  uint8_t bodyIndex;
  uint32_t bodySize;
  uint32_t size;
  uint32_t serviceNameLen;
  uint32_t rpcNameLen;
  uint32_t serializedJsonLen;
  uint32_t bodyLen;
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
  explicit Message(const std::string& serializedMaessage) {
    deserialize(serializedMaessage);
  }

  ~Message() = default;

  size_t getSize() const { m_header.size; }
  size_t getHeaderSize() const { return HEADER_SIZE; }
  size_t getBodySize() const { return m_header.bodySize; }

  const HeaderPacket& getHeader() const { return m_header; }
  const BodyPacket& getBody() const { return m_body; }

  /**
   * Set header packet infomation
   */ 
  void setHeader(HeaderPacket header) {
    // std::copy(&header, &(header) + HEADER_SIZE, m_header);
    memcpy(reinterpret_cast<void*>(&m_header),
           reinterpret_cast<void*>(&header),
           HEADER_SIZE);
  }

  void setUuid(const std::string& uuid) {
    uuid.copy(m_header.uuid, uuid.size()+1);
    // std::copy(&uuid, &(uuid) + 36, m_header.uuid);
  }

  void setCommand(Command cmd) {
    m_header.command = static_cast<uint8_t>(cmd);
  }

  /**
   * Set body packet infomation
   */
  void setBody(BodyPacket body) {
    // std::copy(&body, &(body) + HEADER_SIZE, m_body);
    memcpy(reinterpret_cast<void*>(&m_body),
           reinterpret_cast<void*>(&body),
           sizeof(body));
  }

  void setBody(const std::string& serviceName,
               const std::string& rpcName,
               const std::string& serializedParams) {
    m_body.serviceName = serviceName;
    m_header.serviceNameLen = serviceName.size();

    m_body.serviceName = serviceName;
    m_header.serviceNameLen = serviceName.size();

    m_body.rpcName = rpcName;
    m_header.rpcNameLen = rpcName.size();

    updatePacketSize();
  }

  void setServiceName(const std::string& serviceName) {
    m_body.serviceName = serviceName;
    m_header.serviceNameLen = serviceName.size();

    updatePacketSize();
  }

  void setRpcName(const std::string& rpcName) {
    m_body.rpcName = rpcName;
    m_header.rpcNameLen = rpcName.size();

    updatePacketSize();
  }

  void setParams(const std::string& serializedParams) {
    m_body.serializedJson = serializedParams;
    m_header.serializedJsonLen = serializedParams.size();

    updatePacketSize();
  }

  /**
   * Message serialize and deserialize
   */
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

  // Test Code
  void printPacketforDubugging() {
    std::string uuid(m_header.uuid);
    std::cout << "-----------------------\n";
    std::cout << "======== Header ========\n";
    std::cout << "command : " <<
                  CommandToString(static_cast<Command>(m_header.command)) << std::endl;
    std::cout << "headerSize : " << static_cast<unsigned>(m_header.headerSize) << std::endl;
    std::cout << "bodySize : " << m_header.bodySize << std::endl;
    std::cout << "size : " << m_header.size << std::endl;
    std::cout << "serviceNameLen : " << m_header.serviceNameLen << std::endl;
    std::cout << "rpcNameLen : " << m_header.rpcNameLen << std::endl;
    std::cout << "serializedJsonLen : " << m_header.serializedJsonLen << std::endl;
    std::cout << "bodyLen : " << m_header.bodyLen << std::endl;
    std::cout << "bodyIndex : " << static_cast<unsigned>(m_header.bodyIndex) << std::endl;
    std::cout << "uuid : " << uuid << std::endl;
    std::cout << "======== Body ========\n";
    std::cout << "serviceName : " << m_body.serviceName << std::endl;
    std::cout << "rpcName : " << m_body.rpcName << std::endl;
    std::cout << "serializedJson : " << m_body.serializedJson << std::endl;
  }

 private:
  const std::string& serializedHeader() {
    char buf[HEADER_SIZE];
    std::copy(reinterpret_cast<char*>(&m_header),
              reinterpret_cast<char*>(&m_header) + HEADER_SIZE,
              buf);

    return std::string(buf);
  }

  const std::string& serializedBody() {
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

  void updatePacketSize() {
    m_header.bodySize = m_header.serviceNameLen +
                        m_header.rpcNameLen +
                        m_header.serializedJsonLen;
    m_header.size = m_header.size + m_header.bodySize;
  }

 private:
  HeaderPacket m_header;
  BodyPacket m_body;
};

}  // namespace srpc

#endif  // SRPC_MESSAGE_HPP_
