#pragma once
#include <winsock2.h>
#include <string>

class Robot {
private:
    SOCKET connectSocket = INVALID_SOCKET;
    bool isConnected = false;
    
    // Internal helper to send raw text over the network
    void transmit(const std::string& command);

public:
    Robot();
    ~Robot();

    // Establishes the TCP connection to the Pico Server
    bool connectToPico(const std::string& ipAddress, int port);
    
   
    void close(); 
    void open();
    void setAngle(int angle);
};
