#ifndef NETWORK_H
#define NETWORK_H

#include "lwip/tcp.h"

class Gripper;

class Network
{
public:
  Network(const char *ssid, const char *password, Gripper &gripper);

  bool init(); // Init WiFi + server

private:
  // enum class GripperState
  // {
  //   OPEN,
  //   CLOSED
  // };

  // const char *state_to_text(GripperState state);
  // void apply_gripper_state(GripperState state);

  // variabler
  Gripper &gripper;
  const char *wifi_ssid;
  const char *wifi_password;

  // data struktur som bruges som state machine
  struct NetworkState
  {
    tcp_pcb *server_pcb;
    tcp_pcb *client_pcb;
  };

  NetworkState state;

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
  static Network *instance;
};

#endif