#include <stdio.h>
#include "pico/stdlib.h"
#include "TCPServer.h"

int main()
{
  stdio_init_all();
  sleep_ms(2000);
  printf("USB interface init\n");

  const char *ssid = "Jeppe - iPhone";
  const char *password = "banankage3021";

  TcpServer server(ssid, password);

  if (!server.init())
  {
    printf("Server init failed\n");
    return 1;
  }

  while (true)
  {
    sleep_ms(1000);
  }

  return 0;
}