#include "src/include/Packet.hpp"

Packet::~Packet() {
    delete header;
}