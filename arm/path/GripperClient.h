#pragma once
#include <string>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using socket_t = SOCKET;
#else
using socket_t = int;  
#endif

class GripperClient
{
public:
    GripperClient(const std::string &pico_ip, uint16_t port = 4242);
    ~GripperClient();

    bool open();   // sender "OPEN",  venter på "OK OPEN"
    bool close();  // sender "CLOSE", venter på "OK CLOSED"

private:
    std::string send_command(const std::string &command);
    socket_t sock;
};