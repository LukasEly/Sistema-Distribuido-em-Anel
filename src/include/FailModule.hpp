#ifndef FAILMODULE_HPP
#define FAILMODULE_HPP

#include <random>

class FailModule {

    public:

        FailModule();
        ~FailModule();

        void addPakcetError(char* buffer);
        bool removeToken(); // só vai retornar se remove ou não
        bool addToken(); // só vai retornar se adiciona ou não
};

#endif