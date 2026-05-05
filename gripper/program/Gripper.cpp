#include "Gripper.h"

#include <stdio.h>

Gripper::Gripper(Motor &m, CurrentSensor &s) : motor(m), sensor(s) {}

bool Gripper::init()
{
  if (!sensor.init())
  {
    printf("Current Sensor init failed\n");
    return false;
  }
  if (!motor.init())
  {
    printf("Motor init failed\n");
    return false;
  }
  motor.stop();
  return true;
}

bool Gripper::open()
{
  motor.forward();
  while (true)
  {
    float current = sensor.getCurrent();
    if (current > treshold)
    {
      motor.stop();
      return true;
    }
  }
}

bool Gripper::close()
{
  motor.backward();
  while (true)
  {
    float current = sensor.getCurrent();
    if (current > treshold)
    {
      motor.stop();
      return true;
    }
  }
}