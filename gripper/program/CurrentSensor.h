#ifndef CURRENTSENSOR_H
#define CURRENTSENSOR_H

class CurrentSensor
{
private:
  float calibrateZero();
  float readVoltage();
  int pin = 0;
  float sensitivity = 0.185;
  float noLoadVoltage;
  float abs_val(float x);

public:
  CurrentSensor(int ADCpin, float sens);
  bool init();
  float getCurrent();
  bool currentThreshold(float threshold);
};

#endif