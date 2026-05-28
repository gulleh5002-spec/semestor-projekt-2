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
    // rotation of the robot base
    double rot = 2.368;
    // takes the robots start postions
    std::vector<double> startPose = rtde_r.getActualTCPPose();
    std::vector<double> StartAngle = rtde_r.getActualQ();
   
    Eigen::Matrix4d T_tcp_flang;
    Eigen::Matrix4d T_tcp_flangrot;
    Eigen::Matrix4d T_tcp_flangtrans;
    Eigen::Matrix4d T_tcp_flangtransrot;


    // i made a mistake when i calculated the grippers transformation matrix so it rotatded in the falnge insted of the gripper end so depended how the gripper
    // roated 90 degres i had to make 2 diffents translations.
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
        }
        else
        {
            xOffset = 0.002;
            yOffset = -0.006;
            
        }
        T_tcp_flangrot   = IKcal.poseToTransform({0, 0, 0, 0, 0, -wrist_angle});
        T_tcp_flangtrans = IKcal.poseToTransform({xOffset, yOffset , 0.2, 0, 0, 0});
        T_tcp_flang = T_tcp_flangrot * T_tcp_flangtrans;
    }

    // this is used when the gripper should pick somthing up form the tray is calibartede so the gripper perfekly pickes up a block for the tray
    // insted of doing this flang 3 could be used an the calibaren clould just be add the the tray matrix.
    if (flange == 1)
    {
        T_tcp_flangrot   = IKcal.poseToTransform({0, 0, 0, 0, 0, rot});
        T_tcp_flangtrans = IKcal.poseToTransform({0.10, -0.034, 0.205, 0, 0, 0});
        T_tcp_flang = T_tcp_flangrot * T_tcp_flangtrans;
    }

    // this is used when the camara trys to get over the qr code so no translation is used becouse the coordnates is seen prom the cmareas point ofview
    if (flange == 2)
    {
        T_tcp_flang = IKcal.poseToTransform({0, 0, 0.2, 0, 0, rot});
    }



    // here is the correct way of diefing the gripper so insted of having the rotaton in the flange it in the end of the matrix. if there was more time flang 1 and 0 will be rpelaced with this one
    if (flange == 3)
    {
        T_tcp_flangrot   = IKcal.poseToTransform({0, 0, 0, 0, 0, rot});
        T_tcp_flangtrans = IKcal.poseToTransform({-0.0015, -0.008 , 0.2, 0, 0, 0});
        T_tcp_flangtransrot = IKcal.poseToTransform({0, 0, 0, 0, 0, koordinatWorld[6]});
        T_tcp_flang = T_tcp_flangrot * T_tcp_flangtrans*T_tcp_flangtransrot;
    }




    Eigen::Matrix4d T_tcp_flangInvser = T_tcp_flang.inverse();


    // take only the first 6 values (x, y, z, rx, ry, rz) and convert to transformation matrix
    Eigen::Matrix4d T_Grid_goal = IKcal.poseToTransform(std::vector<double>(koordinatWorld.begin(), koordinatWorld.begin() + 6));

    // transformation matrix that aligns with the robot base rotation (2.74 rad around Z)
    Eigen::Matrix4d T_base_world = IKcal.poseToTransform({0 ,0 ,0, 0, 0, 2.74});

    // transformation matrix that aligns with the grid frame
    Eigen::Matrix4d T_world_grid = IKcal.poseToTransform(gridFrame);

    // multiply the matrices together to get the full transform from base to goal TCP
    Eigen::Matrix4d T_TCP_goal  = T_base_world * T_world_grid * T_Grid_goal  * T_tcp_flangInvser;

    std::vector<double> goalBase = IKcal.TransformToPose(T_TCP_goal);

    std::vector<std::vector<double>> joints = pf.findPath(startPose, goalBase, StartAngle);

    std::vector<std::vector<double>> path;
    

    // sets a blend and speed og every point.
    double blend = 0.05;
    double decay;

    // defines a dacey so it does not over shoot is taget
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
    // first block difens the movement to pick a block of from a pickup grid or tray
    std::vector<double> newkoordinat2 = koordinat2;
    int Flang1 = GridFrame1.TCP;
    int Flang2 = GridFrame2.TCP;
    std::vector<double> gridFrame1 = GridFrame1.grid_to_base;
    std::vector<double> gridFrame2 = GridFrame2.grid_to_base;
    double speed = 1;
    double acceleration = 1;
    std::vector<double> newkoordinat1 = koordinat1;
    double brikoffset = 0.05;
    double brikoffset1 = 0.05 + koordinat2[2];
    
    newkoordinat1[2] += brikoffset1;
    movetool(newkoordinat1, speed, acceleration, gridFrame1, Flang1);
    
    movetool(koordinat1, speed, acceleration, gridFrame1, Flang1);

    // gripper closes
    gripper.close();
    movetool(newkoordinat1, speed, acceleration, gridFrame1, Flang1);
    

