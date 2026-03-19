#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  


int main()
{
    RobotArm magnum("127.0.0.1", 1.0, 1.0);

   // magnum.home();
   magnum.movetool({ 0.3, 0.0, 0.5, 3.14159, 0.0, 0.0 });


    

    return 0;
}