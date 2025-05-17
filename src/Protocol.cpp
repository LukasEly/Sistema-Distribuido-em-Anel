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
    
    std::vector<std::string> input;

    while (true)
    {
        console.clearScreen();
        console.menu();

        console.readInput(input);
        
        if (input.size() == 0) {
            continue;
        }

        if (!isInteger(input[0])) {
            printf("Erro: <tipo> deve ser um número inteiro.\n");
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

            break;
        case 2:
            if(input.size() < 2) {
                printf("Erro: <mensagem> é obrigatória.\n");
                continue;
            }

            break;
        case 3:
            
            break;
        case 4:
        
            break;
        case 5: 
            if(input.size() < 2) {
                printf("Erro: <porcentagem de erro> é obrigatória.\n");
                continue;
            }
        
            break;
        case 6:
            console.printLog();            

            break;
        case 7:
            // ainda não funciona, tem que limpar todas threads e dados antes !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            console.__exit();

            break;
        default:
            printf("Erro: <tipo> deve ser um número inteiro entre 1 e 7.\n");
            break;
        }
    }
}

SigmaProtocol::~SigmaProtocol() {
    delete client; 
}



