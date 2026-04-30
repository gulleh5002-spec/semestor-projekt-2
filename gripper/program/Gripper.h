#ifndef GRIPPER_H
#define GRIPPER_H

#include "Motor.h"
#include "CurrentSensor.h"

class Gripper
{
private:
  Motor &motor;
  CurrentSensor &sensor;
  float treshold = 0.16;

public:
  Gripper(Motor &m, CurrentSensor &s);
  bool init();
  bool open();
  bool close();
};

#endif