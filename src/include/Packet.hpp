#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>

class Header {
    std::string estado;
    std::string nomeOrigem;
    std::string nomeDestino;
    std::string crc32;
};

class Packet {
    
    private:
        
        int type;
        Header* header;
        std::string payload;
    
    public:
    
        Packet(int type, Header* header, std::string payload);
        ~Packet();
        
        void serialize(char* buffer);
};
// OBS: packet precisa ser serializada para enviar via socket, por possuir tipos que não são base, senão da merda do outro lado

#endif