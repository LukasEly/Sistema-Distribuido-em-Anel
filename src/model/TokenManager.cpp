#include "src/include/TokenManager.hpp"

TokenManager::TokenManager(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken, int numDevices) 
    : Client(ipAddressNext, port, name, tokenTimeout, hasToken), tokenTime(0) {
        
    this->numDevices = numDevices; 
}

TokenManager::~TokenManager() {
}