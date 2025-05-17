#include "src/include/Client.hpp"

Client::Client(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken) 
    : ipAddressNext(ipAddressNext), port(port), name(name), tokenTimeout(tokenTimeout), hasToken(hasToken) {
    // Inicializa o cliente com os parâmetros fornecidos
}

std::string Client::toString() const {
    std::string result = "IP: " + ipAddressNext + "\n";
    result += "Porta: " + std::to_string(port) + "\n";
    result += "Nome: " + name + "\n";
    result += "Token Timeout: " + std::to_string(tokenTimeout) + "\n";
    result += "Has Token: " + std::string(hasToken ? "true" : "false") + "\n";
    return result;
}

std::string Client::getName() const {
    return name;
}

Client::~Client() {
    messageQueue.clear(); // não precisava, mas vai ser feito de qualquer forma, então tanto faz
}
