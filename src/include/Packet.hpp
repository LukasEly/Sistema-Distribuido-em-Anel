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
    
        std::string getType() const;
        std::string getDestino() const { return header ? header->nomeDestino : ""; }
        std::string getOrigem() const { return header ? header->nomeOrigem : ""; }
        std::string getEstado() const { return header ? header->estado : ""; }
        std::string getCrc32() const { return header ? header->crc32 : ""; }
        std::string getPayload() const { return payload; }
        
        void setCrc32(const std::string& crc32) { header->crc32 = crc32; }

        std::string toString() const;

        bool isCrcOk() const;

};
// OBS: packet precisa ser serializada para enviar via socket, por possuir tipos que não são base, senão da merda do outro lado

#endif