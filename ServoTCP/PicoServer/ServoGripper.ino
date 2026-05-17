#include "ServoGripper.h"
#include <WiFi.h>

const char* ssid     = "AndroidAPa138";
const char* password = "shle9732";
const int serverPort = 5000;

// Defines static IP configuration so it's the same everytime
IPAddress local_IP(10, 174, 5, 49);
IPAddress gateway(10, 174, 5, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(serverPort);
WiFiClient client; 

ServoGripper gripper(16, 120, 145); 

void setup() 
{
    Serial.begin(115200);
    delay(3000);

    gripper.begin();
    Serial.println("Gripper ready.");


    WiFi.config(local_IP, gateway, subnet);
    Serial.println("Static IP configured.");

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected. Permanent IP: " + WiFi.localIP().toString());

    // Start the TCP server on port 5000
    server.begin();
    Serial.println("Pico Server started. Listening for PC connections...");
}

void loop()
{
    if (!client || !client.connected())
    {
        client = server.accept(); // Using the updated accept() method
        if (client) {
            Serial.println("PC Client connected!");
        }
    }

    if (client && client.connected() && client.available())
    {
        String command = client.readStringUntil('\n');
        command.trim();

        if (command == "GRIP") {
            gripper.close();
            client.println("GRIPPED");
        }
        else if (command == "OPEN") {
            gripper.open();
            client.println("OPENED");
        }
        else if (command.startsWith("ANGLE:")) {
            int angle = command.substring(6).toInt();
            gripper.setAngle(angle);
            client.println("ANGLE_SET:" + String(angle));
        }
        else {
            client.println("ERROR: Unknown command.");
        }
    }
}
