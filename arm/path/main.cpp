#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  


int main()
{
    RobotArm gulle("192.168.1.100", 1, 1.0);
    //RobotArm magnum("127.0.0.1", 1.0, 1.0);
    std::vector<double> point1 = { 0.4, 0.4, 0.1, 3.14, 0.0, 0.0 };
    std::vector<double> point2 = { 0.2, 0.2, 0.3, 3.14, 0.0, 0.0 };
    //gulle.moveblock(point1, point2);
    gulle.moveblock(point1, point2);
    //magnum.movetool(point1);
    //gulle.movetool({ 0, 0.4, 0.5, 3.14, 0.0, 0.0 });
    //gulle.movetool({ 0.4, 0, 0.5, 3.14, 0.0, 0.0 });
    //gulle.home();
    return 0;
}