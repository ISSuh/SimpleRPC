/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>
#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Message Test\n";

  srpc::Uuid newUUID = boost::uuids::random_generator()();
  std::string strUUID = to_string(newUUID);

  srpc::Message serializeMessage;
  serializeMessage.setUuid(strUUID);
  serializeMessage.setCommand(srpc::Command::ACCEPT);

  serializeMessage.printPacketforDubugging();
  std::cout << std::endl;

  std::vector<char> serializedMessage(serializeMessage.serialize());
  std::cout << serializedMessage.size() << std::endl;

  srpc::Message deserializedMsg(serializedMessage);
  deserializedMsg.printPacketforDubugging();

  std::cout << "==================================\n\n";

  srpc::Uuid newUUID_1 = boost::uuids::random_generator()();
  std::string strUUID_1 = to_string(newUUID_1);

  srpc::Message serializeMessage_1;
  serializeMessage_1.setUuid(strUUID_1);
  serializeMessage_1.setCommand(srpc::Command::ACCEPT);

  serializeMessage_1.setServiceName("myService");
  serializeMessage_1.setRpcName("myRPC");
  serializeMessage_1.setParams("{\"params\":\"hello\"}");

  serializeMessage_1.printPacketforDubugging();
  std::cout << std::endl;

  std::vector<char> serializedMessage_1(serializeMessage_1.serialize());
  std::cout << serializedMessage_1.size() << std::endl;

  srpc::Message deserializedMsg_1(serializedMessage_1);
  deserializedMsg_1.printPacketforDubugging();
}
