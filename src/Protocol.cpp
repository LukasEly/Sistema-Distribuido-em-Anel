#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
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
        } else {
            std::cerr << "Formato inválido na linha. Esperado: ip:porta" << std::endl;
        }
    } else {
        std::cerr << "Arquivo vazio ou erro de leitura." << std::endl;
    }

    if (std::getline(file, linha)) {
        name = linha;
    }

    if (std::getline(file, linha)) {
        tokenTimeout = std::stoi(linha);
    }

    if (std::getline(file, linha)) {
        hasToken = (linha == "true");
    }

    file.close();

    if(hasToken) {
        client = new TokenManager(ipAddressNext, port, name, tokenTimeout, hasToken, this->numDevices);
    } else {
        client = new Client(ipAddressNext, port, name, tokenTimeout, hasToken);
    }    
    

    printf("---------------------------------- Cliente ---------------------------------- \n");
    printf("%s" ,client->toString().c_str());
    printf("----------------------------------------------------------------------------- \n");
}


void SigmaProtocol::start() {
    std::vector<std::string> input;
    int valor = 0;

    // Inicia a thread de monitoramento de pacotes especiais
    stopThreadP = false;
    listenerThread = std::thread([this]() { this->monitorSpecialPackets(); });

    while (!stopThreadP)
    {
        console.clearScreen();
        console.menu();
        console.readInput(input);
        
        if (input.size() == 0) {
            continue;
        }

        if (!isInteger(input[0])) {
            std::cout << "Erro: <tipo> deve ser um número inteiro.\n";
            continue;
        }

        int tipo = std::stoi(input[0]);

        switch (tipo)
        {
            case 1:

                if(input.size() < 3) {
                    printf("Erro: <destino> e <mensagem> são obrigatórios.\n");
                    continue;
                }
                if(input[1] == this->client->getName()) {
                    printf("Erro: não é permitido enviar mensagem para si mesmo.\n");
                    continue;
                }
                if(!client->enqueueMessage(input[1], input[2])) {
                    printf("Erro: número máximo de mensagens atingido.\n");
                    continue;
                }
                break;

            case 2:

                if(input.size() < 2) {
                    printf("Erro: <mensagem> é obrigatória.\n");
                    continue;
                }
                if(!client->enqueueMessage("TODOS", input[1])) {
                    printf("Erro: número máximo de mensagens atingido.\n");
                    continue;
                }
                break;

            case 3:
                client->removeToken();
                break;
            case 4:
                client->sendToken();    
                break;
            case 5: 
                if(input.size() < 2) {
                    printf("Erro: <porcentagem de erro> é obrigatória.\n");
                    continue;
                }
                valor = std::stoi(input[1]);
                if (valor < 0 || valor > 100) {
                    printf("Erro: porcentagem deve estar entre 0 e 100.\n");
                    continue;
                }
                client->setPacketError(valor);
                std::cout << Debug::verde("[INFO] Porcentagem de erro definida para: ") << valor << "%" << std::endl;
                
                break;

            case 6:
                console.clearScreen();
                console.printLog();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();          
                break;
            case 7:
                // Finaliza o loop e encerra o programa
                stopThreadP = true;
                console.__exit();
                break;
            default:
                std::cout << "Erro: <tipo> deve ser um número inteiro entre 1 e 7.\n";
                break;
        }
    }
}

void SigmaProtocol::monitorSpecialPackets() {
    char buffer[1024];
    
    while (!stopThreadP) {
        std::cout << Debug::amarelo("Esperando pacote...") << std::endl;
        int bytesReceived = client->recv(buffer, sizeof(buffer) - 1); // -1 para garantir espaço para o terminador nulo
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << Debug::amarelo("Pacote recebido: ") << std::string(buffer) << std::endl;

            try {
                Packet* packet = Packet::deserialize(std::vector<char>(buffer, buffer + bytesReceived));
                if (packet) {
                    // Debug::verde("Pacote desserializado com sucesso: " + packet->toString());
                    if (packet->getType() == 9000) {
                        std::this_thread::sleep_for(std::chrono::seconds(client->getTokenTimeout()));
                        client->handleToken(packet); 
                    }
                    else if(packet->getType() == 7777) {
                        std::this_thread::sleep_for(std::chrono::seconds(client->getTokenTimeout()));
                        client->handleMessage(packet); 
                    }
                    delete packet;
                } else {
                    std::cout << Debug::erro("Falha ao desserializar o pacote.");
                }
            } catch (const std::exception& e) {
                std::cout << Debug::erro("Exceção ao desserializar pacote: ") << e.what() << std::endl;
            } catch (...) {
                std::cout << Debug::erro("Exceção desconhecida ao desserializar pacote.") << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

SigmaProtocol::~SigmaProtocol() {
    stopThreadP = true;
    if (listenerThread.joinable()) {
        listenerThread.join();
    }
    if (client != nullptr) {
        delete client;
        client = nullptr;
    }
}



