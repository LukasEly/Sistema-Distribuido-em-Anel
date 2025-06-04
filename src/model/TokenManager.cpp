#include "src/include/TokenManager.hpp"

TokenManager::TokenManager(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken, int numDevices) 
    : Client(ipAddressNext, port, name, tokenTimeout, hasToken), tokenTime(0), numDevices(numDevices) {

    this->_generateToken();
    
    std::cout << Debug::verde("Token iniciado") << std::endl;

    stopThread = false;
    tokenThread = std::thread(&TokenManager::_manageTokenTime, this); 
}

void TokenManager::_generateToken() {
    Packet* tokenPacket = new Packet(9000, nullptr, ""); 

    this->_sendPacket(tokenPacket); 

    delete tokenPacket;  
}

void TokenManager::_manageTokenTime() {

    while (!stopThread)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        {
            std::lock_guard<std::mutex> lock(tokenMutex); 
            tokenTime += 0.01;
        }
    
        if(tokenTime >= this->numDevices*this->tokenTimeout+0.5) { // se exceder o tempo que cada device fica com o token
            std::cout << "[DEBUG] Tempo do Token Excedido: " << tokenTime << " segundos" << std::endl;
            std::cout << "Reenviando token" << std::endl;
            _generateToken();
            resetTokenTime();
        }
    }
}

void TokenManager::resetTokenTime() {
    std::lock_guard<std::mutex> lock(tokenMutex); 
    std::cout << "[DEBUG] Resetando Tempo do Token \n";
    tokenTime = 0; 
}

void TokenManager::evaluateTokenTime() {
    std::lock_guard<std::mutex> lock(tokenMutex); 
    if(tokenTime < this->numDevices*this->tokenTimeout-0.5) { // se chegou com mais de meio segundo antes do tempo limite
        std::cout << "[DEBUG] Token Chegou antes do Tempo \n" << std::endl;
        this->removeToken();
    }

    // se ta tudo certo, reseta o tempo do token
    tokenTime = 0; // Reseta o tempo do token após a avaliação
}

TokenManager::~TokenManager() {
    stopThread = true;
    if (tokenThread.joinable()) {
        tokenThread.join();
    }
}