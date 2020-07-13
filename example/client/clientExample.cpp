/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Client Example\n";

  srpc::ClientHandle c(srpc::ProtocolType::TCP, srpc::FunctionType::ASYNC);
  c.connect("127.0.0.1", "33669");

  int i = 0;
  while (1) {
    c.request(std::to_string(i));
    sleep(1);
  }


  // c.request("add", )

  c.terminate();
}
