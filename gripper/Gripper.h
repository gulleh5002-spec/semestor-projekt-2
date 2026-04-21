#pragma once

#include <Arduino.h>

class Gripper 
{
public:
Gripper(int dirPin1, int dirPin2, int currentPin);
void begin();
void close(int speed = 200);
void open(int speed = 200);
void stop();
void closeUntilGrip(int speed = 200, float threshold = 0.4);
float readCurrent();

private: 
int _dirPin1;
int _dirPin2;
int _currentPin;
};
