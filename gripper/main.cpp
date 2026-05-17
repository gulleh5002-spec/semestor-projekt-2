#include "Gripper.h"

// Pins: PWM, Direction1, Direction2

Gripper gripper(0, 1);




void setup() 
{
 
  Serial.begin(115200);
  gripper.begin();
  Serial.println("Gripper ready.");
}

void loop() 
{

  Serial.println("Closing...");
  gripper.close(150);   // speed 0-255
  delay(6000);

  gripper.stop();
  delay(500);

  Serial.println("Opening...");
  gripper.open(150);
  delay(6000);

  gripper.stop();
  delay(500);

}
