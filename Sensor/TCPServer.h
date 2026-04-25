#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "lwip/tcp.h"

class TcpServer
{
public:
  TcpServer(const char *ssid, const char *password);

  bool init(); // Init WiFi + server

private:
  // variabler
  const char *wifi_ssid;
  const char *wifi_password;

  // data struktur som bruges som state machine
  struct TcpServerState
  {
    tcp_pcb *server_pcb;
    tcp_pcb *client_pcb;
  };

  TcpServerState state;

  // --- interne funktioner ---
  err_t sendResponse(tcp_pcb *pcb, const char *text);
  err_t handleCommand(tcp_pcb *pcb, const char *command);

  void trimCommand(char *buffer);
  void closeClientConnection();

  bool connectToWifi();
  bool startTcpServer();

  // --- static callbacks (KRÆVET af lwIP) ---
  static err_t onTcpReceive(void *arg, tcp_pcb *pcb, pbuf *p, err_t err);
  static err_t onClientConnected(void *arg, tcp_pcb *client_pcb, err_t err);
  static void onTcpError(void *arg, err_t err);

  // helper til at få this pointer
  static TcpServer *instance;
};

#endif