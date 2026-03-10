#pragma once

#include <Arduino.h>

class Gripper 
{
public:
Gripper(int pwmPin, int dirPin1, int dirPin2);
void begin();
void close(int speed = 200);
void open(int speed = 200);
void stop();

private: 
int _pwmPin;
int _dirPin1;
int _dirPin2;

};
