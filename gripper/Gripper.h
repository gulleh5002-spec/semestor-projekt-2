#pragma once

#include <Arduino.h>

class Gripper 
{
public:
Gripper(int dirPin1, int dirPin2);
void begin();
void close(int speed = 150);
void open(int speed = 150);
void stop();

private: 
int _dirPin1;
int _dirPin2;

};
