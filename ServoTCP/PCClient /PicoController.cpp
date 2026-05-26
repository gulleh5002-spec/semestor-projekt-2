#include "PicoController.h"
#include <iostream>
#include <ws2tcpip.h>

Robot::Robot() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}

Robot::~Robot() {
    if (connectSocket != INVALID_SOCKET) {
        closesocket(connectSocket);
    }
    WSACleanup();
}

bool Robot::connectToPico(const std::string& ipAddress, int port) {
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

    std::cout << "[SDK System] Connecting to Pico Server at " << ipAddress << ":" << port << "...\n";
    
    if (connect(connectSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
        return false;
    }

    u_long mode = 1;
    ioctlsocket(connectSocket, FIONBIO, &mode);

    isConnected = true;
    std::cout << "[SDK System] Network link established successfully!\n";
    return true;
}

void Robot::transmit(const std::string& command) {
    if (!isConnected) return;
    std::string packet = command + "\n";
    send(connectSocket, packet.c_str(), static_cast<int>(packet.size()), 0);
}

// Map your abstract API methods to the raw network commands
void Robot::close() {
    std::cout << "[API Invoke] robot.close()\n";
    transmit("GRIP");
}

void Robot::open() {
    std::cout << "[API Invoke] robot.open()\n";
    transmit("OPEN");
}

void Robot::setAngle(int angle) {
    std::cout << "[API Invoke] robot.setAngle(" << angle << ")\n";
    
    // Construct the command string: "ANGLE:75"
    std::string command = "ANGLE:" + std::to_string(angle);
    transmit(command);
}
