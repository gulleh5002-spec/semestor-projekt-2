#include "ServoGripper.h"

ServoGripper::ServoGripper(int servoPin, int closeAngle, int openAngle)
{
    _servoPin     = servoPin;
    _closeAngle    = closeAngle;
    _openAngle   = openAngle;
    _currentAngle = openAngle;
}

void ServoGripper::begin()
{
    // attach(pin, minPulseWidth, maxPulseWidth)
    _servo.attach(_servoPin, 500, 2500);
    _servo.write(_openAngle); // turns angles into pulses
    _currentAngle = _openAngle;
    delay(500);
}

void ServoGripper::moveSlowly(int targetAngle, int stepDelay) // Waits a small amount between each step, that is set as openAngle and closeAngle
//for smoother action
{
    if (targetAngle > _currentAngle)
    {
        for (int angle = _currentAngle; angle <= targetAngle; angle++)
        {
            _servo.write(angle);
            delay(stepDelay);
        }
    }
    else
    {
        for (int angle = _currentAngle; angle >= targetAngle; angle--)
        {
            _servo.write(angle);
            delay(stepDelay);
        }
    }

    _currentAngle = targetAngle;
}

void ServoGripper::open()
{
    moveSlowly(_openAngle, 15); // slower = larger delay
    _currentAngle = _openAngle;
}

void ServoGripper::close()
{
    moveSlowly(_closeAngle, 15);
    _currentAngle = _closeAngle;
}

void ServoGripper::setAngle(int angle)
{
    
    if (angle < 0)   angle = 0;
    if (angle > 180) angle = 180;

    moveSlowly(angle, 15);
    _currentAngle = angle;
}

void ServoGripper::setOpenAngle(int angle)
{
    _openAngle = angle;
}

void ServoGripper::setCloseAngle(int angle)
{
    _closeAngle = angle;
}

int ServoGripper::getCurrentAngle()
{
    return _currentAngle;
}
