#include "src/include/Utils.hpp"

bool isInteger(const std::string& str) {
    try {
        std::size_t pos;
        std::stoi(str, &pos);
        return pos == str.size(); // Garante que a string inteira foi convertida
    } catch (...) {
        return false;
    }
}