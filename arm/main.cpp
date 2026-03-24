<<<<<<< HEAD
#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  


int main()
{
    RobotArm magnum("127.0.0.1", 1.0, 1.0);
   magnum.movetool({ 0.4, 0.4, 0.1, 3.14, 0.0, 0.0 });
   magnum.movetool({ 0.4, 0.4, 0.4, 3.14, 0.0, 0.0 });
   magnum.movetool({ 0.4, -0.4, 0.4, 3.14, 0.0, 0.0 });
   magnum.movetool({ 0.4, -0.4, 0.1, 3.14, 0.0, 0.0 });

  
  
  


    

    return 0;
=======
#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  


int main()
{
    RobotArm magnum("127.0.0.1", 1.0, 1.0);

  
   magnum.movetool({ 0.4, 0.4, 0.1, 3.14, 0.0, 0.0 });
   magnum.movetool({ 0.4, -0.4, 0.1, 3.14, 0.0, 0.0 });
   magnum.movetool({ 0.4, -0.4, 0.2, 3.14, 0.0, 0.0 });
   magnum.movetool({ 0.4, 0.4, 0.2, 3.14, 0.0, 0.0 });
  
  


    

    return 0;
>>>>>>> 4172af4f0e4691f0cb5e081c76f0627b777b28a9
}