#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <fstream>
#include <list>

#include "Packet.hpp"

class Client {

    private:

        std::string ipAddressNext;
        int port;
        std::string name;
        int tokenTimeout;
        bool hasToken;
        int numDevices;

        std::list<Packet> messageQueue; // melhor usar packet ou string?

    public:

        Client(int numDevices);
        ~Client();

        bool enqueueMessage(std::string message); // já passa como packet ou como string?
        std::string dequeueMessage();

        void handleMessage(const Packet* packet); // passar como packet é melhor? ou passar como string?
        void handleToken(const Packet* packet);
        void handleAck(const Packet* packet);
        void handleNack(const Packet* packet);
        void handleNotExist(const Packet* packet);
};


#endif