#include "RobotArm.h"
#include <iostream>


//defoult contructer

RobotArm::RobotArm(std::string init_ip, double speed, double acceleration)
:speed(speed),
acceleration(acceleration),
rtde_r(init_ip),
rtde_c(init_ip),
gripper("172.20.10.8"),
pf()
{
    //rtde_c.setTcp({0, 0, 0.264, 0, 0, 0});
}
//go to home position OBS!! do not take notece of grupper
void RobotArm::home()
{
    
    rtde_c.moveL({ 0.0, 0.5, 0.5, 3.14, 0.0, 0.0 }, 0.5, 0.5);
}

void RobotArm::movetool(std::vector<double> koordinatWorld , double speed, double acceleration, std::vector<double> gridFrame)
{
    // finder start vinklen og start tcp pose
    std::vector<double> startPose = rtde_r.getActualTCPPose();
    std::vector<double> StartAngle = rtde_r.getActualQ();  
    
    // laver målet om til en transformaotns matrice -0.008
    double wrist_angle = 0;
    double xOffset = 0;

    if (koordinatWorld[6] == -1.57)
    {
       wrist_angle = koordinatWorld[6];
       xOffset = -0.006;
       std::cout << "trun" << std::endl;
    }
    else
    {
      
        std::cout << "not turn" << std::endl;
        wrist_angle = 0;
        xOffset = 0.001;
        
    }
    std::cout << xOffset << std::endl;
    Eigen::Matrix4d T_tcp_flang = IKcal.poseToTransform({xOffset, -0.009, 0.2, 0, 0, -wrist_angle});

    Eigen::Matrix4d T_tcp_flangInvser = T_tcp_flang.inverse();

    // gør man kun får de første 6 værdier
    Eigen::Matrix4d T_Grid_goal = IKcal.AngelPoseToTransform(std::vector<double>(koordinatWorld.begin(), koordinatWorld.begin() + 6));

    //  transformaotns matrice  som går den er alignet med bordet 2.74
    Eigen::Matrix4d T_base_world = IKcal.poseToTransform({0 ,0 ,0, 0, 0, 2.74});

    // Transformaons matrice som går den er alignet med gridet
    Eigen::Matrix4d T_world_grid = IKcal.poseToTransform(gridFrame);

    // ganger matricerne sammen så du for vejen til målet
    Eigen::Matrix4d T_TCP_goal  =  T_base_world * T_world_grid *  T_Grid_goal * T_tcp_flangInvser;
    std::vector<double> goalBase = IKcal.TransformToPose(T_TCP_goal);  
    std::vector<std::vector<double>> joints = pf.findPath(startPose, goalBase, StartAngle);

    std::vector<std::vector<double>> path;
    
    double blend = 0.05;
    double decay;
    if (joints.size() > 5)
    {
       
        decay = blend/5;
    }
    else
    {
        decay = blend/joints.size();
    }
    
    for (size_t i = 0; i < joints.size(); i++)
    {
        std::vector<double> entry = joints[i];
       
        entry.push_back(speed);
        entry.push_back(acceleration);
        
        if (i < joints.size() - 2)
        {
            if (joints.size() - i < 5)
            {
                blend -= decay;
            }
            
            entry.push_back(blend);
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

void RobotArm::moveblock(std::vector<double> koordinat1, std::vector<double> koordinat2, std::vector<double> gridFrame1, std::vector<double> gridFrame2)
{
    double speed = 1;
    double acceleration = 1;
    // do so the robot move to were the brik is ind the hight of placement koordiante  so i do not colide
    std::vector<double> newkoordinat1 = koordinat1;
    double brikoffset = 0.2;
    newkoordinat1[2] += brikoffset;
    movetool(newkoordinat1, speed, acceleration, gridFrame1);

    std::cout << "ping";
    
    movetool(koordinat1, speed, acceleration, gridFrame1);

    // gripper lukker
    gripper.close();
    movetool(newkoordinat1, speed, acceleration, gridFrame1);
    
    std::vector<double> newkoordinat2 = koordinat2;
    newkoordinat2[2] += brikoffset;

    movetool(newkoordinat2, speed, acceleration, gridFrame2);

    // gripper åbner
    
    movetool(koordinat2, speed, acceleration, gridFrame2);
    //gripper lukker
    
    gripper.open();
    movetool(newkoordinat2, speed, acceleration, gridFrame2);

    
    
}

void RobotArm::build(Grid& Gridblocks, Grid& Gridplace, std::vector<Block> buildBlocks, std::vector<Block> takeBlocks)
{
    
    takeBlocks = compile.compiletake(takeBlocks);
    Gridblocks.placeBlock(takeBlocks);
    buildBlocks = compile.compileplace(buildBlocks);

    for (int i = 0; i < buildBlocks.size(); i++)
    {
       std::vector<double> coord1 = Gridblocks.findBlock(takeBlocks[i]);
       for (int j = 0; j < coord1.size(); j++)
       {
        std::cout << coord1[j] <<std::endl;
       }
       Gridplace.placeBlock({buildBlocks[i]});

       std::vector<double> coord2 = buildBlocks[i].getCoordnate();
       moveblock(coord1, coord2, Gridblocks.grid_to_base, Gridplace.grid_to_base);
    }
    
}

void RobotArm::moveToGridPos(Grid grid, Block block)
{
    movetool(block.getCoordnate(), 0.5, 0.5, grid.grid_to_base);
}

RobotArm::~RobotArm()
{
   
}

void RobotArm::take()
{
    gripper.close();
}

void RobotArm::drop()
{
    
    gripper.open();
}


