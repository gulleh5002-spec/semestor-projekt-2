#include "RobotArm.h"


//defoult contructer

RobotArm::RobotArm(std::string init_ip, double init_gridWidth, double init_gridLength)
:ip(init_ip), gridWidth(init_gridWidth), gridLength(init_gridLength),
rtde_r(init_ip), rtde_c(init_ip)
{
    
}


void RobotArm::home()
{
    std::vector<double> joints = { 0, -1.5708, 1.5708, -1.5708, -1.5708, 0 };
    rtde_c.moveJ(joints, 0.5, 0.5);
}

void RobotArm::movetool(std::vector<double> koordinat, double speed, double acceleration)
{
    rtde_c.moveL({koordinat}, speed, acceleration);
}








RobotArm::~RobotArm()
{
   
}



