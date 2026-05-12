#include "Motor.h"

#include <stdio.h>
#include "pico/stdlib.h"

Motor::Motor(int ePin, int mPin)
{
  E1 = ePin; // enable pin
  M1 = mPin; // direction pin
}

bool Motor::init()
{
  gpio_init(E1);
  gpio_set_dir(E1, GPIO_OUT);

  gpio_init(M1);
  gpio_set_dir(M1, GPIO_OUT);

  gpio_put(E1, 1);

  return true;
}

void Motor::forward()
{
  gpio_put(E1, 1);
  gpio_put(M1, 0);
  sleep_ms(500); // bruger mere strøm ved opstart så for at give den lidt tid
}

void Motor::backward()
{
  gpio_put(E1, 1);
  gpio_put(M1, 0);
  sleep_ms(500);
}

void Motor::stop()
{
  gpio_put(E1, 0);
}
