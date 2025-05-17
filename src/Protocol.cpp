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

            printf("IP: %s\n", ipAddressNext.c_str());
            printf("Porta: %d\n", port);
        } else {
            std::cerr << "Formato inválido na linha. Esperado: ip:porta" << std::endl;
        }
    } else {
        std::cerr << "Arquivo vazio ou erro de leitura." << std::endl;
    }

    if (std::getline(file, linha)) {
        name = linha;
        printf("Nome: %s\n", name.c_str());
    }

    if (std::getline(file, linha)) {
        tokenTimeout = std::stoi(linha);
        printf("Token Timeout: %d\n", tokenTimeout);
    }

    if (std::getline(file, linha)) {
        hasToken = (linha == "true");
        printf("Has Token: %s\n", (hasToken ? "true" : "false"));
    }

    file.close();

    if(hasToken) {
        client = new TokenManager(ipAddressNext, port, name, tokenTimeout, hasToken, this->numDevices);
    } else {
        client = new Client(ipAddressNext, port, name, tokenTimeout, hasToken);
    }    

    printf(" ---------------------------------- cliente ---------------------------------- \n");
    printf("%s\n" ,client->toString().c_str());
    printf(" ----------------------------------------------------------------------------- \n");
}


void SigmaProtocol::start() {

    Header* header = new Header("naoexiste", client->getName(), "ALL");
    Packet* packet = new Packet(9000, header, "ola");

    std::vector<char> buffer;

    packet->serialize(buffer);

    printf("Data: %.*s\n", (int)buffer.size(), buffer.data());

    delete packet;
}

SigmaProtocol::~SigmaProtocol() {
    delete client; 
}



