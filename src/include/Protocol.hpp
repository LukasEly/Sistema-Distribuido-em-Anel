#ifndef SIGMA_PROTOCOL_HPP
#define SIGMA_PROTOCOL_HPP
#include <iostream>

class SigmaProtocol {


public:
    SigmaProtocol();
    ~SigmaProtocol();

    void setConfiguration();
    void sendData(const std::string& data);
    std::string receiveData();

    int getIpAddress() const;
    int getPort() const;
    int getTokenTimeout() const;
    bool hasTokenFlag() const;
    const std::string& getName() const;
};

#endif // SIGMA_PROTOCOL_HPP
