#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <string>

namespace DebugColor {
    const std::string RESET    = "\033[0m";
    const std::string YELLOW   = "\033[1;33m";
    const std::string RED      = "\033[1;31m";
    const std::string GREEN    = "\033[1;32m";
    const std::string BLUE     = "\033[1;34m";
    const std::string MAGENTA  = "\033[1;35m";
    const std::string CYAN     = "\033[36m";
    const std::string WHITE    = "\033[1;37m";
}

class Debug {
public:
    static std::string amarelo(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::YELLOW + msg + DebugColor::RESET;
    }
    static std::string vermelho(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::RED + msg + DebugColor::RESET;
    }
    static std::string verde(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::GREEN + msg + DebugColor::RESET;
    }
    static std::string azul(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::BLUE + msg + DebugColor::RESET;
    }
    static std::string magenta(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::MAGENTA + msg + DebugColor::RESET;
    }
    static std::string ciano(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::CYAN + msg + DebugColor::RESET;
    }
    static std::string branco(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::WHITE + msg + DebugColor::RESET;
    }
    static std::string erro(const std::string& msg) {
        return DebugColor::WHITE + "[ERRO] " + DebugColor::RESET + DebugColor::RED + msg + DebugColor::RESET;
    }
    static std::string info(const std::string& msg) {
        return DebugColor::WHITE + "[INFO] " + DebugColor::RESET + DebugColor::GREEN + msg + DebugColor::RESET;
    }
    static std::string mensagem(const std::string& msg) {
        return DebugColor::WHITE + "[MENSAGEM] " + DebugColor::RESET + DebugColor::GREEN + msg + DebugColor::RESET;
    }
    static std::string onlyBlue(const std::string& msg) {
        return DebugColor::BLUE + msg + DebugColor::RESET;
    }
    static std::string onlyWhite(const std::string& msg) {
        return DebugColor::WHITE + msg + DebugColor::RESET;
    }
    static std::string onlyMagenta(const std::string& msg) {
        return DebugColor::MAGENTA + msg + DebugColor::RESET;
    }
    static std::string onlyRed(const std::string& msg) {
        return DebugColor::RED + msg + DebugColor::RESET;
    }
    static std::string exitMSG(const std::string& msg) {
        return DebugColor::RED + "[EXIT] " + DebugColor::RESET + DebugColor::WHITE + msg + DebugColor::RESET;
    }
};

#endif