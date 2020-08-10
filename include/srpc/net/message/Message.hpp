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
const size_t REQUEST_HEADER_SIZE = 16;
const size_t RESPONSE_HEADER_SIZE = 8;
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
  RequestHeader() : serviceNameLen(0),
                    rpcNameLen(0),
                    serializedJsonLen(0) {}

  uint32_t serviceNameLen;
  uint32_t rpcNameLen;
  uint32_t serializedJsonLen;
};

struct RequestBody {
  RequestBody() : serviceName(""),
                  rpcName(""),
                  serializedJson("") {}

  std::string serviceName;
  std::string rpcName;
  std::string serializedJson;
};

struct RequestPacket {
  RequestHeader header;
  RequestBody body;
};

struct ResponseHeader {
  ResponseHeader() : responseTypeLen(0),
                     responseLen(0) {}

  uint32_t responseTypeLen;
  uint32_t responseLen;
};

struct ResponseBody {
  ResponseBody() : responseType(""),
                   response("") {}

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

  void setUuid(const std::string& uuid) {
    std::copy(&uuid[0], &uuid[0] + UUID_SIZE, m_header.uuid);
  }

  void setCommand(Command cmd) {
    m_header.command = static_cast<uint8_t>(cmd);
  }

  Command getCommand() const { return static_cast<Command>(m_header.command); }
  size_t getSize() const { return m_header.size; }
  size_t getHeaderSize() const { return m_header.headerSize; }
  size_t getBodySize() const { return m_header.bodySize; }
  std::string getUuid() const {
    return std::string(m_header.uuid);
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

  void setResquest(const std::string& serviceName,
               const std::string& rpcName,
               const std::string& serializedParams) {
    m_request.body.serviceName = serviceName;
    m_request.body.rpcName = rpcName;
    m_request.body.serializedJson = serializedParams;

    updateRequestPacketSize();
  }

  template<class T>
  void setResponse(const T& reponse) {
    size_t size = sizeof(reponse);
    const char* buf = reinterpret_cast<const char*>(&reponse);
    std::string serializeBuffer(buf, buf + size);

    m_response.body.responseType = demangle(typeid(T).name());
    m_response.body.response = serializeBuffer;
    updateResponsePacketSize();
  }

  template<class T>
  void getResponse(T& response) {
    if (m_response.body.responseType != demangle(typeid(T).name())) {
      return;
    }

    std::swap(response, *reinterpret_cast<T*>(&m_response.body.response[0]));
  }

  void setServiceName(const std::string& serviceName) {
    m_request.body.serviceName = serviceName;
    updateRequestPacketSize();
  }

  void setRpcName(const std::string& rpcName) {
    m_request.body.rpcName = rpcName;
    updateRequestPacketSize();
  }

  void setParams(const std::string& serializedParams) {
    m_request.body.serializedJson = serializedParams;
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
  void printPacketHeader();
  void printRequestPacketHeader();
  void printReponsePacketHeader();

 private:
  std::string serializedHeader() const;
  std::string serializedBody() const;
  std::string serializedRequestPacket() const;
  std::string serializedResponsePacket() const;

  void deserializeHeader(const std::string& header);
  void deserializeBody(const std::string& body);
  void deserializeRequestHeader(const std::string& header);
  void deserializeRequestBody(const std::string& body);
  void deserializeResponseHeader(const std::string& header);
  void deserializeResponseBody(const std::string& body);

  void updateRequestPacketSize();
  void updateResponsePacketSize();

 private:
  HeaderPacket m_header;

  RequestPacket m_request;
  ResponsePacket m_response;
};

std::string Message::serializedHeader() const {
  const char* buf = reinterpret_cast<const char*>(&m_header);
  return std::string(buf, buf + HEADER_SIZE);
}

std::string Message::serializedBody() const {
  std::string serializedBodyBuf;

  switch (static_cast<Command>(m_header.command)) {
  case Command::REQUEST :
    serializedBodyBuf = serializedRequestPacket();
    break;
  case Command::REPONSE :
    serializedBodyBuf = serializedResponsePacket();
    break;
  default:
    break;
  }

  return serializedBodyBuf;
}

std::string Message::serializedRequestPacket() const {
  const char* buf = reinterpret_cast<const char*>(&m_request.header);
  std::string headerBuf(buf, buf + REQUEST_HEADER_SIZE);

  std::string bodyBuf;
  bodyBuf.append(m_request.body.serviceName);
  bodyBuf.append(m_request.body.rpcName);
  bodyBuf.append(m_request.body.serializedJson);

  return headerBuf + bodyBuf;
}

std::string Message::serializedResponsePacket() const {
  const char* buf = reinterpret_cast<const char*>(&m_response.header);
  std::string headerBuf(buf, buf + RESPONSE_HEADER_SIZE);

  std::string bodyBuf;
  bodyBuf.append(m_response.body.responseType);
  bodyBuf.append(m_response.body.response);

  return headerBuf + bodyBuf;
}

void Message::deserializeHeader(const std::string& header) {
  m_header = *reinterpret_cast<const HeaderPacket*>(&header[0]);
}

void Message::deserializeBody(const std::string& body) {
  std::string headerBuf;
  std::string bodyBuf;

  switch (static_cast<Command>(m_header.command)) {
  case Command::REQUEST :
    headerBuf = body.substr(0, REQUEST_HEADER_SIZE);
    bodyBuf = body.substr(REQUEST_HEADER_SIZE);

    deserializeRequestHeader(headerBuf);
    deserializeRequestBody(bodyBuf);
    break;
  case Command::REPONSE :
    headerBuf = body.substr(0, RESPONSE_HEADER_SIZE);
    bodyBuf = body.substr(RESPONSE_HEADER_SIZE);

    deserializeResponseHeader(headerBuf);
    deserializeResponseBody(bodyBuf);
    break;
  default:
    break;
  }
}

void Message::deserializeRequestHeader(const std::string& header) {
  m_request.header = *reinterpret_cast<const RequestHeader*>(&header[0]);
}

void Message::deserializeRequestBody(const std::string& body) {
  int32_t rpcNameIndex = m_request.header.serviceNameLen;
  int32_t serializedJsonIndex = rpcNameIndex + m_request.header.rpcNameLen;

  m_request.body.serviceName = body.substr(0, m_request.header.serviceNameLen);
  m_request.body.rpcName = body.substr(rpcNameIndex, m_request.header.rpcNameLen);
  m_request.body.serializedJson = body.substr(serializedJsonIndex);
}

void Message::deserializeResponseHeader(const std::string& header) {
  m_response.header = *reinterpret_cast<const ResponseHeader*>(&header[0]);
}

void Message::deserializeResponseBody(const std::string& body) {
  int32_t responseTypeIndex = m_response.header.responseTypeLen;

  m_response.body.responseType = body.substr(0, m_response.header.responseTypeLen);
  m_response.body.response = body.substr(responseTypeIndex);
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
  m_response.header.responseTypeLen = m_response.body.responseType.size();
  m_response.header.responseLen = m_response.body.response.size();

  m_header.bodySize = m_response.header.responseTypeLen +
                      m_response.header.responseLen;

  m_header.size = m_header.headerSize + m_header.bodySize;
}


void Message::printPacketHeader() {
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

void Message::printRequestPacketHeader() {
  std::cout << "======== Header ========\n";
  std::cout << "serviceNameLen : " << m_request.header.serviceNameLen << std::endl;
  std::cout << "rpcNameLen : " << m_request.header.rpcNameLen << std::endl;
  std::cout << "serializedJsonLen : " << m_request.header.serializedJsonLen << std::endl;
  std::cout << "======== Body ========\n";
  std::cout << "serviceName : " << m_request.body.serviceName << std::endl;
  std::cout << "rpcName : " << m_request.body.rpcName << std::endl;
  std::cout << "serializedJson : " << m_request.body.serializedJson << std::endl;
}

void Message::printReponsePacketHeader() {
  std::string uuid(m_header.uuid);
  std::cout << "======== Header ========\n";
  std::cout << "reponseTypeLen : " << m_response.header.responseTypeLen << std::endl;
  std::cout << "reponseLen : " << m_response.header.responseLen << std::endl;
  std::cout << "======== Body ========\n";
  std::cout << "reponseType : " << m_response.body.responseType << std::endl;
  // std::cout << "reponse : " << m_response.body.response << std::endl;
}

}  // namespace srpc

#endif  // SRPC_NET_MESSAGE_MESSAGE_HPP_
