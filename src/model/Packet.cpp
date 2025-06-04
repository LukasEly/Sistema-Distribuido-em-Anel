#include "src/include/Packet.hpp"
#include <zlib.h>

Header::Header(std::string estado, std::string nomeOrigem, std::string nomeDestino) : estado(estado), nomeOrigem(nomeOrigem), nomeDestino(nomeDestino) {
    this->crc32 = ""; // só calcula header depois
}

int Header::size() {
    return (estado + ";" + nomeOrigem + ";" + nomeDestino + ";" + crc32).size();
}

std::string Header::toString() {
    return estado + ";" + nomeOrigem + ";" + nomeDestino + ";" + crc32;
}

Packet::Packet(int type, Header* header, std::string payload, std::string crc32) : type(type), header(header), payload(payload) {
    if(type != 9000 && type != 7777) {
        throw std::invalid_argument("Invalid packet type");
    }

    bool isToken = (type == 9000);

    if(isToken) {
        this->header = nullptr;
        return;
    }

    if(!isToken && header == nullptr) {
        throw std::invalid_argument("Header is null");
    } 

    this->header = header;

    this->header->crc32 = crc32;

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

Packet* Packet::deserialize(const std::vector<char>& buffer) {
    size_t offset = 0;
    int type = 0;

    // Lê o tipo do pacote
    std::string type_str;
    while (offset < buffer.size() && buffer[offset] != ':') {
        type_str += buffer[offset++];
    }
    offset++; // Pula o ':'

    try {
        type = std::stoi(type_str); 
    } catch (const std::exception& e) {
        return nullptr;
    }


    if(type != 9000 && type != 7777) {
        return nullptr;
    }

    if(offset > buffer.size() && type == 9000) {
        return new Packet(type, nullptr, ""); 
    } else if(offset <= buffer.size() && type == 9000) {
        return nullptr;
    }

    Header* header = nullptr;

    // Lê o header
    std::string s;
    while (offset < buffer.size() && buffer[offset] != '\0') {
        s += buffer[offset++];
    }
    offset++; // Pula o ';'

    // Divide o header em partes
    size_t pos1 = s.find(';');
    size_t pos2 = s.find(';', pos1 + 1);
    size_t pos3 = s.find(';', pos2 + 1);
    size_t pos4 = s.find(';', pos3 + 1);

    if(pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos || pos4 == std::string::npos) {
        throw std::invalid_argument("Invalid header format");
    }

    std::string estado = s.substr(0, pos1);
    std::string nomeOrigem = s.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string nomeDestino = s.substr(pos2 + 1, pos3 - pos2 - 1);
    std::string crc32 = s.substr(pos3 + 1, pos4 - pos3 - 1);

    header = new Header(estado, nomeOrigem, nomeDestino);
    
    std::string payload = s.substr(pos4 + 1);

    return new Packet(type, header, payload, crc32);
}

std::string Packet::toString() const {
    std::string result = std::to_string(type);
    if(header != nullptr) {
        result += ':' + header->toString() + ';';
    }
    result += payload;
    return result;
}

bool Packet::isCrcOk() const {
    if (!header) return true; // Se não tem header, considere válido (ou ajuste conforme sua lógica)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(payload.c_str()), payload.size());
    return std::to_string(crc) == header->crc32;
}

std::string Packet::getType() const {
    if(type == 7777) {
        return "MENSAGEM";
    } else if (type == 9000) {
        return "TOKEN";
    }

    return "DESCONHECIDO";
}   

Packet::Packet(const Packet& other)
    : type(other.type), payload(other.payload)
{
    if (other.header)
        header = new Header(*other.header);
    else
        header = nullptr;
}

Packet::~Packet() {
    if(header != nullptr) {
        delete header;
    }
}