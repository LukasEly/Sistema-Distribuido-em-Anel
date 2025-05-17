#include <iostream>
#include "include/Protocol.hpp"
#include "include/LogStreamBuf.hpp"

int main(int argc, char *argv[]) {

    // configuração para todo cout chamado implementar função de save em um arquivo de log
    // ---------------------------------------------------------------------------------------
    LogStreamBuf customBuf(std::cout.rdbuf());
    ScopedLogRedirect redirect(std::cout, &customBuf);
    // ---------------------------------------------------------------------------------------
    // apenas aplicado para cout e cerr  

    // IMPORTANTE: Se não quiser que o print saia no arquivo log.txt, usar printf()

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numDevices>\n", argv[0]);
        return 1; // erro
    }

    if(!isInteger(argv[1])) {
        fprintf(stderr, "Erro: <numDevices> deve ser um número inteiro.\n");
        return 1; // erro
    }

    int numDevices = std::stoi(argv[1]);

    { // bloco do código
        SigmaProtocol protocolo(numDevices);
        protocolo.start();
    } // destrutor é chamado aqui

    return 0;
}