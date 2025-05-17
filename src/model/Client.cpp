#include "src/include/Client.hpp"

Client::Client(std::string ipAddressNext, int port, std::string name, int tokenTimeout, bool hasToken) 
    : ipAddressNext(ipAddressNext), port(port), name(name), tokenTimeout(tokenTimeout), hasToken(hasToken) {

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Erro ao criar socket!" << std::endl;
        throw std::runtime_error("Erro ao criar socket");
    }

    // Configurando o endereço do servidor
    struct sockaddr_in end;
    std::memset(&end, 0, sizeof(end));
    end.sin_family = AF_INET;
    end.sin_addr.s_addr = INADDR_ANY;  // Aceita qualquer endereço
    end.sin_port = htons(LOCALPORT);  // Porta de escuta

    // permitindo reusar a socket pra ser menos estressante na hora de testar
    int opt = 1;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    } 

    if (bind(this->clientSocket, (struct sockaddr*)&end, sizeof(end)) < 0) {
        std::cerr << "Erro ao fazer bind!" << std::endl;
        throw std::runtime_error("Erro ao fazer bind");
    }   
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

void Client::_sendPacket(Packet* packet) {
    // Implementar o envio do pacote
    // Aqui você pode usar sockets ou qualquer outro método de comunicação
    std::cout << "Enviando pacote: " << packet->toString() << std::endl;
}



Client::~Client() {
    messageQueue.clear(); // não precisava, mas vai ser feito de qualquer forma, então tanto faz
}
