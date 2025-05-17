#include "src/include/Console.hpp"

Console::Console() {
    // Inicializa o console
}
Console::~Console() {

}

void Console::clearScreen() {
    printf("\033[2J\033[1;1H");
}

void Console::menu() {
    printf("1. Enviar mensagem unicast\n");
    printf("2. Enviar mensagem broadcast\n");
    printf("3. Remover token\n");
    printf("4. Adicionar token\n");
    printf("5. Adicionar erros em pacotes\n");
    printf("6. Imprimir log\n");
    printf("7. Sair\n");
    printf("Escolha uma opção: ");
}

void Console::printLog() {
    std::ifstream logFile("log.txt");
    if (!logFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(logFile, line)) {
        std::cout << line << std::endl;
    }
    
    logFile.close();
}

void Console::__exit() {
    std::cout << "Saindo..." << std::endl;
    exit(0);
}












