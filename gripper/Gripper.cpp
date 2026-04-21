#include "Gripper.h"

Gripper::Gripper(int dirPin1, int dirPin2, int currentPin)
{
  _dirPin1 = dirPin1;
  _dirPin2 = dirPin2;
  _currentPin = currentPin;

}

void Gripper::begin()
{

  pinMode(_dirPin1, OUTPUT); 
  pinMode(_dirPin2, OUTPUT);
  pinMode(_currentPin, INPUT);
  stop();

}

void Gripper::close(int speed) 
{
  digitalWrite(_dirPin2, LOW);
  analogWrite(_dirPin1, speed);  // 0–255
}

void Gripper::open(int speed) 
{
  digitalWrite(_dirPin1, LOW);
  analogWrite(_dirPin2, speed);
}


void Gripper::stop() 
{
  digitalWrite(_dirPin1, LOW);
  digitalWrite(_dirPin2, LOW);
}

float Gripper::readCurrent()
{
  int raw = analogRead(_currentPin);

  float voltage = raw * (3.3f / 4095.0f);

  // V_actual = V_measured × (R1+R2)/R2 = V_measured × 30k/20k = × 1.5
  voltage = voltage * 1.5f;

    // ACS712 05B: 0A = 2.5V, sensitivity = 185mV/A
  float current = (voltage - 2.5f) / 0.185f;

  return current; // Measured in Amps
}



void Gripper::closeUntilGrip(int speed, float threshold)
{
  close(speed);
  while (true)
  {
    float current = readCurrent();
    Serial.print("Current: ");
    Serial.print(current, 3);
    Serial.println(" A");

    if (abs(current) >= threshold)
    {
      stop();
      Serial.println("Object gripped!");
      break;

    }
    delay(10);

  }
}




