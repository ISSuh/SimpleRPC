#include <iostream>

#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Client Example\n";

  srpc::ClientHandle s("127.0.0.1", "33669");
  s.run();
}
