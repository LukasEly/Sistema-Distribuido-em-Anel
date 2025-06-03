#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <fstream>
#include <list>
#include <arpa/inet.h>
#include <sys/select.h>

#include "src/include/Debug.hpp"
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
        void dequeueMessage();

        virtual void resetTokenTime() {} 

        // bool isCrcOk(const Packet* packet); // já ta definido em packet
        
        // funções de falhas
        void removeToken();
        void sendToken();
        void setPacketError(int percent);

        bool hasTokenFlag() const { return hasToken; }
        int getTokenTimeout() const { return tokenTimeout; }
        int getPort() const { return port; }

        void handleMessage(Packet* packet);
        void handleToken(const Packet* packet);
        void handleAck(const Packet* packet);
        void handleNack(const Packet* packet);
        void handleNotExist(const Packet* packet);

        std::string toString() const;
};


#endif