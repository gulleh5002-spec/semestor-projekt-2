#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
private:
  int E1;
  int M1;

public:
  Motor(int ePin, int mPin);
  bool init();
  void forward();
  void backward();
  void stop();
};

#endif