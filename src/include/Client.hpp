#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <fstream>
#include <list>
#include <arpa/inet.h>
#include <sys/select.h>

#include "Packet.hpp"

#define LOCALPORT 6000
#define MAX_DATA 10

class Client {

    protected:

        std::string ipAddressNext;
        int port;
        std::string name;
        int tokenTimeout;
        bool hasToken;

        int clientSocket;
        struct sockaddr_in dst;

        std::list<Packet*> messageQueue; // melhor usar packet ou string?

        // variáveis de falhas
        bool _removeToken;
        int _packetError; 

        void _sendPacket(Packet* packet); // envia o pacote


    public:
    
        Client(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken);
        virtual ~Client(); // precisa ser virtual pra poder identificar o tipo do objeto, e também pra "ativar" o polimorfismo

        std::string getName() const;

        bool enqueueMessage(std::string destination, std::string message); // já passa como packet ou como string?
        std::string dequeueMessage();

        virtual void resetTokenTime() {} // No Client

        // funções de falhas
        void removeToken();
        void addToken();
        void setPacketError(int percent);

        bool hasTokenFlag() const { return hasToken; }

        void handleMessage();
        void handleToken(const Packet* packet);
        void handleAck(const Packet* packet);
        void handleNack(const Packet* packet);
        void handleNotExist(const Packet* packet);

        std::string toString() const;
};


#endif