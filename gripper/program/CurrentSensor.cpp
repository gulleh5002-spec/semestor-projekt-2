#include "CurrentSensor.h"

#include <stdio.h>        // printf
#include "pico/stdlib.h"  // PICO funktioner
#include "hardware/adc.h" // ADC

CurrentSensor::CurrentSensor(int ADCpin, float sens) // constructor
{
  pin = ADCpin;
  sensitivity = sens;
}

bool CurrentSensor::init()
{
  adc_init();
  adc_gpio_init(pin);
  adc_select_input(0);

  printf("Calibrating...\n");
  sleep_ms(2000);

  noLoadVoltage = calibrateZero();
  printf("Zero offset: %.3f V\n", noLoadVoltage);
  return true;
}

float CurrentSensor::getCurrent()
{
  float voltage = readVoltage();
  float correctedVoltage = voltage * 2.0f; // pga af spændingsdeler
  float current = (correctedVoltage - noLoadVoltage) / sensitivity;

  current = abs_val(current);
  if (current < 0.1f)
    current = 0;

  // printf("Current: %.2f A\n", current);
  return abs_val(current);
}

bool CurrentSensor::currentThreshold(float threshold)
{
  float current = getCurrent();
  if (current >= threshold)
  {
    printf("Treshold: true | Current: %.2f A\n", current); // til debug
    return true;
  }
  printf("Treshold: false | Current: %.2f A\n", current); // til debug
  return false;
}

float CurrentSensor::calibrateZero()
{
  float sum = 0;
  int samples = 100;

  for (int i = 0; i < samples; i++)
  {
    uint16_t raw = adc_read();
    float voltage = raw * 3.3f / 4095.0f;
    sum += voltage;
    sleep_ms(5);
  }

  return (sum / samples) * 2.0f;
}

float CurrentSensor::readVoltage()
{
  float sum = 0;
  int samples = 50;

  for (int i = 0; i < samples; i++)
  {
    sum += adc_read();
    sleep_us(200);
  }

  float avg = sum / samples;
  return avg * 3.3f / 4095.0f;
}

float CurrentSensor::abs_val(float x)
{
  return (x < 0) ? -x : x;
}