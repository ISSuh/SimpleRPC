/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

#include <srpc.hpp>

int main(int argc, char* argv[]) {
  std::cout << "SimpleRPC Client Example\n";

  srpc::ClientHandle c("127.0.0.1", "33669");
  c.connect();

  for (auto i = 0 ; i < 10 ; ++i) {
    c.request(std::to_string(i));
    sleep(1);
  }

  c.terminate();
}
