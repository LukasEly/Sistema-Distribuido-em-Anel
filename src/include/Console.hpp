#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>

class Console {

    public:

        Console();
        ~Console();

        void clearScreen();
        void menu();

        std::string createMessageUni();
        std::string createMessageBroad();

        bool removeToken();
        bool addToken();

        void saveLog();
        void printLog();

        void __exit(); // '__' porque exit já existe no C e geralmente '_' é método privado
};

#endif