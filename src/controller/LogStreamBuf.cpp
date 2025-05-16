#include "src/include/LogStreamBuf.hpp"

LogStreamBuf::LogStreamBuf(std::streambuf* originalBuf) : original(originalBuf) {}

int LogStreamBuf::overflow(int c) {
    if (c != EOF) {
        char ch = static_cast<char>(c);
        buffer += ch;

        if(original) {
            original->sputc(ch); // ainda imprime no terminal
        }

        if (ch == '\n') {
            saveLog(buffer);
            buffer.clear();
        }
    }
    return c;
}

void LogStreamBuf::saveLog(const std::string& log) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << log;
    } else {
        fprintf(stderr, "[ERRO] Não foi possível salvar no log\n");
    }
}

ScopedLogRedirect::ScopedLogRedirect(std::ostream& stream, std::streambuf* newBuf) : stream(stream), oldBuf(stream.rdbuf()) {
    stream.rdbuf(newBuf);
}

ScopedLogRedirect::~ScopedLogRedirect() {
    stream.rdbuf(oldBuf); // restaura automaticamente
}       