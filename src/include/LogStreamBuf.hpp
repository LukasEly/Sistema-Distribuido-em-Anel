#ifndef LOGSTREAMBUF_HPP
#define LOGSTREAMBUF_HPP

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

class LogStreamBuf : public std::streambuf {
public:
    LogStreamBuf(std::streambuf* originalBuf);

protected:
    int overflow(int c) override;

private:
    std::streambuf* original;
    std::string buffer;

    void saveLog(const std::string& log);
};

class ScopedLogRedirect {
public:
    ScopedLogRedirect(std::ostream& stream, std::streambuf* newBuf);

    ~ScopedLogRedirect();

private:
    std::ostream& stream;
    std::streambuf* oldBuf;
};


#endif