#include "RobotArm.h"
#include <iostream>


//defoult contructer

RobotArm::RobotArm(std::string init_ip, double init_gridWidth, double init_gridLength)

:gridWidth(init_gridWidth),
gridLength(init_gridLength),
rtde_r(init_ip),
rtde_c(init_ip)
{   
}



void RobotArm::home()
{
    std::vector<double> joints = { 0, -1.5708, 1.5708, -1.5708, -1.5708, 0 };
    rtde_c.moveJ(joints, 0.5, 0.5);
}

void RobotArm::movetool(std::vector<double> koordinat, double speed, double acceleration)
{
    std::vector<std::vector<double>> aprovedMove = validMove(koordinat, 20);
    std::vector<bool> gimbleAprovedList = gimbleLocktjek(aprovedMove);


    for (int i = 1; i < aprovedMove.size(); i++)
    {
        if (gimbleAprovedList[i] != gimbleAprovedList[i - 1])
        {
            if (gimbleAprovedList[i-  1] == true)
            {
                std::cout << i << "L" <<std::endl;
                rtde_c.moveL(aprovedMove[i], speed, acceleration);
            }
            if (gimbleAprovedList[i - 1] == false)
            {
                std::cout << i << "L" << std::endl;
                rtde_c.moveJ_IK(aprovedMove[i], speed, acceleration);
            }
        }   
    }
    // Bevæg til det sidste punkt
    if (gimbleAprovedList.back() == true)
        rtde_c.moveL(aprovedMove.back(), speed, acceleration);
    else
        rtde_c.moveJ_IK(aprovedMove.back(), speed, acceleration);
}

std::vector<double> RobotArm::tcpPose()
{
    return rtde_r.getActualTCPPose();
}



