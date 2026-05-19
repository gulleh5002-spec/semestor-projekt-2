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

void RobotArm::movetool(std::vector<double> koordinatWorld , double speed, double acceleration, std::vector<double> gridFrame, int flange)
{
    double rot = 2.368;
    // finder start vinklen og start tcp pose
    std::vector<double> startPose = rtde_r.getActualTCPPose();
    std::vector<double> StartAngle = rtde_r.getActualQ();
    std::cout << "Grid/Block mål: X=" << koordinatWorld[0] << " Y=" << koordinatWorld[1] << " Z=" << koordinatWorld[2]
              << " Rx=" << koordinatWorld[3] << " Ry=" << koordinatWorld[4] << " Rz=" << koordinatWorld[5] << "\n";  
    Eigen::Matrix4d T_tcp_flang;
    Eigen::Matrix4d T_tcp_flangrot;
    Eigen::Matrix4d T_tcp_flangtrans;
    Eigen::Matrix4d T_tcp_flangtransrot;
    // laver målet om til en transformaotns matrice -0.008 y -0.009
    if (flange == 0)
    {
        double wrist_angle = -rot;
        double xOffset = 0;
        double yOffset = 0;
        if (koordinatWorld.size() > 6 && koordinatWorld[6] == -1.57)
        {
        wrist_angle =  koordinatWorld[6] - rot;
        xOffset = -0.006;
        yOffset = 0;
        std::cout << "trun" << std::endl;
        }
        else
        {
        
            std::cout << "not turn" << std::endl;
            xOffset = 0.002;
            yOffset = -0.006;
            
        }
        std::cout << xOffset << std::endl;
        T_tcp_flangrot   = IKcal.poseToTransform({0, 0, 0, 0, 0, -wrist_angle});
        T_tcp_flangtrans = IKcal.poseToTransform({xOffset, yOffset , 0.2, 0, 0, 0});
        T_tcp_flang = T_tcp_flangrot * T_tcp_flangtrans;
        std::cout << "2";
    }
    if (flange == 1)
    {
         T_tcp_flangrot   = IKcal.poseToTransform({0, 0, 0, 0, 0, rot});
        T_tcp_flangtrans = IKcal.poseToTransform({0.10, -0.034, 0.205, 0, 0, 0});
        T_tcp_flang = T_tcp_flangrot * T_tcp_flangtrans;
       //T_tcp_flang = IKcal.poseToTransform({0.001, -0.0, 0.202, 0, 0, rot});
    }
    // + y ind mod mig -x hen mod bordet
    if (flange == 2)
    {
        T_tcp_flang = IKcal.poseToTransform({0, 0, 0.2, 0, 0, rot});
    }
    if (flange == 3)
    {
        T_tcp_flangrot   = IKcal.poseToTransform({0, 0, 0, 0, 0, rot});
        T_tcp_flangtrans = IKcal.poseToTransform({-0.0015, -0.008 , 0.2, 0, 0, 0});
        T_tcp_flangtransrot = IKcal.poseToTransform({0, 0, 0, 0, 0, koordinatWorld[6]});
        T_tcp_flang = T_tcp_flangrot * T_tcp_flangtrans*T_tcp_flangtransrot;
        std::cout << "3";
    }

    Eigen::Matrix4d T_tcp_flangInvser = T_tcp_flang.inverse();

    // gør man kun får de første 6 værdier
    Eigen::Matrix4d T_Grid_goal = IKcal.AngelPoseToTransform(std::vector<double>(koordinatWorld.begin(), koordinatWorld.begin() + 6));

    //  transformaotns matrice  som går den er alignet med bordet 2.74
    Eigen::Matrix4d T_base_world = IKcal.poseToTransform({0 ,0 ,0, 0, 0, 2.74});

    // Transformaons matrice som går den er alignet med gridet
    Eigen::Matrix4d T_world_grid = IKcal.poseToTransform(gridFrame);

    // ganger matricerne sammen så du for vejen til målet
    Eigen::Matrix4d T_TCP_world = T_base_world * T_world_grid * T_Grid_goal;
    std::vector<double> tcpWorldGoal = IKcal.TransformToPose(T_TCP_world);
    std::cout << "TCP mål (base): X=" << tcpWorldGoal[0] << " Y=" << tcpWorldGoal[1] << " Z=" << tcpWorldGoal[2] << "\n";
    Eigen::Matrix4d T_TCP_goal  =  T_TCP_world * T_tcp_flangInvser;
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
        
        if (i < joints.size() - 1)
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
    std::vector<double> actualTCP = rtde_r.getActualTCPPose();
    std::cout << "TCP endelig: X=" << actualTCP[0] << " Y=" << actualTCP[1] << " Z=" << actualTCP[2]
              << " Rx=" << actualTCP[3] << " Ry=" << actualTCP[4] << " Rz=" << actualTCP[5] << "\n";
}




