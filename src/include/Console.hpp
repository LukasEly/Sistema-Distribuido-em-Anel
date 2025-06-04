#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <mutex>
#include <fstream>
#include <vector>

#include "Utils.hpp"
#include "src/include/Debug.hpp"

class Console {

    private:

        std::mutex consoleMutex;

    public:

        Console();
        ~Console();

        void clearScreen();
        void menu();

        void readInput(std::vector<std::string>& input);

        void printLog();
        void clearLog();
        
        void __exit();
};

#endif