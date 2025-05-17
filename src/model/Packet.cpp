#include "src/include/Packet.hpp"

Header::Header(std::string estado, std::string nomeOrigem, std::string nomeDestino) : estado(estado), nomeOrigem(nomeOrigem), nomeDestino(nomeDestino) {
    this->crc32 = ""; // só calcula header depois
}

int Header::size() {
    return (estado + ";" + nomeOrigem + ";" + nomeDestino + ";" + crc32).size();
}

std::string Header::toString() {
    return estado + ";" + nomeOrigem + ";" + nomeDestino + ";" + crc32;
}

Packet::Packet(int type, Header* header, std::string payload) : payload(payload) {
    if(type != 9000 && type != 7777) {
        throw std::invalid_argument("Invalid packet type");
    }

    this->type = type;
    bool isToken = (type == 9000);

    if(isToken) {
        this->header = nullptr;
        return;
    }

    if(!isToken && header == nullptr) {
        throw std::invalid_argument("Header is null");
    } 

    this->header = header;

    uLong crc = crc32(0L, Z_NULL, 0); // inicia o CRC
    crc = crc32(crc, reinterpret_cast<const Bytef*>(payload.c_str()), payload.size());    

    header->crc32 = std::to_string(crc); // calcula o CRC do payload
}

void Packet::serialize(std::vector<char>& buffer) {

    bool isToken = (type == 9000);
    std::string type_s = std::to_string(type);

    if(isToken && buffer.size() < type_s.size()) {
        buffer.resize(type_s.size());
    } else if(!isToken && buffer.size() < type_s.size() + header->size() + payload.size()) {
        buffer.resize(type_s.size() + header->size() + payload.size() + 2); // +5 é do ';' e do ':' 
    }

    size_t offset = 0;
    memcpy(buffer.data() + offset, type_s.c_str(), type_s.size());
    offset += type_s.size();

    if(isToken) {
        return;
    }  

    if(header == nullptr) {
        throw std::invalid_argument("Header is null");
    }

    buffer[offset++] = ':';
    
    // -------------- construção do header

    std::string header_str = header->toString();
    memcpy(buffer.data() + offset, header_str.c_str(), header_str.size());
    offset += header_str.size();
    buffer[offset++] = ';';

    // -------------------------

    memcpy(buffer.data() + offset, payload.c_str(), payload.size());
    offset += payload.size();
}

Packet::~Packet() {
    if(header != nullptr) {
        delete header;
    }
}