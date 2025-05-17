#include "src/include/TokenManager.hpp"

TokenManager::TokenManager(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken, int numDevices) 
    : Client(ipAddressNext, port, name, tokenTimeout, hasToken), tokenTime(0), numDevices(numDevices) {

    this->_generateToken();
        
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
        std::this_thread::sleep_for(std::chrono::seconds(1));

        {
            std::lock_guard<std::mutex> lock(tokenMutex); 
            ++tokenTime;
        }
    
        if(tokenTime >= this->numDevices*this->tokenTimeout) { // se exceder o tempo que cada device fica com o token
            std::cout << "Reenviando token" << std::endl;
            _generateToken();
            resetTokenTime();           
        }
    }
    
}

void TokenManager::resetTokenTime() {
    std::lock_guard<std::mutex> lock(tokenMutex); 
    tokenTime = 0; 
}

TokenManager::~TokenManager() {
    stopThread = true;
    if (tokenThread.joinable()) {
        tokenThread.join();
    }
}