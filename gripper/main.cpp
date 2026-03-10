#include "Gripper.h"

// Pins: PWM, Direction1, Direction2
// Adjust these to match your wiring on the Pico
Gripper gripper(16, 0, 1);




void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  gripper.begin();
  Serial.println("Gripper ready.");
}

void loop() 
{
  // put your main code here, to run repeatedly:

  Serial.println("Closing...");
  gripper.close(200);   // speed 0-255
  delay(2000);

  gripper.stop();
  delay(500);

  Serial.println("Opening...");
  gripper.open(200);
  delay(2000);

  gripper.stop();
  delay(500);

}

