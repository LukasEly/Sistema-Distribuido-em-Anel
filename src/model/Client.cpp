#include "src/include/Client.hpp"

Client::Client(int numDevices) : numDevices(numDevices) {

    std::ifstream file("src/settings.conf");  // Nome do arquivo sem extensão

    if (!file) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }

    std::string linha;
    if (std::getline(file, linha)) {
        size_t pos = linha.find(':');
        if (pos != std::string::npos) {
            this->ipAddressNext = linha.substr(0, pos);
            this->port = std::stoi(linha.substr(pos + 1));

            std::cout << "IP: " << ipAddressNext << std::endl;
            std::cout << "Porta: " << port << std::endl;
        } else {
            std::cerr << "Formato inválido na linha. Esperado: ip:porta" << std::endl;
        }
    } else {
        std::cerr << "Arquivo vazio ou erro de leitura." << std::endl;
    }

    if (std::getline(file, linha)) {
        this->name = linha;
        std::cout << "Nome: " << name << std::endl;
    }

    if (std::getline(file, linha)) {
        this->tokenTimeout = std::stoi(linha);
        std::cout << "Token Timeout: " << tokenTimeout << std::endl;
    }

    if (std::getline(file, linha)) {
        this->hasToken = (linha == "true");
        std::cout << "Has Token: " << (hasToken ? "true" : "false") << std::endl;
    }

    file.close();
}