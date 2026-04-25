#include "ObjDet.h"


ObjDet::ObjDet(double xLow, double xHigh, double yLow, double yHigh, double zLow, double zHigh)

{

}

std::vector<std::vector<double>> ObjDet::objektAvoid(std::vector<std::vector<double>> path, double tcpRadius)
{
    
    bool validpath;
 
    while(true)
    { 
        for (size_t i = 0; i < path.size(); i++)
        {
            double x = path[i][0];
            double y = path[i][1];
            double z = path[i][2];
            validpath = true;
            if (x + tcpRadius >= xLow && x - tcpRadius <= xHigh &&
                y + tcpRadius >= yLow && y - tcpRadius <= yHigh &&
                z + tcpRadius >= zLow && z - tcpRadius <= zHigh)
            {
                
                std::cout << "fail" << std::endl;
                // moves the z koordnate untill the tcp do not collide
                path[i][2] += 0.1;
                validpath = false;
                break;

            }


            if (validpath)
            {
                return path;
            }
        }
    }
    


}
