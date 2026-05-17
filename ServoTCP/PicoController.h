#pragma once
#include <winsock2.h>
#include <string>

class PicoController {
private:
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    bool isPicoConnected = false;

public:
    PicoController();
    ~PicoController();
    
    bool startServer(int port);
    void updateConnection(); // Automatically checks for incoming connection
    bool sendCommand(const std::string& cmd);
    std::string readResponse(); // Non-blocking read function
    bool isConnected() const { return isPicoConnected; }
};
