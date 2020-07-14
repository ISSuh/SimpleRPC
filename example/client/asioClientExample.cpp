/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>

#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<functional>

using boost::asio::ip::tcp;
using namespace std;

class client {
 public:
  client(boost::asio::io_service& io_context, const string& host, const string& port) : socket_(io_context) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    boost::asio::async_connect(socket_, endpoint_iterator,
      std::bind(&client::handle_connect, this, std::placeholders::_1));
  }

  void handle_connect(const boost::system::error_code& e) {
    if (!e) {
      cout << "Connected!" << endl;
      string msg = "Hello! Server!";

      boost::asio::async_write(socket_, boost::asio::buffer(msg, msg.length()),
          std::bind(&client::handle_write, this, std::placeholders::_1));
    }
  }

  void handle_write(const boost::system::error_code& e) {
    if (!e) {
      cout << "Done!" << endl;
    }
  }

 private :
  tcp::socket socket_;
  char data_[1024];
};

int main(int argc, char* argv[]) {
  std::cout << "ASIO Client Example\n";

  boost::asio::io_service io_context;
  client c(io_context, "127.0.0.1", "33669");
  io_context.run();
}
