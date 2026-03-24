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
    std::vector<std::vector<double>> joints = executeblejoints(koordinat);
    std::vector<std::vector<double>> path;
    for (size_t i = 0; i < joints.size(); i++)
    {
        std::vector<double> entry = joints[i];
        for (int j = 0; j < 6; j++)
        {
            std::cout << ",: " << joints[i][j];
        }
        std::cout << std::endl;
        entry.push_back(speed);        // velocity
        entry.push_back(acceleration); // acceleration
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
     
    
            //std::cout << ":x " << bestSol[0] << "y: " << bestSol[1] << "z " << bestSol[2] << "rx: " << bestSol[3] << "ry: " << bestSol[4] << "rx: " << bestSol[5] << std::endl;
            
            //std::cout << ":x " << aprovedjoints[i][0] << "y: " << aprovedjoints[i][1] << "z " << aprovedjoints[i][2] << "rx: " << aprovedjoints[i][3] << "ry: " << aprovedjoints[i][4] << "rx: " << aprovedjoints[i][5] << std::endl;
        
        
    
    
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
            for (int i = 0; i < newpath.size(); i++)
            {
            //std::cout << i <<std::endl;
            //getTcpInfo(newpath[i]);
            }
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

std::vector<std::vector<double>> RobotArm::getInvseKinamtiksList(std::vector<double> point, bool debug)
{   
    double T[16];
    double joints[8 * 6];
    std::vector <std::vector<double>> JointList;

    makeTransformMatrix(point, T);
    int num_sol = ur_kinematics::inverse(T, joints);
    if (num_sol != 0)
    { 
        for (int i = 0; i < num_sol; i++)
        {
            std::vector<double> solution(joints + i * 6, joints + i * 6 + 6);
        
            JointList.push_back(solution);
            if (debug)
            {
                if (num_sol != 0)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        std::cout << "  angle:" << j << ":  " << solution[j];
                    }
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << "emty" << std::endl;
                }
                
                
                
            
            
            }
        }
    }
    
    //std::cout << "number of sol" << num_sol << std::endl;
    return JointList;
}

//imortens
std::vector<std::vector<std::vector<double>>> RobotArm::getPreferedInvseKinamtikSoltions(std::vector<std::vector<double>> points, int sol)
{
    std::vector<std::vector<std::vector<double>>>preferedJointsList;
    for (int i = 0; i < points.size(); i++)
    {
        //std::cout << "  index:  " << i << std::endl;
        std::vector <std::vector<double>> JointList = getInvseKinamtiksList(points[i], false);
        if (JointList.empty())
        {
            std::cout << "emty"<< std::endl;
            preferedJointsList.push_back({});
            
        }
        else
        {
            std::vector <std::vector<double>>sortetdList = solutuonSorter(JointList, "albowOp");
            preferedJointsList.push_back(sortetdList);
      
        }
            

    }
    return preferedJointsList ;
}
std::vector<std::vector<double>> RobotArm::solutuonSorter(std::vector<std::vector<double>> JointList, std::string sol)
{
    std::vector<std::vector<double>> sortetPointList;
    for (int i = 0; i < JointList.size(); i++)
    {
        if (sol == "albowOp")
        {
            if (JointList[i][1] > M_PI)
            {
                sortetPointList.push_back(JointList[i]);
            }
        }
    }
    return sortetPointList;

}
std::vector<std::vector<double>> RobotArm::executeblejoints(std::vector<double> koordinat)
{
    std::vector<std::vector<double>> executeblejointslist;
    std::vector<std::vector<double>> aprovedMove = validMove(koordinat, 20);
    std::cout << "aprovedMove" << aprovedMove.size() << std::endl;

    std::vector<std::vector<std::vector<double>>> aprovedjoints = getPreferedInvseKinamtikSoltions(aprovedMove, 5);
    std::cout << "aprovedjoints" << aprovedjoints.size() << std::endl;

    // lav så du ikke bruger chats nomelzer men kigger på inden du tjekke distance om og hvis -2pi og det så bliver korter
    std::vector<double> lastSol = rtde_r.getActualQ();
    std::vector<std::vector<double>> normlist;
    for (int l = 0; l < 6; l++)
    {
        std::cout << "lastSolstart: " << lastSol[l] << std::endl;
    }

    for (int i = 0; i < aprovedjoints.size(); i++)
    {

        if (!aprovedjoints[i].empty())
        {
            std::vector<double> bestSol;
            double last = 10;

            for (int k = 0; k < aprovedjoints[i].size(); k++)
            {
                double sum = 0;
                for (int s = 0; s < 6; s++)
                {
                    double a = aprovedjoints[i][k][s];
                    double b = lastSol[s];
                    double diff = abs(a - b);
                    double aNorm = a - 2 * M_PI;
                    double diffnorm = abs(aNorm - b);

                    if (diff > diffnorm)
                    {
                        sum += diffnorm;
                        aprovedjoints[i][k][s] = aNorm;
                    }
                    else
                    {
                        sum += diff;
                    }


                }

                if (sum < last)
                {
                    last = sum;
                    bestSol = aprovedjoints[i][k];  // ← gem original

                }
            }

            // Normaliser til -π til π inden moveJ


            lastSol = bestSol;
            executeblejointslist.push_back(bestSol);

        }
    }
    return executeblejointslist;
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



