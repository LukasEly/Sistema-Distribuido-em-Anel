#include <iostream>
#include <fstream>
#include <string>
#include "include/Protocol.hpp"

SigmaProtocol::SigmaProtocol(int numDevices) : numDevices(numDevices) {

    std::string ipAddressNext;
    int port;
    std::string name;
    int tokenTimeout;
    bool hasToken;

    std::ifstream file("src/settings.conf");  // Nome do arquivo sem extensão

    if (!file) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }

    std::string linha;
    if (std::getline(file, linha)) {
        size_t pos = linha.find(':');
        if (pos != std::string::npos) {
            ipAddressNext = linha.substr(0, pos);
            port = std::stoi(linha.substr(pos + 1));

            std::cout << "IP: " << ipAddressNext << std::endl;
            std::cout << "Porta: " << port << std::endl;
        } else {
            std::cerr << "Formato inválido na linha. Esperado: ip:porta" << std::endl;
        }
    } else {
        std::cerr << "Arquivo vazio ou erro de leitura." << std::endl;
    }

    if (std::getline(file, linha)) {
        name = linha;
        std::cout << "Nome: " << name << std::endl;
    }

    if (std::getline(file, linha)) {
        tokenTimeout = std::stoi(linha);
        std::cout << "Token Timeout: " << tokenTimeout << std::endl;
    }

    if (std::getline(file, linha)) {
        hasToken = (linha == "true");
        std::cout << "Has Token: " << (hasToken ? "true" : "false") << std::endl;
    }

    file.close();

    if(hasToken) {
        client = new TokenManager(ipAddressNext, port, name, tokenTimeout, hasToken, this->numDevices);
    } else {
        client = new Client(ipAddressNext, port, name, tokenTimeout, hasToken);
    }    

    std::cout << " ---------------------------------- cliente ---------------------------------- " << std::endl;
    std::cout << client->toString() << std::endl;
    std::cout << " ------------------------------------------------------------------------------ " << std::endl;
}

SigmaProtocol::~SigmaProtocol() {
    delete client; 
}



