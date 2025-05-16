#ifndef TOKENMANAGER_HPP
#define TOKENMANAGER_HPP

#include <mutex>
#include "Client.hpp"

class TokenManager : public Client {

    private:

        std::mutex tokenMutex; // como tem threads, melhor limitar o acesso da escrita do token
        int tokenTime;

        int numDevices;

        // essas funções só ficam no construtor
        void _manageTokenTime();
        void _generateToken();

    public:

        TokenManager(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken, int numDevices);
        ~TokenManager() override;

        bool receivedToken(); // só sinaliza que recebeu o token para resetar o tempo, ou tratar o token
        Packet createTokenPacket(); // cria o pacote do token
};

#endif