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

  srpc::Message msg;
  msg.setUuid(strUUID);
  msg.setCommand(srpc::Command::ACCEPT);

  msg.printPacketforDubugging();
  std::cout << std::endl;

  const std::string& serializedMessage(msg.serialize());

  stc::cout << "==================================\n";
  std::cout << "serializedMessage size : " << serializedMessage.size() << std::endl;
  std::cout << "serializedMessage len : " << serializedMessage.length() << std::endl;
  stc::cout << "==================================\n";

  srpc::Message deserializedMsg(serializedMessage);
  deserializedMsg.printPacketforDubugging();

}
