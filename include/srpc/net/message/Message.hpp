/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_NET_MESSAGE_MESSAGE_HPP_
#define SRPC_NET_MESSAGE_MESSAGE_HPP_

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "../../helper/Common.hpp"

namespace srpc {

const size_t HEADER_SIZE = 65;
const size_t UUID_SIZE = 36;

struct HeaderPacket {
  HeaderPacket() : command(0),
                   reserve(0),
                   headerSize(HEADER_SIZE),
                   bodyIndex(0),
                   bodySize(0),
                   size(HEADER_SIZE),
                   bodyLen(1),
                   uuid("00000000-0000-0000-0000-000000000000") {}


  uint8_t command;
  uint8_t reserve;
  uint8_t headerSize;
  uint8_t bodyIndex;
  uint32_t bodySize;
  uint32_t size;
  uint32_t bodyLen;
  char uuid[37];
};

struct RequestHeader {
  uint32_t serviceNameLen;
  uint32_t rpcNameLen;
  uint32_t serializedJsonLen;
};

struct RequestBody {
  std::string serviceName;
  std::string rpcName;
  std::string serializedJson;
};

struct RequestPacket {
  RequestHeader header;
  RequestBody body;
};

struct ResponseHeader {
  uint32_t responseLen;
};

struct ResponseBody {
  std::string responseType;
  std::string response;
};

struct ResponsePacket {
  ResponseHeader header;
  ResponseBody body;
};

class Message {
 public:
  Message(const std::string& uuid, Command cmd) {
    setUuid(uuid);
    setCommand(cmd);
  }

  explicit Message(std::string serializedMessage) {
    deserialize(serializedMessage);
  }

  ~Message() = default;

  Command getCommand() const { return static_cast<Command>(m_header.command); }
  size_t getSize() const { return m_header.size; }
  size_t getHeaderSize() const { return m_header.headerSize; }
  size_t getBodySize() const { return m_header.bodySize; }
  std::string getUuid() const {
    return std::string(m_header.uuid);
  }

  void setUuid(const std::string& uuid) {
    std::copy(&uuid[0], &uuid[0] + UUID_SIZE, m_header.uuid);
  }

  void setCommand(Command cmd) {
    m_header.command = static_cast<uint8_t>(cmd);
  }

  const HeaderPacket& getPacketHeader() const { return m_header; }
  const RequestPacket& getRequestPacket() const { return m_request; }
  const ResponsePacket& getResponsePacket() const { return m_response; }

  void setPacketHeader(HeaderPacket header) {
    const char* buffer = reinterpret_cast<const char*>(&header);
    std::copy(buffer, buffer + HEADER_SIZE, reinterpret_cast<char*>(&m_header));
  }

  void setHeader(RequestHeader header) {
    const char* buffer = reinterpret_cast<const char*>(&header);
    std::copy(buffer, buffer + HEADER_SIZE, reinterpret_cast<char*>(&m_request.header));
    updateRequestPacketSize();
  }

  void setHeader(ResponseHeader header) {
    const char* buffer = reinterpret_cast<const char*>(&header);
    std::copy(buffer, buffer + HEADER_SIZE, reinterpret_cast<char*>(&m_response.header));
    updateResponsePacketSize();
  }

  void setBody(RequestBody body) {
    size_t bodysize = sizeof(body);
    const char* buffer = reinterpret_cast<const char*>(&body);
    std::copy(buffer, buffer + bodysize, reinterpret_cast<char*>(&m_request.body));
    updateRequestPacketSize();
  }

  void setBody(ResponseBody body) {
    size_t bodysize = sizeof(body);
    const char* buffer = reinterpret_cast<const char*>(&body);
    std::copy(buffer, buffer + bodysize, reinterpret_cast<char*>(&m_response.body));
    updateResponsePacketSize();
  }

  void setBody(const std::string& serviceName,
               const std::string& rpcName,
               const std::string& serializedParams) {
    m_body.serviceName = serviceName;
    m_body.rpcName = rpcName;
    m_body.serializedJson = serializedParams;

    updateRequestPacketSize();
  }

  template<class T>
  void setBody(const T& reponse) {
    size_t size = sizeof(reponse);
    std::vector<char> serializeBuffer(size);
    std::copy(&serializeBuffer[0], &serializeBuffer[0] + size,
              reinterpret_cast<const char*>(&m_response.body.response));

    m_response.body.responseType = demangle(typeid(T).name());

    updateResponsePacketSize();
  }

