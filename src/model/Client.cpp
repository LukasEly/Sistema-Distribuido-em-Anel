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

    std::memset(&dst, 0, sizeof(dst));
    dst.sin_family = AF_INET;
    dst.sin_port = htons(this->port); // porta de destino
    inet_pton(AF_INET, ipAddressNext.c_str(), &dst.sin_addr);

    _removeToken = false;
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

    std::cout << "Enviando pacote: " << packet->toString() << std::endl;

    std::vector<char> buffer;
    packet->serialize(buffer);

    sendto(clientSocket, buffer.data(), buffer.size(), 0, (struct sockaddr*)&dst, sizeof(dst));
}

//boooooooooooooooosta


void Client::receive() {
    receiveThread = std::thread(&Client::receive, this);
}

void Client::_select() {
    fd_set readfds;
    struct timeval tv;
    
    while (!_stopReceiveThread) {
        FD_ZERO(&readfds);
        FD_SET(clientSocket, &readfds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int activity = select(clientSocket + 1, &readfds, NULL, NULL, &tv);

        if (activity < 0) {
            std::cerr << "Erro no select" << std::endl;
            continue;
        }

        if (activity == 0) {
            continue;
        }

        if (FD_ISSET(clientSocket, &readfds)) {
            char buffer[1024];
            struct sockaddr_in srcAddr;
            socklen_t addrLen = sizeof(srcAddr);

            ssize_t bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&srcAddr, &addrLen);

            if (bytesReceived < 0) {
                std::cerr << "Erro ao receber dados" << std::endl;
                continue;
            }

            // Aqui você faz parsing do pacote
            /*Packet packet;
            if (packet.deserialize(buffer, bytesReceived)) {
                std::cout << "Pacote recebido: " << packet.toString() << std::endl;

                // Processa o pacote recebido
                _handlePacket(packet);
            } else {
                std::cerr << "Falha na desserialização do pacote" << std::endl;
            }*/
        }
    }
}

void Client::stopReceiveThread() {
    _stopReceiveThread = true;
    if (receiveThread.joinable()) {
        receiveThread.join();
    }
}


// booooooooooooooooosta



bool Client::enqueueMessage(std::string destination, std::string message) {
    if (messageQueue.size() >= MAX_DATA) { // número máximo de mensagens
        return false;
    }

    Header* header = new Header("naoexiste", this->name, destination);
    Packet* packet = new Packet(7777, header, message);
    messageQueue.push_back(packet);

    // debug dos pacotes salvos se precisar
    /*
    std::cout << "----------------- imprimindo pacotes -----------------" << std::endl;
    for (auto& packet : messageQueue) {
        std::cout << "Pacote enfileirado: " << packet->toString() << std::endl;
    }
    */

    return true;
}

void Client::removeToken() {
    if(hasToken) {
        hasToken = false;
        return;
    }
    _removeToken = true; // tem que tratar o token no recebimento
}

void Client::addToken() {

    Packet* packet = new Packet(9000, nullptr, "");
    this->_sendPacket(packet);
    delete packet;
}

void Client::setPacketError(int percent) {
    if (percent < 0 || percent > 100) {
        std::cerr << "Erro: porcentagem de erro deve estar entre 0 e 100." << std::endl;
        return;
    }
    this->_packetError = percent;
}

Client::~Client() {
    messageQueue.clear(); // não precisava, mas vai ser feito de qualquer forma, então tanto faz
}
