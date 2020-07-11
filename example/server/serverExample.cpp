/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

#include <srpc.hpp>

void Hello(const std::string& arg) {
  std::cout << "Hello " << arg << std::endl;
}

int sum(int a, int b) {
  return a+b;
}

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Server Example\n";

  srpc::ServerHandle s(srpc::ProtocolType::TCP, srpc::FunctionType::ASYNC);
  s.setServer(33669);

  // ExampleUserService rpcService;
  // rpcService.addRPCFunction<void>("Hello", Hello);
  // rpcService.addRPCFunction<int>("sum", sum);

  // s.addRPCService(rpcService);

  s.run();
}
