#include "Gripper.h"

Gripper::Gripper(int dirPin1, int dirPin2)
{
  _dirPin1 = dirPin1;
  _dirPin2 = dirPin2;

}

void Gripper::begin()
{

  pinMode(_dirPin1, OUTPUT); 
  pinMode(_dirPin2, OUTPUT);
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




