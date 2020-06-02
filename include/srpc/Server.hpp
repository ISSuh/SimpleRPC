/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_SERVER_HPP
#define SRPC_SERVER_HPP

class Server {
 public:
  virtual ~Server() = default;

 protected:
  Server() = default;
  
  virtual void accept() = 0;
};

#endif // SRPC_SERVER_HPP