  void setServiceName(const std::string& serviceName) {
    m_body.serviceName = serviceName;
    updateRequestPacketSize();
  }

  void setRpcName(const std::string& rpcName) {
    m_body.rpcName = rpcName;
    updateRequestPacketSize();
  }

  void setParams(const std::string& serializedParams) {
    m_body.serializedJson = serializedParams;
    updateRequestPacketSize();
  }

  /**
   * Message serialize and deserialize
   */
  std::string serialize() const {
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

    deserializeHeader(serializedMessage.substr(0, HEADER_SIZE - 1));
    deserializeBody(serializedMessage.substr(HEADER_SIZE));
  }

  // Test Code
  void printPacketHeader() {
    std::string uuid(m_header.uuid);
    std::cout << "======== Header ========\n";
    std::cout << "command : " <<
                  CommandToString(static_cast<Command>(m_header.command)) << std::endl;
    std::cout << "headerSize : " << static_cast<unsigned>(m_header.headerSize) << std::endl;
    std::cout << "bodySize : " << m_header.bodySize << std::endl;
    std::cout << "size : " << m_header.size << std::endl;
    std::cout << "bodyLen : " << m_header.bodyLen << std::endl;
    std::cout << "bodyIndex : " << static_cast<unsigned>(m_header.bodyIndex) << std::endl;
    std::cout << "uuid : " << uuid << std::endl;
  }

  void printRequestPacketHeader() {
    std::cout << "======== Header ========\n";
    std::cout << "serviceNameLen : " << m_request.header.serviceNameLen << std::endl;
    std::cout << "rpcNameLen : " << m_request.header.rpcNameLen << std::endl;
    std::cout << "serializedJsonLen : " << m_request.header.serializedJsonLen << std::endl;
    std::cout << "======== Body ========\n";
    std::cout << "serviceName : " << m_request.body.serviceName << std::endl;
    std::cout << "rpcName : " << m_request.body.rpcName << std::endl;
    std::cout << "serializedJson : " << m_request.body.serializedJson << std::endl;
  }

  void printReponsePacketHeader() {
    std::string uuid(m_header.uuid);
    std::cout << "======== Header ========\n";
    std::cout << "size : " << m_response.header.responseLen << std::endl;
    std::cout << "======== Body ========\n";
    std::cout << "serializedJson : " << m_response << std::endl;
  }

 private:
  std::string serializedHeader() const;
  std::string serializedBody() const;
  void deserializeHeader(const std::string& header);
  void deserializeBody(const std::string& body);

  void updateRequestPacketSize();
  void updateResponsePacketSize();

 private:
  HeaderPacket m_header;
  RequestBody m_body;

  RequestPacket m_request;
  ResponsePacket m_response;
};

std::string Message::serializedHeader() const {
  const char* buf = reinterpret_cast<const char*>(&m_header);
  return std::string(buf, buf + HEADER_SIZE);
}

std::string Message::serializedBody() const {
  std::string str;
  str.append(m_body.serviceName);
  str.append(m_body.rpcName);
  str.append(m_body.serializedJson);

  return str;
}

void Message::deserializeHeader(const std::string& header) {
  m_header = *reinterpret_cast<const HeaderPacket*>(&header[0]);
}

void Message::deserializeBody(const std::string& body) {
  int32_t rpcNameIndex = m_header.serviceNameLen;
  int32_t serializedJsonIndex = rpcNameIndex + m_header.rpcNameLen;

  m_body.serviceName = body.substr(0, m_header.serviceNameLen);
  m_body.rpcName = body.substr(rpcNameIndex, m_header.rpcNameLen);
  m_body.serializedJson = body.substr(serializedJsonIndex, m_header.serializedJsonLen);
}

void Message::updateRequestPacketSize() {
  m_request.header.serviceNameLen = m_request.body.serviceName.size();
  m_request.header.rpcNameLen = m_request.body.rpcName.size();
  m_request.header.serializedJsonLen = m_request.body.serializedJson.size();

  m_header.bodySize = m_request.header.serviceNameLen +
                      m_request.header.rpcNameLen +
                      m_request.header.serializedJsonLen;

  m_header.size = m_header.headerSize + m_header.bodySize;
}

void Message::updateResponsePacketSize() {
  m_response.header.responseLen = m_response.body.response.size();
  m_header.bodySize = m_response.header.responseLen;

  m_header.size = m_header.headerSize + m_header.bodySize;
}

}  // namespace srpc

#endif  // SRPC_NET_MESSAGE_MESSAGE_HPP_
