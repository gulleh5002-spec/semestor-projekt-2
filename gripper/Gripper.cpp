#include "Gripper.h"

Gripper::Gripper(int pwmPin, int dirPin1, int dirPin2)
{
  _pwmPin = pwmPin;
  _dirPin1 = dirPin1;
  _dirPin2 = dirPin2;

}

void Gripper::begin()
{

  pinMode(_pwmPin, OUTPUT); // Sets the pwm pin as an output
  pinMode(_dirPin1, OUTPUT); 
  pinMode(_dirPin2, OUTPUT);
  stop();

}

void Gripper::close(int speed) 
{
  digitalWrite(_dirPin1, HIGH);
  digitalWrite(_dirPin2, LOW);
  analogWrite(_pwmPin, speed);  // 0–255
}

void Gripper::open(int speed) 
{
  digitalWrite(_dirPin1, LOW);
  digitalWrite(_dirPin2, HIGH);
  analogWrite(_pwmPin, speed);
}


void Gripper::stop() 
{
  digitalWrite(_dirPin1, LOW);
  digitalWrite(_dirPin2, LOW);
  analogWrite(_pwmPin, 0);
}




