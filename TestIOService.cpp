#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include "TestIOService.h"

void TestIOService::makeConnection() {
//    boost::asio::io_service service;
    boost::asio::ip::udp::socket socket(service);
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.1.2"), 1234);
    socket.connect(endpoint);
    socket.close();
}

TestIOService::TestIOService(std::string address) : address(address) { }

TestIOService TestIOService::getInst() {
    return TestIOService("192.168.1.2");
}
