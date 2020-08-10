/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>
#include <vector>

#include <srpc/net/message/Message.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Message Test\n";

  srpc::Message sm1("00000011-1100-0000-0000-000001100000",
                                 srpc::Command::ACCEPT);

  sm1.printPacketHeader();
  std::cout << std::endl;

  std::string smStr1(sm1.serialize());

  srpc::Message dm1(smStr1);
  dm1.printPacketHeader();

  std::cout << "==============================================\n";

  srpc::Message sm2("00000011-1100-0000-0000-000001100000",
                    srpc::Command::REQUEST);

  sm2.setServiceName("myService");
  sm2.setRpcName("myRpc");
  sm2.setParams("myParam");

  sm2.printPacketHeader();
  sm2.printRequestPacketHeader();

  std::cout << std::endl;

  std::string smStr2(sm2.serialize());

  srpc::Message dm2(smStr2);
  dm2.printPacketHeader();
  dm2.printRequestPacketHeader();

  std::cout << "==============================================\n";

  srpc::Message sm3("00000011-1100-0000-0000-000001100000",
                    srpc::Command::REPONSE);

  double tt[5];
  for (auto i = 0 ; i < 5 ; ++i) {
    tt[i] = i * 2.3;
  }

  sm3.setResponse(tt);

  sm3.printPacketHeader();
  sm3.printReponsePacketHeader();

  std::cout << std::endl;

  std::string smStr3(sm3.serialize());

  srpc::Message dm3(smStr3);
  dm3.printPacketHeader();
  dm3.printReponsePacketHeader();

  double res[5];
  dm3.getResponse(res);

  for (auto& i : res) {
    std::cout << "Val : " << i << std::endl;
  }
}
