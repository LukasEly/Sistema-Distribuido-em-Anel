#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <mutex>
#include <fstream>
#include <vector>

class Console {

    private:

        std::mutex consoleMutex;

    public:

        Console();
        ~Console();

        void clearScreen();
        void menu();

        std::string createMessageUni();
        std::string createMessageBroad();

        bool removeToken();
        bool addToken();

        void readInput(std::vector<std::string>& input); // definir se vai fazer algo personalizado aqui ou não

        void printLog();

        // não sei se tem como implementar essa função assim de um jeito fácil
        void __exit(); // '__' porque exit já existe no C e geralmente '_' é método privado
};

#endif