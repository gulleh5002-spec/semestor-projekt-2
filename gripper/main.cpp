#include "Gripper.h"

// pinA, pinB, ADC pin
// Adjust these to match your wiring on the Pico
Gripper gripper(0, 1, 27);




void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(3000);
  gripper.begin();
  Serial.println("Gripper ready.");
}


void loop()
{
    gripper.close(150);
    for(int i = 0; i < 60; i++) {  // 6 seconds worth of readings
        Serial.println(analogRead(27));
        delay(100);
    }

    gripper.stop();
    delay(500);

    gripper.open(150);
    for(int i = 0; i < 60; i++) {
        Serial.println(analogRead(27));
        delay(100);
    }

    gripper.stop();
    delay(500);
}

