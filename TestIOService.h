#ifndef OPENIPCAM_TESTIOSERVICE_H
#define OPENIPCAM_TESTIOSERVICE_H

    class TestIOService {

    public:
        void makeConnection();
        static TestIOService getInst();

    private:
        TestIOService(std::string address);
        std::string address;
        boost::asio::io_service service;
    };


#endif //OPENIPCAM_TESTIOSERVICE_H
