#include "Network.h"
#include "Gripper.h"
#include "CurrentSensor.h"
#include "Motor.h"

#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    // til debug
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    Motor motor(2, 3);
    CurrentSensor sensor(26, 0.185);
    Gripper gripper(motor, sensor);

    if (!gripper.init())
    {
        return 1;
    }

    const char *ssid = "Gustav - iPhone";
    const char *password = "gustavhansen";

    Network server(ssid, password, gripper);

    if (!server.init())
    {
        printf("Server init failed\n");
        return 1;
    }

    sleep_ms(1000);

    while (true)
    {
        sleep_ms(10);
    }
}
