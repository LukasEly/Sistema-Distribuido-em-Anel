#include "src/include/Client.hpp"
#include "src/include/Debug.hpp"

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
    std::cout << Debug::amarelo("Enviando pacote: ") << packet->toString() << std::endl;

    std::vector<char> buffer;
    packet->serialize(buffer);

    sendto(clientSocket, buffer.data(), buffer.size(), 0, (struct sockaddr*)&dst, sizeof(dst));
}

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

void Client::sendToken() {
    if(this->hasToken) { // só manda o token se tiver com ele
        Packet* packet = new Packet(9000, nullptr, "");
        this->_sendPacket(packet);
        delete packet;
    }
}

void Client::setPacketError(int percent) {
    if (percent < 0 || percent > 100) {
        std::cerr << Debug::erro("porcentagem de erro deve estar entre 0 e 100.") << std::endl;
        return;
    }
    this->_packetError = percent;
}

void Client::handleMessage(Packet* packet) {
    std::cout << Debug::azul("Pacote é uma mensagem ") << std::endl;
    
    // --------------------------------------- IMPORTANTE LER ISSO AQUI SE EU ME ESQUECI DE FALAR

    resetTokenTime(); // precisa testar, mas com os testes que eu tinha feito antes, tava gerando muita duplicação de pacote, por causa do tempo que o ack levava pra chegar

    // ---------------------------------------- LUKAS CONKA, LÊ O COMENTÁRIO E VÊ SE CONCORDA COMIGO

    if (packet->getDestino() == this->name) {

        if(packet->getEstado() == "naoexiste") {
            std::cout << Debug::verde("Mensagem recebida com sucesso: ") << packet->toString() << std::endl;
            handleNotExist(packet);
        } else if(packet->getEstado() == "ACK") {
            std::cout << Debug::verde("Mensagem recebida com sucesso: ") << packet->toString() << std::endl;
            handleAck(packet);
        } else if(packet->getEstado() == "NACK") {
            std::cout << Debug::vermelho("Mensagem com algum problema NACK: ") << packet->toString() << std::endl;
            handleNack(packet);
        } else {
            std::cout << Debug::erro("Mensagem recebida, mas controle de ERRO inconsistente ") << packet->getEstado() << std::endl;
        }
    } else if((packet->getOrigem() == this->name && packet->getDestino() != "TODOS") ) {
        std::cout << Debug::erro("Mensagem não enviada, destino inválido: ") << packet->getDestino() << std::endl;
        this->dequeueMessage(); 
        sendToken();
        return;
    } else if (packet->getDestino() == "TODOS" && packet->getOrigem() != this->name) {
        std::cout << Debug::ciano("Mensagem recebida para TODOS: ") << packet->toString() << std::endl;
        // acho que não faz nada, não faz sentido mandar ack, senão vai flodar tudo, então só passa adiante
        _sendPacket(packet); // envia o pacote para todos os outros clientes
    } else if (packet->getDestino() == "TODOS" && packet->getOrigem() == this->name) {
        std::cout << Debug::azul("Finalizando ciclo de mensagem enviada para TODOS: ") << packet->toString() << std::endl;
        // não faz mais nada, termina aqui
    } else {
        std::cout << Debug::vermelho("Mensagem não é para mim, ignorando.") << std::endl;
        _sendPacket(packet);
    }
} 

void Client::handleToken(const Packet* packet) {
    std::cout << Debug::magenta("Pacote é um Token.") << std::endl;
    resetTokenTime(); // acho que talvez seja interessante resetar o tempo do token toda vez que recebe uma menagem, porque tem mais o tempo do ack,
                      // então o token manager só analisa de ainda ta circulando mensagens no anel, então eu vou colocar essa linha quando receber uma mensagem
    if (messageQueue.empty()) {
        std::cout << Debug::vermelho("Pacote não possui mensagens.") << std::endl;
        sendToken();
    } else {
        std::cout << Debug::amarelo("Pacote possui mensagens.") << std::endl;
        Packet* msgPacket = messageQueue.front();
        _sendPacket(msgPacket);
        std::cout << Debug::verde("Pacote enviado: ") << msgPacket->toString() << std::endl;

        
        // no caso onde o destino é "TODOS", não faz sentido enviar ACK ou NACK, pois é um broadcast
        if(msgPacket->getDestino() == "TODOS") {
            this->dequeueMessage();
            sendToken();
        }
    }
} 

void Client::handleNack(const Packet* packet) {
    /* Não da pra reutilizar o pacote que ta corrompido né mano ;-;

    Header* header = new Header("naoexiste", this->name, packet->getOrigem());
    Packet* msgPacket = new Packet(7777, header, packet->getPayload());
    _sendPacket(msgPacket);
    delete msgPacket;
    */

    if(messageQueue.empty()) {
        std::cout << Debug::vermelho("Fila de mensagens está vazia, não há o que reenviar.") << std::endl;
        return;
    } else {
        std::cout << Debug::amarelo("Reenviando mensagem corrompida: ") << packet->toString() << std::endl;
        Packet* msgPacket = messageQueue.front();
        _sendPacket(msgPacket);
        std::cout << Debug::verde("Pacote reenviado: ") << msgPacket->toString() << std::endl;
    }
}

void Client::handleAck(const Packet* packet) {
    this->dequeueMessage(); 
    sendToken();
}

void Client::dequeueMessage() {
    if (!messageQueue.empty()) {
        Packet* packet = messageQueue.front();
        messageQueue.pop_front();
        delete packet;  // libera a memória do pacote
    } else {
        std::cout << Debug::vermelho("Fila de mensagens está vazia.") << std::endl;
    }
}

void Client::handleNotExist(const Packet* packet) {
    
    if(packet->isCrcOk()) {

        Header* header = new Header("ACK", this->name, packet->getOrigem());
        Packet* msgPacket = new Packet(7777, header, packet->getPayload());

        _sendPacket(msgPacket);
        std::cout << Debug::verde("Pacote enviado com ACK: ") << msgPacket->toString() << std::endl;

    } else {
        Header* header = new Header("NACK", this->name, packet->getOrigem());
        Packet* msgPacket = new Packet(7777, header, packet->getPayload());

        _sendPacket(msgPacket);
        std::cout << Debug::erro("Pacote enviado com NACK: ") << msgPacket->toString() << std::endl;
    }

}

Client::~Client() {
    messageQueue.clear(); // não precisava, mas vai ser feito de qualquer forma, então tanto faz
}
