
#include "GripperClient.h"
#include <cstdio>
#include <cstring>

GripperClient::GripperClient(const std::string &pico_ip, uint16_t port)
{
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, pico_ip.c_str(), &addr.sin_addr);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0)
        {
            printf("Gripper connection FAILED to %s:%u, error: %d\n", pico_ip.c_str(), port, WSAGetLastError());
            sock = INVALID_SOCKET;
            return;
        }
        printf("Gripper connected to %s:%u\n", pico_ip.c_str(), port);
}

GripperClient::~GripperClient()
{
    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif
}

std::string GripperClient::send_command(const std::string &command)
{
    std::string msg = command + "\n";
    send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);

    char buffer[128] = {};
    int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    printf("recv returned: %d, error: %d\n", received, WSAGetLastError());

    std::string response(buffer);
    while (!response.empty() && (response.back() == '\n' || response.back() == '\r'))
        response.pop_back();
    return response;
}

bool GripperClient::open()
{
    std::string response = send_command("OPEN");
    printf("Gripper: %s\n", response.c_str());
    return response == "OK OPEN";
}

bool GripperClient::close()
{
    std::string response = send_command("CLOSE");
    printf("Gripper: %s\n", response.c_str());
    return response == "OK CLOSED";
}
