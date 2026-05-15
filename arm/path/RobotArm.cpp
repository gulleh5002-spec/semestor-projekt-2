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
    // finder start vinklen og start tcp pose
    std::vector<double> startPose = rtde_r.getActualTCPPose();
    std::vector<double> StartAngle = rtde_r.getActualQ();  
    Eigen::Matrix4d T_tcp_flang;
    
    // laver målet om til en transformaotns matrice -0.008 y -0.009
    if (flange == 0)
    {
        double wrist_angle = 0;
        double xOffset = 0;

        if (koordinatWorld.size() > 6 && koordinatWorld[6] == -1.57)
        {
        wrist_angle = koordinatWorld[6];
        xOffset = -0.006;
        std::cout << "trun" << std::endl;
        }
        else
        {
        
            std::cout << "not turn" << std::endl;
            wrist_angle = 0;
            xOffset = 0.0015;
            
        }
        std::cout << xOffset << std::endl;
        T_tcp_flang = IKcal.poseToTransform({xOffset, 0.00, 0.2, 0, 0, -wrist_angle});
    }
    if (flange == 1)
    {
         T_tcp_flang = IKcal.poseToTransform({0.005, -0.030, 0.2, 0, 0, 0});
    }
    if (flange == 2)
    {
        T_tcp_flang = IKcal.poseToTransform({0, 0, 0.2, 0, 0, 0});
    }

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

void RobotArm::moveblock(std::vector<double> koordinat1, std::vector<double> koordinat2, std::vector<double> gridFrame1, std::vector<double> gridFrame2, int method)
{
    double speed = 1;
    double acceleration = 1;
    // do so the robot move to were the brik is ind the hight of placement koordiante  so i do not colide
    std::vector<double> newkoordinat1 = koordinat1;
    double brikoffset = 0.2;
    newkoordinat1[2] += brikoffset;
    movetool(newkoordinat1, speed, acceleration, gridFrame1);
    
    movetool(koordinat1, speed, acceleration, gridFrame1);

    // gripper lukker
    gripper.close();
    movetool(newkoordinat1, speed, acceleration, gridFrame1);
    


    // placere

    if (method == 0)
    {
        std::vector<double> newkoordinat2 = koordinat2;
        newkoordinat2[2] += brikoffset;

        movetool(newkoordinat2, speed, acceleration, gridFrame2);

        // gripper åbner
        
        movetool(koordinat2, speed, acceleration, gridFrame2);
        //gripper lukker
        
        gripper.open();
        movetool(newkoordinat2, speed, acceleration, gridFrame2);
    }
    if (method == 1)
    {
        std::vector<double> opkoordinat2 = koordinat2;
        opkoordinat2[2] += brikoffset;

        std::vector<double> upsidekoordinat2 = koordinat2;
        upsidekoordinat2[0] += 0.05;
        upsidekoordinat2[2] += brikoffset;

        std::vector<double> sidekoordinat2 = koordinat2;
        sidekoordinat2[0] += 0.05;

        movetool(upsidekoordinat2, speed, acceleration, gridFrame2);
        movetool(sidekoordinat2, speed, acceleration, gridFrame2);
        movetool(koordinat2, 0.2, 0.05, gridFrame2);
        gripper.open();

        movetool(opkoordinat2, speed, acceleration, gridFrame2);
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
       moveblock(coord1, coord2, Gridblocks.grid_to_base, Gridplace.grid_to_base, buildBlocks[i].moveMethod);
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
        poses[2] = 0.15;
        poses[3] = 3.14;
        poses[4] = 0;
        poses[5] = 0;
        poses.push_back(0);
        movetool(poses, 0.5, 0.5, {0,0,0,0,0,0}, 2);

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
            poses = getTrayFrame(true);
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
        T_flange_camera = IKcal.poseToTransform({0, 0, 0.158, 0, 0, 0});
    }
    else
    {
        T_flange_camera = IKcal.poseToTransform({-0.0265, 0, 0.158, 0, 0, 0});
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

    std::vector<double> endpose = IKcal.TransformToPose(T_world_object);
    return endpose;
}



