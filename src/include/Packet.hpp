#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <zlib.h>
#include <memory>

struct Header {
    
    std::string estado;
    std::string nomeOrigem;
    std::string nomeDestino;
    std::string crc32;

    Header(std::string estado, std::string nomeOrigem, std::string nomeDestino);
    int size();
    std::string toString();
};

class Packet {
    
    private:
        
        int type;
        Header* header;
        std::string payload;
    
    public:
    
        Packet(int type, Header* header, std::string payload);
        ~Packet();
        
        void serialize(std::vector<char>& buffer);
        std::string toString();
};
// OBS: packet precisa ser serializada para enviar via socket, por possuir tipos que não são base, senão da merda do outro lado

#endif