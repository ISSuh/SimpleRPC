#include <iostream>

#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Server Example\n";

  srpc::ServerHandle s(33669);
  s.run();
}
