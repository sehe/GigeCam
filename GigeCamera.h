#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>

#ifndef GIGECAMERA_H
#define GIGECAMERA_H

namespace gige {
    namespace ba = boost::asio;

    class GigeCamera {

    public:
//        static std::vector<GigeCamera> GetAllCameras();

        GigeCamera(std::string address, ushort port);

        ~GigeCamera();

        std::string getVendor();

    private:
        std::string address_;
        ushort port_;
        std::string vendor;
        ba::io_service service_;
        boost::optional<ba::io_service::work> work_ { service_ };
        ba::strand strand_                          { service_ };
        ba::ip::udp::socket socket_                 { service_, { ba::ip::address::from_string(address_), port_} };
        boost::thread th_                           { boost::bind(&ba::io_service::run, &service_) };
//        boost::shared_ptr<boost::asio::ip::udp::socket> socket;
//        boost::shared_ptr<boost::asio::io_service> service;
        short sequenceNumber = 0;

        /**
         * Control Channel Privilege Register (CCP)
         */
        void privilegeRegister();

        void onPrivilegeRegisterDone(
                const boost::system::error_code &error, // Result of operation.
                std::size_t bytes_transferred,           // Number of bytes sent.
                char *buffer);


        void loadDescriptor();

        void onLoadDescriptorDone(
                const boost::system::error_code &error, // Result of operation.
                std::size_t bytes_transferred,           // Number of bytes sent.
                char *data);

        void onDataRead(
                const boost::system::error_code &error, // Result of operation.
                std::size_t bytes_transferred,           // Number of bytes sent.
                char *data);

        void handler(const boost::system::error_code& error);
    };
}


#endif //GIGECAMERA_H
