#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <bitset>
#include "GigeCamera.h"
/*
void handler(
    const boost::system::error_code& error, // Result of operation.
    std::size_t bytes_transferred,           // Number of bytes sent.
    char* recv_buffer
) {
    if(error) {
        std::cout << error.message() << std::endl;
    }

    std::cout << "data read: " << bytes_transferred << std::endl;
    for(int i = 0; i<=bytes_transferred; i++) {
        std::cout << (std::bitset<8>) recv_buffer[i] << std::endl;
    }
}

void onConfigURLRead(
    const boost::system::error_code& error, // Result of operation.
    std::size_t bytes_transferred,           // Number of bytes sent.
    char* data,
    boost::asio::ip::udp::socket* m_sock
) {
    if(error) {
        std::cout << error.message() << std::endl;
    }

    std::cout << "data read: " << bytes_transferred << std::endl;
    std::string dataWithURL(data, bytes_transferred);
    std::cout << "data: " << dataWithURL << std::endl;
    std::size_t localPos = dataWithURL.find("Local:");
    std::cout << "localPos: " << localPos << std::endl;
    std::string afterLocal = dataWithURL.substr(localPos +6);
    std::cout << "afterLocal"<< afterLocal <<std::endl;

    std::vector<std::string> splits;
    boost::split(splits, afterLocal, boost::is_any_of(";"));
    std::string fileName = splits[0];
//  42 01 00 84 00 08 00 23 21 bc 00 00 00 00 02 00
    boost::array<uint8_t, 16> buff = {{ 0x42, 0x01, 0x00, 0x84, 0x00, 0x08, 0x00, 0x23, 0x21, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

}
*/
int main(void)
{
//    std::vector<gige::GigeCamera> allCameras = gige::GigeCamera::GetAllCameras();
    gige::GigeCamera camera {"192.168.1.60", 3956};

    std::string vendor = camera.getVendor();
    std::cout << vendor;

    /*
    gige::GigeCamera cam = gige::GigeCamera("192.168.1.60", 3956 );
    cam.getVendor();
    /*
    boost::asio::io_service m_service;
    boost::asio::ip::udp::resolver resolver(m_service);

    const std::string& sAddr = "192.168.1.60";
    //const std::string& sAddr = "255.255.255.255";
    const std::string& sPort = "3956";

    boost::asio::ip::udp::resolver::query basic_query(boost::asio::ip::udp::v4(), sAddr, sPort);
    boost::asio::ip::udp::resolver::iterator iterator = resolver.resolve(basic_query);

    boost::asio::ip::udp::socket m_sock(m_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));

    // Control Channel Privilege Register (CCP)
    boost::array<uint8_t, 16> buff = {{ 0x42, 0x01, 0x00, 0x82, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x02}};
    char recv_buffer[32];
    m_sock.async_send_to(boost::asio::buffer(buff), *iterator, boost::bind(handler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, recv_buffer));

    char recv_buffer1[32];
    m_sock.async_receive(boost::asio::buffer(recv_buffer1), boost::bind(&handler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, recv_buffer1));

    //42 01 00 84 00 08 00 0c 00 00 02 00 00 00 02 00
    boost::array<uint8_t, 16> requestURL = {{ 0x42, 0x01, 0x00, 0x84, 0x00, 0x08, 0x00, 0x0c, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00}};
    m_sock.async_send_to(boost::asio::buffer(requestURL), *iterator, boost::bind(&handler, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, recv_buffer));

    char recv_buffer2[512];
    m_sock.async_receive(boost::asio::buffer(recv_buffer2),
        boost::bind(&onConfigURLRead, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, recv_buffer2, &m_sock));

    m_service.run();*/
    return 0;
}







