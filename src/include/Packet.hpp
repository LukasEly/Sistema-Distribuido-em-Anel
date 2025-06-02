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
        Packet(int type, Header* header, std::string payload, std::string crc32);
        Packet(const Packet& other);
        ~Packet();
        
        void serialize(std::vector<char>& buffer);
        static Packet* deserialize(const std::vector<char>& buffer);
    
        int getType() const { return type; }
        std::string getDestino() const { return header->nomeDestino; }
        std::string getOrigem() const { return header->nomeOrigem; }
        std::string getEstado() const { return header->estado; }
        std::string getCrc32() const { return header->crc32; }
        std::string getPayload() const { return payload; }

        std::string toString() const;

        bool isCrcOk() const;

};
// OBS: packet precisa ser serializada para enviar via socket, por possuir tipos que não são base, senão da merda do outro lado

#endif