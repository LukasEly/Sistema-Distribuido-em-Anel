#ifndef SIGMA_PROTOCOL_HPP
#define SIGMA_PROTOCOL_HPP

#include <iostream>
#include "Utils.hpp"
#include "Client.hpp"
#include "TokenManager.hpp"
#include "Console.hpp"

class SigmaProtocol {

    private: 

        int numDevices;
        Client* client;
        Console console;

        std::thread listenerThread;
        std::atomic<bool> running;

        void SigmaProtocol::_listenForPackets();

    public:
        SigmaProtocol(int numDevices);
        ~SigmaProtocol();

        void start();

        int getIpAddress() const;
        int getPort() const;
        int getTokenTimeout() const;
        bool hasTokenFlag() const;
        const std::string& getName() const;
};

#endif // SIGMA_PROTOCOL_HPP
