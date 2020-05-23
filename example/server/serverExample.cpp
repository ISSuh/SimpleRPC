#include <iostream>

#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Server Example\n";
  
  try {
    srpc::ServerHandle server(33669);
    server.run();
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}