//make if there is no valid path
std::vector<std::vector<double>> RobotArm::validMove(std::vector<double> koordinat, double tcpRadius)
{
    
    
    double resolution = 0.01;
    bool gimbletjek = false;
    bool allcheck = false;
    
   
    std::vector<double> startPose = tcpPose();
   
   while(true)
   { 
        std::vector<std::vector<double>> newpath;
        std::vector<double> gimbleJoints;
        bool validpath = true;
        
        double dx = koordinat[0] - startPose[0];
        double dy = koordinat[1] - startPose[1];
        double dz = koordinat[2] - startPose[2];

        double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
        int points = std::ceil(distance / resolution);
        std::cout << points << std::endl;
    
        for (int i = 0; i <= points; i++)
        {
            double t = (double)i / points;
            double x = (startPose[0] + t * dx) * 1000;
            double y = (startPose[1] + t * dy) * 1000;
            double z = (startPose[2] + t * dz) * 1000;
            std::vector<double> pointPose = { x / 1000, y / 1000, z / 1000, koordinat[3], koordinat[4], koordinat[5] };
            newpath.push_back(pointPose);
            if (x + tcpRadius >= basexLow && x - tcpRadius <= basexHigh &&
                y + tcpRadius >= baseyLow && y - tcpRadius <= baseyHigh &&
                z + tcpRadius >= basezLow && z - tcpRadius <= basezHigh)
            {
               newpath = {};
               std::cout << "fail" << std::endl;
               koordinat[2] += 0.1;
               validpath = false;
               break;
               
            }
        }
       
        if(validpath)
        {
            std::cout << newpath.size() << std::endl;
            
            return newpath;
        }
   }
   
    
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

void RobotArm::getTcpInfo(std::vector<double> point)
{
    std::vector<std::vector<double>> alle_løsninger;
    double T[16];
    double joints[8 * 6];

    makeTransformMatrix(point, T);
    int num_sol = ur_kinematics::inverse(T, joints);



    for (int j = 0; j < num_sol; j++)
    {
        std::vector<double> sol(joints + j * 6, joints + j * 6 + 6);
        alle_løsninger.push_back(sol);
    }

    for (int j = 0; j < alle_løsninger.size(); j++)
    {
        printf("Løsning %d: ", j);
        for (int k = 0; k < 6; k++)
        {
            printf("%.2f ", alle_løsninger[j][k] * 180.0 / M_PI);
        }
        printf("\n");
    }
}

bool RobotArm::gimblelock(double joints[6])
{
    double thresholdDeg = 5;

    // omregner til radianer
    double t = thresholdDeg * M_PI / 180.0;


    if (std::abs(joints[4]) < t ||
        std::abs(std::abs(joints[4]) - M_PI / 2) < t ||
        std::abs(std::abs(joints[4]) - M_PI) < t)
    {
        return true;
    }

    // Elbow singularitet
    if (std::abs(joints[2]) < t ||
        std::abs(std::abs(joints[2]) - M_PI) < t)
    {
        return true;
    }

    // Shoulder singularitet
    if (std::abs(std::abs(joints[1] + joints[2]) - M_PI) < t)
    {
        return true;
    }

    return false;
  
    
    
        
}

std::vector<bool> RobotArm::gimbleLocktjek(std::vector<std::vector<double>> points)
{
    std::vector<bool> gimblepoints;

    bool out = false;
    double T[16];
    double joints[8 * 6];

    for (int i = 0; i < points.size(); i++)
    {  
        // if there is a gimble lock the last point will be added as a cheakpoint
        makeTransformMatrix(points[i], T);
        int num_sol = ur_kinematics::inverse(T, joints);
        
        if (num_sol == 0)
        {
            gimblepoints.push_back(false);
            std::cout << num_sol << "gimbel" << i << std::endl;
            
        }
        if (num_sol != 0)
        {
            bool foundSafe = false;
            for (int j = 0; j < num_sol; j++)
            {
               
                if (!gimblelock(&joints[6 * j]))
                {

                    foundSafe = true;
                    break;
                    

                }
            }
            if (!foundSafe)
            {
                gimblepoints.push_back(false);
                std::cout << " gimble:  " << i << std::endl;
            }
            else
            {
                gimblepoints.push_back(true);
            }
        }
       
    }
    for (int i = 0; i < gimblepoints.size(); i++)
    {
        std::cout << gimblepoints[i] << "  index:  " << i << std::endl;
    }

    return gimblepoints;
}

void RobotArm::makeTransformMatrix(std::vector<double> moveVector, double T[16])
{

    // angels axis
    double x = moveVector[0];
    double y = moveVector[1];
    double z = moveVector[2];
    double rx = moveVector[3];
    double ry = moveVector[4];
    double rz = moveVector[5];

    double angle = std::sqrt(rx * rx + ry * ry + rz * rz);

    double R00, R01, R02, R10, R11, R12, R20, R21, R22;

    double kx = rx / angle, ky = ry / angle, kz = rz / angle;
    double c = cos(angle), s = sin(angle), v = 1 - cos(angle);

    R00 = kx * kx * v + c;    R01 = kx * ky * v - kz * s; R02 = kx * kz * v + ky * s;
    R10 = kx * ky * v + kz * s; R11 = ky * ky * v + c;    R12 = ky * kz * v - kx * s;
    R20 = kx * kz * v - ky * s; R21 = ky * kz * v + kx * s; R22 = kz * kz * v + c;
    

    // ur_kinematics::inverse læser i denne rækkefølge:
    // -T02  T00  T01  -T03
    // -T12  T10  T11  -T13
    //  T22 -T20 -T21   T23
    T[0] = -R02; T[1] = R00; T[2] = R01; T[3] = -x;
    T[4] = -R12; T[5] = R10; T[6] = R11; T[7] = -y;
    T[8] = R22; T[9] = -R20; T[10] = -R21; T[11] = z;
    T[12] = 0;   T[13] = 0;   T[14] = 0;   T[15] = 1;
}

RobotArm::~RobotArm()
{
   
}



