#include "ServoGripper.h"
#include <WiFi.h>

const char* ssid       = "YOUR_WIFI";
const char* password   = "YOUR_PASSWORD";
const char* serverIP   = "192.168.1.100";
const int   serverPort = 5000;


// servoPin, openAngle, closeAngle
ServoGripper gripper(16, 100, 145); // To ensure the values are correct, the gripper needs to be put on the rotor correctly
// Find the endpoint where the rotor can no longer go counter clockwise, put the finger on at its most open, close it down until the finger
// is about the midpoint of the base. Now take the finger off and re-position it at the most open. It is now calibrated and will work
// with these values
WiFiClient client;

void setup() // TCP CODE NOT TESTED
{
    Serial.begin(115200);
    delay(3000);

    gripper.begin();
    Serial.println("Gripper ready.");

    // connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

    // connect to PC server
    while (!client.connect(serverIP, serverPort))
    {
        Serial.println("Connecting to server...");
        delay(1000);
    }
    Serial.println("Connected to server!");

    gripper.begin();
 
}

void loop()
{
        // reconnect if dropped
    if (!client.connected())
    {
        client.connect(serverIP, serverPort);
        return;
    }

    if (client.available())
    {
        String command = client.readStringUntil('\n');
        command.trim();  // remove whitespace and newline characters

        if (command == "GRIP")
        {
            gripper.close();
            Serial.println("Gripped");
            client.println("GRIPPED");
        }
        else if (command == "OPEN")
        {
            gripper.open();
            Serial.println("Opened");
            client.println("OPENED");
        }
        else if (command.startsWith("ANGLE:"))
        {
            // PC can send a specific angle if needed
            // example: "ANGLE:75"
            int angle = command.substring(6).toInt();
            gripper.setAngle(angle);
            client.println("ANGLE_SET:" + String(angle));
        }
    }
    // normal operation — open and close repeatedly

    Serial.println("Opening...");
    gripper.open();
    delay(2000);

    Serial.println("Closing...");
    gripper.close();
    delay(2000);

    Serial.println("Opening...");
    gripper.open();
    delay(2000);


    
}
