#include "TCPServer.h"

#include <cstdio>
#include <cstring>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"

// ----------- konstanter -----------
constexpr uint16_t SERVER_PORT = 4242;
constexpr size_t BUFFER_SIZE = 128;

// static instance
TcpServer *TcpServer::instance = nullptr;

// ----------- constructor -----------
TcpServer::TcpServer(const char *ssid, const char *password)
{
  wifi_ssid = ssid;
  wifi_password = password;

  state.server_pcb = nullptr;
  state.client_pcb = nullptr;

  instance = this; // vigtigt til callbacks
}

// ----------- public -----------
bool TcpServer::init()
{
  if (cyw43_arch_init())
  {
    printf("WiFi chip init failed\n");
    return false;
  }

  cyw43_arch_enable_sta_mode();

  if (!connectToWifi())
    return false;

  if (!startTcpServer())
    return false;

  return true;
}

// ----------- private -----------

err_t TcpServer::sendResponse(tcp_pcb *pcb, const char *text)
{
  if (!pcb)
    return ERR_CONN;

  uint16_t len = strlen(text);
  err_t err = tcp_write(pcb, text, len, TCP_WRITE_FLAG_COPY);
  if (err != ERR_OK)
    return err;

  return tcp_output(pcb);
}

// tilføj efter behov
err_t TcpServer::handleCommand(tcp_pcb *pcb, const char *cmd)
{
  if (strcmp(cmd, "PING") == 0)
  {
    return sendResponse(pcb, "PONG\n");
  }

  return sendResponse(pcb, "ERROR\n");
}

void TcpServer::trimCommand(char *buf)
{
  size_t len = strlen(buf);
  while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r' || buf[len - 1] == ' '))
  {
    buf[len - 1] = '\0';
    len--;
  }
}

void TcpServer::closeClientConnection()
{
  if (!state.client_pcb)
    return;

  tcp_close(state.client_pcb);
  state.client_pcb = nullptr;
}

bool TcpServer::connectToWifi()
{
  printf("Connecting to WiFi SSID '%s'...\n", wifi_ssid);

  int result = cyw43_arch_wifi_connect_timeout_ms(
      wifi_ssid,
      wifi_password,
      CYW43_AUTH_WPA2_AES_PSK,
      30000);

  if (result != 0)
  {
    printf("WiFi connection failed: %d\n", result);
    return false;
  }

  const ip4_addr_t *ip = netif_ip4_addr(netif_default);
  printf("WiFi connected. Pico IP: %s\n", ip4addr_ntoa(ip));
  return true;
}

bool TcpServer::startTcpServer()
{
  tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
  if (!pcb)
    return false;

  if (tcp_bind(pcb, IP_ANY_TYPE, SERVER_PORT) != ERR_OK)
    return false;

  state.server_pcb = tcp_listen_with_backlog(pcb, 1);

  tcp_accept(state.server_pcb, onClientConnected);

  return true;
}

// ----------- callbacks -----------

err_t TcpServer::onClientConnected(void *arg, tcp_pcb *client, err_t err)
{
  if (!instance)
    return ERR_OK;

  instance->state.client_pcb = client;

  tcp_recv(client, onTcpReceive);
  tcp_err(client, onTcpError);

  printf("Client connected\n");
  return ERR_OK;
}

err_t TcpServer::onTcpReceive(void *arg, tcp_pcb *pcb, pbuf *packet, err_t err)
{
  if (!instance)
    return ERR_OK;

  if (!packet)
  {
    instance->closeClientConnection();
    printf("Client disconnected\n");
    return ERR_OK;
  }

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));

  pbuf_copy_partial(packet, buffer, packet->tot_len, 0);
  tcp_recved(pcb, packet->tot_len);
  pbuf_free(packet);

  instance->trimCommand(buffer);

  printf("Received command: %s\n", buffer);

  return instance->handleCommand(pcb, buffer);
}

void TcpServer::onTcpError(void *arg, err_t err)
{
  if (instance)
    instance->state.client_pcb = nullptr;
  printf("TCP error: %d\n", err);
}