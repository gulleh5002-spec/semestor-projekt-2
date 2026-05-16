#pragma once

#include <Arduino.h>
#include <Servo.h>

class ServoGripper
{
public:
    ServoGripper(int servoPin, int closeAngle = 90, int openAngle = 0);

    void begin();                    // attach servo and move to open position
    void moveSlowly(int targetAngle, int stepDelay = 10);
    void open();                     // move to open angle
    void close();                    // move to grip angle
    void setAngle(int angle);        // move to any specific angle
    void setCloseAngle(int angle);    // update open angle
    void setOpenAngle(int angle);   // update close angle
    int  getCurrentAngle();          // returns current angle

private:
    Servo _servo;
    int   _servoPin;
    int   _closeAngle;
    int   _openAngle;
    int   _currentAngle;
};
