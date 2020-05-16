#include <iostream>

#include <boost/asio.hpp>

class Mock {
 public:
    Mock() {
        std::cout << "Mock" << std::endl;
    }
};