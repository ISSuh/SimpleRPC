/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Server Example\n";

  srpc::ServerHandle s(srpc::ProtocolType::TCP, srpc::FunctionType::ASYNC);
  s.setServer(33669);

  s.run();
}
