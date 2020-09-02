/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_RPC_STREAM_HPP_
#define SRPC_RPC_STREAM_HPP_

#include <iostream>
#include <vector>

namespace srpc {
namespace utils {

class Stream {
 public:
  Stream() : m_readPos(0) {}

  ~Stream();

 private:
  std::vector<char> m_buf;
  size_t m_readPos;
};

}
}

#endif  // SRPC_RPC_STREAM_HPP_