std::vector<double> RobotArm::getTCPPose()
{
    std::vector<double> tcp = rtde_r.getActualTCPPose();
    std::cout << "TCP: X=" << tcp[0] << " Y=" << tcp[1] << " Z=" << tcp[2]
              << " Rx=" << tcp[3] << " Ry=" << tcp[4] << " Rz=" << tcp[5] << "\n";
    return tcp;
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

void RobotArm::moveblock(std::vector<double> koordinat1, std::vector<double> koordinat2, Grid& GridFrame1, Grid& GridFrame2, int method)
{
     std::vector<double> newkoordinat2 = koordinat2;
    int Flang1 = GridFrame1.TCP;
    int Flang2 = GridFrame2.TCP;
    std::vector<double> gridFrame1 = GridFrame1.grid_to_base;
    std::vector<double> gridFrame2 = GridFrame2.grid_to_base;
    double speed = 1;
    double acceleration = 1;
    // do so the robot move to were the brik is ind the hight of placement koordiante  so i do not colide
    std::vector<double> newkoordinat1 = koordinat1;
    double brikoffset = 0.05;
    double brikoffset1 = 0.05 + koordinat2[2];
    
    newkoordinat1[2] += brikoffset1;
    movetool(newkoordinat1, speed, acceleration, gridFrame1, Flang1);
    
    movetool(koordinat1, speed, acceleration, gridFrame1, Flang1);

    // gripper lukker
    gripper.close();
    movetool(newkoordinat1, speed, acceleration, gridFrame1, Flang1);
    


    // placere

    if (method == 0)
    {
       
        newkoordinat2[2] += brikoffset;

        movetool(newkoordinat2, speed, acceleration, gridFrame2, Flang2);

        // gripper åbner
        
        movetool(koordinat2, speed, acceleration, gridFrame2, Flang2);
        //gripper lukker
        
        gripper.open();
        movetool(newkoordinat2, speed, acceleration, gridFrame2, Flang2);
    }
    if (method == 1)
    {
        std::vector<double> opkoordinat2 = koordinat2;
        opkoordinat2[2] += brikoffset;

        std::vector<double> upsidekoordinat2 = koordinat2;
        upsidekoordinat2[0] += 0.05;
        upsidekoordinat2[2] += brikoffset;

        std::vector<double> sidekoordinat2 = koordinat2;
        sidekoordinat2[0] += 0.045;
        
        movetool(upsidekoordinat2, speed, acceleration, gridFrame2, Flang2);
        movetool(sidekoordinat2, speed, acceleration, gridFrame2, Flang2);
        movetool(koordinat2, 0.2, 0.05, gridFrame2, Flang2);
        gripper.open();

        movetool(opkoordinat2, speed, acceleration, gridFrame2, Flang2);
    }
    
    
    
    
    
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
       moveblock(coord1, coord2, Gridblocks, Gridplace, buildBlocks[i].moveMethod);
    }
    
}

void RobotArm::moveToGridPos(Grid grid, Block block, bool m)
{
    if (m)
    {
        movetool(block.getCoordnate(), 0.5, 0.5, grid.grid_to_base, 1);
    }
    else
    {
        movetool(block.getCoordnate(), 0.5, 0.5, grid.grid_to_base, 0);
    }
    
    
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

void RobotArm::findTrayGrid()
{
   
    movetool({0.2, 0.2, 0.5, 3.14, 0, 0}, 0.5, 0.5, {0.2, 0.2, 0, 0, 0, 0}, 2);
    
}
std::vector<double> RobotArm::approsemate()
{
    bool valid = true;
    findTrayGrid();
    std::vector<double> poses = getTrayFrame(true);
    std::vector<double> oldposes = {0,0,0,0,0,0,0};
    while(true)
    {
        valid = true;
        poses[2] = 0.05;
        poses[3] = 0;
        poses[4] = 0;
        
        poses.push_back(0);
        movetool({0, 0, 0, 3.14, 0, 0}, 0.5, 0.5, {poses}, 2);

        std::vector<ArUcoDetector::ArucoPose> camPoses = camera.detectOnce(1);
        if (camPoses.empty())
        {
            poses = getTrayFrame(true);
            continue;
        }

        std::cout << "Camera X=" << camPoses[0].x << "mm Y=" << camPoses[0].y << "mm\n";

        if (abs(camPoses[0].x) > 5.0f || abs(camPoses[0].y) > 5.0f)
        {
            valid = false;
        }

        poses = getTrayFrame(true);

        if (valid)
        {
            poses = getTrayFrame(false);
            //ArUcoDetector camera;
            //camera.run();
            break;
        }
        


    }
    return poses;
}
std::vector<double> RobotArm::getTrayFrame(bool onCamara)
{
    // -0.0265
    Eigen::Matrix4d T_flange_camera;
    if (onCamara)
    {
        T_flange_camera = IKcal.poseToTransform({0, 0, 0.158, 0, 0, 2.368});
    }
    else
    {
        T_flange_camera = IKcal.poseToTransform({0, 0, 0.158, 0, 0, 2.368});
    }
    Eigen::Matrix4d T_world_base = IKcal.poseToTransform({0, 0, 0, 0, 0, -2.74});
    Eigen::Matrix4d T_base_flange = IKcal.poseToTransform(rtde_r.getActualTCPPose());
    
    std::vector<ArUcoDetector::ArucoPose> poses = camera.detectOnce(1);
    if (poses.empty())
    {
        std::cout << "getTrayFrame: ingen markør fundet!\n";
        return {};
    }
    std::cout << "Camera: X=" << poses[0].x << " Y=" << poses[0].y << " Z=" << poses[0].z << "mm"
              << " rvecX=" << poses[0].rvecX << " rvecY=" << poses[0].rvecY << " rvecZ=" << poses[0].rvecZ << "\n";
    Eigen::Matrix4d T_camera_object = IKcal.poseToTransform({
        poses[0].x / 1000.0,
        poses[0].y / 1000.0,
        poses[0].z / 1000.0,
        poses[0].rvecX,
        poses[0].rvecY,
        poses[0].rvecZ
    });
    Eigen::Matrix4d T_world_object = T_world_base * T_base_flange * T_flange_camera * T_camera_object;

   

    return IKcal.TransformToPose(T_world_object);
}



