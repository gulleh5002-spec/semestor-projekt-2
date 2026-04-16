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

}
//go to home position OBS!! do not take notece of grupper
void RobotArm::home()
{
    
    std::vector<double> joints = { 0, -1.5708, 0, -1.5708, 0, 0 };
    rtde_c.moveJ(joints, 0.5, 0.5);
}

void RobotArm::movetool(std::vector<double> koordinat, double speed, double acceleration)
{
    std::vector<double> tcp = repsetory.getTCP();
    
    std::vector<double> startPose = rtde_r.getActualTCPPose();
    std::vector<double> StartAngle = rtde_r.getActualQ();
    Eigen::Matrix4d startPose_T = IKcal.AngelPoseToTransform(startPose);
    

    startPose[0] = startPose_T(0, 3);
    startPose[1] = startPose_T(1, 3);
    startPose[2] = startPose_T(2, 3);
    
    std::cout << "startPose: " << startPose[0] << ", " << startPose[1] << ", " << startPose[2] << std::endl;


    std::vector<std::vector<double>> joints = pf.findPath(startPose, koordinat, StartAngle);
    std::vector<std::vector<double>> path;
    for (size_t i = 0; i < joints.size(); i++)
    {
        std::vector<double> entry = joints[i];
        for (int j = 0; j < 6; j++)
        {
           // std::cout << ",: " << joints[i][j];
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
    double brikoffset = 0.05;
    newkoordinat1[2] = koordinat2[2] + brikoffset;
    movetool(newkoordinat1);
    movetool(koordinat1);
    movetool(newkoordinat1);
    std::vector<double> newkoordinat2 = koordinat2;
    newkoordinat2[2] += brikoffset;
    movetool(newkoordinat2);
    movetool(koordinat2);


    
}

RobotArm::~RobotArm()
{
   
}



