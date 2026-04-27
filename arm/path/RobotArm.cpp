#include "RobotArm.h"
#include <iostream>


//defoult contructer

RobotArm::RobotArm(std::string init_ip, double speed, double acceleration)
:speed(speed),
acceleration(acceleration),
rtde_r(init_ip),
rtde_c(init_ip),
pf()
{   
//rtde_c.setTcp({0, 0, 0.1, 0, 0, 0});
}
//go to home position OBS!! do not take notece of grupper
void RobotArm::home()
{
    
    rtde_c.moveL({ 0.0, 0.5, 0.5, 3.14, 0.0, 0.0 }, 0.5, 0.5);
}

void RobotArm::movetool(std::vector<double> koordinatWorld , double speed, double acceleration, std::vector<double> gridFrame)
{
    
    std::vector<double> startPose = rtde_r.getActualTCPPose();
    std::vector<double> StartAngle = rtde_r.getActualQ();  
    
    Eigen::Matrix4d T_world_goal = IKcal.AngelPoseToTransform(koordinatWorld);
    Eigen::Matrix4d T_base_grid = IKcal.AngelPoseToTransform(gridFrame);
    Eigen::Matrix4d T_base_grid = T_base_grid.inverse();
    Eigen::Matrix4d T_base_goal  = T_base_grid * T_world_goal;

    std::vector<double> goalBase = IKcal.TransformToPose(T_base_goal);  
    
    std::vector<std::vector<double>> joints = pf.findPath(startPose, goalBase, StartAngle);

    std::vector<std::vector<double>> path;
    for (size_t i = 0; i < joints.size(); i++){
        std::vector<double> entry = joints[i];
        for (int j = 0; j < 6; j++)
        {
           //std::cout << ",: " << joints[i][j];
        }
        //std::cout << std::endl;
        entry.push_back(speed);
        entry.push_back(acceleration);
        if (i < joints.size() - 5)
        {
            entry.push_back(0.05);
        }
        else
        {
            entry.push_back(0);
        }
 
        path.push_back(entry);
    }

    rtde_c.moveJ(path);
    std::cout << "new path" << std::endl;
}

void RobotArm::getRTDEinfor()
{
    std::vector<double> q = rtde_r.getActualQ();
    printf("Base:     %.2f deg\n", q[0] * 180.0 / M_PI);
    printf("Shoulder: %.2f deg\n", q[1] * 180.0 / M_PI);
    printf("Elbow:    %.2f deg\n", q[2] * 180.0 / M_PI);
    printf("Wrist 1:  %.2f deg\n", q[3] * 180.0 / M_PI);
    printf("Wrist 2:  %.2f deg\n", q[4] * 180.0 / M_PI);
    printf("Wrist 3:  %.2f deg\n", q[5] * 180.0 / M_PI);
}

void RobotArm::moveblock(std::vector<double> koordinat1, std::vector<double> koordinat2)
{
    // do so the robot move to were the brik is ind the hight of placement koordiante  so i do not colide
    std::vector<double> newkoordinat1 = koordinat1;
    double brikoffset = 0.2;
    newkoordinat1[2] = koordinat2[2] + brikoffset;
    movetool(newkoordinat1, speed, acceleration, gridFrame);
    movetool(koordinat1, speed, acceleration, gridFrame);
    movetool(newkoordinat1, speed, acceleration, gridFrame);

    std::vector<double> newkoordinat2 = koordinat2;
    newkoordinat2[2] += brikoffset;
    
    movetool(newkoordinat2, speed, acceleration, gridFrame);
    movetool(koordinat2, speed, acceleration, gridFrame);
    newkoordinat2[2] += brikoffset;
    movetool(newkoordinat2, speed, acceleration, gridFrame);

    
}

void RobotArm::build(Grid& Gridblocks, Grid& Gridplace, std::vector<Block> Blocks)
{
    
    for (int i = 0; i < Blocks.size(); i++)
    {
       std::vector<double> coord1 = Gridblocks.findBlock(Blocks[i]);
       std::vector<double> coord2 = Blocks[i].getCoordnate();
       Gridblocks.placeBlock(Blocks[i].getCoordnate(), Blocks[i].Id());
       moveblock(coord1, coord2)
    }
    
}

RobotArm::~RobotArm()
{
   
}



