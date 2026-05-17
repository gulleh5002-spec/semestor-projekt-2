#include "PicoController.h"
#include <iostream>

PicoController::PicoController() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}

PicoController::~PicoController() {
    if (clientSocket != INVALID_SOCKET) closesocket(clientSocket);
    if (serverSocket != INVALID_SOCKET) closesocket(serverSocket);
    WSACleanup();
}

bool PicoController::startServer(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) return false;

    // Set server socket to non-blocking so it doesn't freeze while waiting for a Pico
    u_long mode = 1;
    ioctlsocket(serverSocket, FIONBIO, &mode);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        return false;
    }

    listen(serverSocket, 1);
    std::cout << "[SDK Network] Server listening on port " << port << "...\n";
    return true;
}

void PicoController::updateConnection() {
    if (isPicoConnected) return; // Already connected

    // Since serverSocket is non-blocking, accept returns immediately
    SOCKET tempSocket = accept(serverSocket, nullptr, nullptr);
    if (tempSocket != INVALID_SOCKET) {
        clientSocket = tempSocket;
        isPicoConnected = true;
        
        
        u_long mode = 1;
        ioctlsocket(clientSocket, FIONBIO, &mode);
        
        std::cout << "\n[SDK Network] Pico Gripper safely connected to robot engine!\n";
    }
}

bool PicoController::sendCommand(const std::string& cmd) {
    if (!isPicoConnected) return false;
    std::string formattedCmd = cmd + "\n";
    int result = send(clientSocket, formattedCmd.c_str(), formattedCmd.size(), 0);
    
    if (result == SOCKET_ERROR) {
        // Connection dropped
        std::cout << "[SDK Network] Pico disconnected entirely.\n";
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        isPicoConnected = false;
        return false;
    }
    return true;
}

std::string PicoController::readResponse() {
    if (!isPicoConnected) return "";

    char buffer[128] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        return std::string(buffer);
    }
    else if (bytesReceived == 0 || (bytesReceived == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)) {
        // Connection physically broke or disconnected
        std::cout << "[SDK Network] Connection lost during read loop.\n";
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        isPicoConnected = false;
    }
    
    // If bytesReceived == SOCKET_ERROR and error is WSAEWOULDBLOCK, 
    // it simply means "No data available right now". We return empty string instantly.
    return "";
}
