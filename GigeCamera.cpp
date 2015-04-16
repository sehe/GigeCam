#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <bitset>
#include <boost/asio/deadline_timer.hpp>
#include "GigeCamera.h"


gige::GigeCamera::GigeCamera(std::string address, ushort port)
        : address_(address), port_(port) {
    socket_.connect(socket_.local_endpoint());

}

gige::GigeCamera::~GigeCamera() {
    work_.reset();
    if (th_.joinable())
        th_.join(); // perhaps timeout TODO
    service_.stop();
}

/*
std::vector<gige::GigeCamera> gige::GigeCamera::GetAllCameras() {
    std::vector<GigeCamera> allCameras = std::vector<gige::GigeCamera>();
    // TODO discovery
    /**
     * ::device discovery::
     * 255.255.255.255:3956 -> 42:11:00:02:00:00:00:01
     * answer DISCOVERY_CMD = 0x0002 192.168.1.53
     * answer DISCOVERY_ACK = 0x0003 192.168.1.60
     **
    GigeCamera camera {"192.168.1.60", 3956};
    allCameras.push_back(camera);
    return allCameras;
}
*/
void gige::GigeCamera::privilegeRegister() {
    using namespace boost::asio;

//    service = boost::make_shared<io_service>();
//    socket  = boost::make_shared<ip::udp::socket>(*service);
//    socket->connect({ip::address_::from_string(address_), port_ });

//    service = boost::make_shared<io_service>();
//    socket  = boost::make_shared<ip::udp::socket>(service);
//    socket->connect({ip::address_::from_string(address_), port_ });

    // Control Channel Privilege Register (CCP)
    boost::array<uint8_t, 16> sendBuffer = {{ 0x42, 0x01, 0x00, 0x82, 0x00, 0x08, 0x00,
                                              0x01, 0x00, 0x00, 0x0a, 0x00, 0x00,
                                              0x00, 0x00, 0x02 }};
    socket_.send(boost::asio::buffer(sendBuffer));
    char buffer[100];
    socket_.async_receive(boost::asio::buffer(buffer), boost::bind(&gige::GigeCamera::onPrivilegeRegisterDone,
                                                                  this, boost::asio::placeholders::error,
                                                                  boost::asio::placeholders::bytes_transferred,
                                                                  buffer));

    service_.run();
    std::cout << "first returned" << std::endl;
}

void gige::GigeCamera::onPrivilegeRegisterDone(const boost::system::error_code &error, std::size_t bytes_transferred,
                                               char *buffer) {
    if(error) {
        std::cout << error.message() << std::endl;
    } else {
        std::cout << "onPrivilegeRegisterDone, numOfReceivedBytes:" << bytes_transferred << std::endl;
        int payloadSize = buffer[4] << 8 |buffer[5] ;
        int result = buffer[payloadSize + 7];
        std::cout << "result(1-OK, 0-error): " << result << std::endl;
    }

}

std::string gige::GigeCamera::getVendor() {
    if(vendor.empty()) {
        loadDescriptor();
    }
    return vendor;
}

void gige::GigeCamera::loadDescriptor() {
    privilegeRegister();

    using namespace boost::asio;
//    service = boost::make_shared<io_service>();
//    socket  = boost::make_shared<ip::udp::socket>(*service);
//    socket->connect({ip::address_::from_string(address_), port_ });
    //
    boost::array<uint8_t, 16> sendBuffer = {{ 0x42, 0x01, 0x00, 0x84, 0x00, 0x08, 0x00, 0x0c, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00}};

    socket_.send(boost::asio::buffer(sendBuffer));
    char receiveBuffer[800];
    socket_.async_receive(boost::asio::buffer(receiveBuffer), boost::bind(&gige::GigeCamera::onLoadDescriptorDone,
                                                                          this, placeholders::error,
                                                                          placeholders::bytes_transferred,
                                                                          receiveBuffer));

    try {
//        boost::asio::io_service::work work(*service);
//        boost::asio::deadline_timer timer(*service, boost::posix_time::seconds(5));
//        timer.async_wait(&gige::GigeCamera::handler);
//        service->reset();
//        service->run();
        std::cout << "second returned" << std::endl;
    } catch (std::exception& exception) {
        std::cout<< "exception" << exception.what() << std::endl;
    }

}


void gige::GigeCamera::onLoadDescriptorDone(const boost::system::error_code &error, std::size_t bytes_transferred,
                                         char *data) {

    std::cout << "onLoadDescriptorDone" << std::endl;
    if(error) {
        std::cout << error.message() << std::endl;
    }

    std::string dataWithURL(data, bytes_transferred);
    std::size_t localPos = dataWithURL.find("Local:");
    std::string afterLocal = dataWithURL.substr(localPos +6);

    std::vector<std::string> splits;
    boost::split(splits, afterLocal, boost::is_any_of(";"));
    std::string fileName = splits[0];
    std::string addr = splits[1];
    std::string length = splits[2];
    std::cout << "file:" << fileName << std::endl << "addr:" << addr << std::endl << "length:" << length << std::endl;
    //read memory at address_ 21bc0000
//  42 01 00 84 00 08 00 23 21 bc 00 00 00 00 02 00
    short seqNumb = sequenceNumber++;
    seqNumb++;
    seqNumb++;
    uint8_t firstPart = 8 >> seqNumb;
    uint8_t secondPart = seqNumb;
    boost::array<uint8_t, 16> sendBuffer = {{ 0x42, 0x01, 0x00, 0x84, 0x00, 0x08, firstPart, secondPart,
                                              0x21, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00}};

    socket_.send(boost::asio::buffer(sendBuffer));
    char receiveBuffer[100];
    socket_.async_receive(boost::asio::buffer(receiveBuffer), boost::bind(&gige::GigeCamera::onDataRead,
                                                                          this, boost::asio::placeholders::error,
                                                                          boost::asio::placeholders::bytes_transferred,
                                                                          receiveBuffer));

}


void gige::GigeCamera::onDataRead(const boost::system::error_code &error, std::size_t bytes_transferred,
                                char *data) {
    std::cout<<"onDataRead";
}

void gige::GigeCamera::handler(const boost::system::error_code& error) {
    std::cout<< "handler" <<std::endl;
}