// places a block form above 
    if (method == 0)
    {
       
        newkoordinat2[2] += brikoffset;

        movetool(newkoordinat2, speed, acceleration, gridFrame2, Flang2);

        // gripper open
        
        movetool(koordinat2, speed, acceleration, gridFrame2, Flang2);
        //gripper closes
        
        gripper.open();
        movetool(newkoordinat2, speed, acceleration, gridFrame2, Flang2);
    }
    // places a block form the side in x
    if (method == 1)
    {
        std::vector<double> opkoordinat2 = koordinat2;
        opkoordinat2[2] += brikoffset;

        std::vector<double> upsidekoordinat2 = koordinat2;
        upsidekoordinat2[0] += 0.05;
        upsidekoordinat2[2] += brikoffset;

        std::vector<double> sidekoordinat2 = koordinat2;
        sidekoordinat2[0] += 0.05;
        
        movetool(upsidekoordinat2, speed, acceleration, gridFrame2, Flang2);
        movetool(sidekoordinat2, speed, acceleration, gridFrame2, Flang2);
        movetool(koordinat2, 0.2, 0.05, gridFrame2, Flang2);
        gripper.open();

        movetool(opkoordinat2, speed, acceleration, gridFrame2, Flang2);
    }
     // places a block form the side in y
    if (method == 2)
    {
        std::vector<double> opkoordinat2 = koordinat2;
        opkoordinat2[2] += brikoffset;

        std::vector<double> upsidekoordinat2 = koordinat2;
        upsidekoordinat2[1] += 0.05;
        upsidekoordinat2[2] += brikoffset;

        std::vector<double> sidekoordinat2 = koordinat2;
        sidekoordinat2[1] += 0.05;
        
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
       Gridplace.placeBlock({buildBlocks[i]});

       std::vector<double> coord2 = buildBlocks[i].getCoordnate();
       moveblock(coord1, coord2, Gridblocks, Gridplace, buildBlocks[i].moveMethod);
    }
    
}



// debug funktions
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
void RobotArm::take()
{
    gripper.close();
}

void RobotArm::drop()
{
    
    gripper.open();
}
// debug funktions end

 
// 
std::vector<double> RobotArm::approsemate()
{
    bool valid = true;
    // move to a position above the tray so the camera can see the ArUco marker
    movetool({0.2, 0.2, 0.5, 3.14, 0, 0}, 0.5, 0.5, {0.2, 0.2, 0, 0, 0, 0}, 2);
    std::vector<double> poses = getTrayFrame();
    while(true)
    {
        // sets the z to 0.2 and the rotaton of the gripper around x and y to 0 becuse the qr is under and a rotatin in x and y is an error
        valid = true;
        poses[2] = 0.2;
        poses[3] = 0;
        poses[4] = 0;
        
        poses.push_back(0);
        // move to the position above the tray.
        movetool({0, 0, 0, 3.14, 0, 0}, 0.5, 0.5, {poses}, 2);


        // cheak if the error is smalle valid
        std::vector<ArUcoDetector::ArucoPose> camPoses = camera.detectOnce(1);

        if (abs(camPoses[0].x) > 5.0f || abs(camPoses[0].y) > 5.0f)
        {
            valid = false;
        }

        poses = getTrayFrame();

        if (valid)
        {
            poses = getTrayFrame();
            break;
        }
        


    }
    return poses;
}
std::vector<double> RobotArm::getTrayFrame()
{
    Eigen::Matrix4d T_flange_camera;
   
    // this is where the camara is mounted on the gripper there is only the hight becose the translation is from the camaras point of view
    T_flange_camera = IKcal.poseToTransform({0, 0, 0.158, 0, 0, 2.368});

    Eigen::Matrix4d T_world_base = IKcal.poseToTransform({0, 0, 0, 0, 0, -2.74});

    Eigen::Matrix4d T_base_flange = IKcal.poseToTransform(rtde_r.getActualTCPPose());
    
    std::vector<ArUcoDetector::ArucoPose> poses = camera.detectOnce(1);
    if (poses.empty())
    {
        std::cout << "getTrayFrame: ingen markør fundet!\n";
        return {};
    }
    
    Eigen::Matrix4d T_camera_object = IKcal.poseToTransform({
        poses[0].x / 1000.0,
        poses[0].y / 1000.0,
        poses[0].z / 1000.0,
        poses[0].rvecX,
        poses[0].rvecY,
        poses[0].rvecZ
    });

    // the transfomration train form the world to tray
    Eigen::Matrix4d T_world_object = T_world_base * T_base_flange * T_flange_camera * T_camera_object;

   

    return IKcal.TransformToPose(T_world_object);

    
}
RobotArm::~RobotArm()
{
   
}



