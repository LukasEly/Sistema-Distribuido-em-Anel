#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <fstream>

class Client {

    private:

        std::string ipAddressNext;
        int port;
        std::string name;
        int tokenTimeout;
        bool hasToken;
        int numDevices;

    public:

        Client(int numDevices);
        ~Client();

};


#endif