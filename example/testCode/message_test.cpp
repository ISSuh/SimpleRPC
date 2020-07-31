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
  std::cout << strUUID << std::endl;

  srpc::Message serializeMessage(strUUID, srpc::Command::ACCEPT);

  serializeMessage.printPacketforDubugging();
  std::cout << std::endl;

  std::string serializedMessage(serializeMessage.serialize());

  srpc::Message deserializedMsg(serializedMessage);
  deserializedMsg.printPacketforDubugging();

  std::cout << "==================================\n\n";

  srpc::Uuid newUUID_1 = boost::uuids::random_generator()();
  std::string strUUID_1 = to_string(newUUID_1);
  std::cout << strUUID_1 << std::endl;

  srpc::Message serializeMessage_1(strUUID_1, srpc::Command::ACCEPT);

  serializeMessage_1.setServiceName("myServicadafasdfse");
  serializeMessage_1.setRpcName("myRPCasdfadf");
  serializeMessage_1.setParams("{\"params\":\"hello\"}");

  serializeMessage_1.printPacketforDubugging();
  std::cout << std::endl;

  std::string serializedMessage_1(serializeMessage_1.serialize());

  srpc::Message deserializedMsg_1(serializedMessage_1);
  deserializedMsg_1.printPacketforDubugging();
}
