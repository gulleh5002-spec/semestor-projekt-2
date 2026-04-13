#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  


int main()
{
    //RobotArm gulle("192.168.1.11", 1.0, 1.0);
    RobotArm magnum("127.0.0.1", 1.0, 1.0);
    std::vector<double> point1 = { 0.4, 0.4, 0.1, 3.14, 0.0, 0.0 };
    std::vector<double> point2 = { 0.2, -0.2, 0.2, 3.14, 0.0, 0.0 };
    //gulle.moveblock(point1, point2);
    //magnum.moveblock(point1, point2);
    magnum.movetool(point1);
    //gulle.movetool({ 0.0, 0.3, 0.1, 3.14, 0.0, 0.0 });

    return 0;
}