#include <iostream>

#include <boost/asio.hpp>

class Mock {
    Mock() {
        std::cout << "Mock" << std::end;
    }
};