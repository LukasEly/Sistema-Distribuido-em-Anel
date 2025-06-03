#include "src/include/Console.hpp"

Console::Console() {
    this->clearLog();
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

void Console::readInput(std::vector<std::string>& input) {

    input.clear();

    int type;
    std::string type_s;
    std::string arg1;
    std::string arg2;
    
    std::cin >> type_s;

    if(isInteger(type_s)) {
        type = std::stoi(type_s);
    } else {
        std::cerr << "Tipo inválido. Por favor, insira um número." << std::endl;
        return;
    }
    
    input.push_back(type_s);

    switch (type)
    {
    case 5:
        clearScreen(); 
        printf("1. Voltar ao menu\n\n");
        printf("Digite a <porcentagem> de chance de acontecer um erro em uma mensagem: ");

        std::cin >> arg1;
        input.push_back(arg1);
 
        if (arg1 == "1") {
            clearScreen();
            break;
        }
        break;
    case 3 ... 4:
    case 6 ... 7:
        // só precisa de 1 argumento (type)
        break;
    
    case 2:
        clearScreen();
        printf("1. Voltar ao menu\n\n");
        printf("Digite a <mensagem> a ser enviada para todo mundo: ");  
        std::getline(std::cin >> std::ws, arg1); 
        input.push_back(arg1);  
        if (arg1 == "1") {
            clearScreen();
            break;
        }
        break;
    case 1:
        clearScreen();  
        printf("1. Voltar ao menu\n\n");
        printf("Digite o <destino>: ");
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer
        std::cin >> arg1;
        input.push_back(arg1);

        if (arg1 == "1") {
            clearScreen();
            break;
        }
        printf("Digite a <mensagem>: ");
        std::getline(std::cin >> std::ws, arg2); 
        input.push_back(arg2);

        printf("1. Voltar ao menu\n");
        if (arg2 == "1") {
            clearScreen();
            break;
        }
        break;
    default:
        break;
    }
}

void Console::printLog() {
    std::ifstream logFile("log.txt");
    if (!logFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(logFile, line)) {
        printf("%.*s\n", (int)line.size(), line.c_str());
    }
    
    logFile.close();
}

void Console::clearLog() {
    std::ofstream logFile("log.txt", std::ios::trunc);
    if (!logFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;
        return;
    }
    
    logFile.close();
}

void Console::__exit() {
    std::cout << "Saindo..." << std::endl;
    exit(0);
}