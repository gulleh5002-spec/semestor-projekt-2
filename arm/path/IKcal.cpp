#include "IKcal.h"

IKcal::IKcal()
{

}


std::vector<std::vector<double>> IKcal::getInvseKinamtiksList(std::vector<double> point, bool debug)
{
    // calculate all the invse kinamtik solotions for a point and returns the soltions 
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


    return JointList;
}

void IKcal::makeTransformMatrix(std::vector<double> moveVector, double T[16])
{
   
    Eigen::Matrix4d T_BASE_TCP = AngelPoseToTransform(moveVector);
    Eigen::Matrix4d T_BASE_TCP = MatrixIKcon(T_BASE_TCP);
    Eigen::Matrix4d T_world_TCP = FindTCP_WORLD(T_BASE_TCP);
    Matrix4dToArry(T_BASE_TCP, T);


}

void IKcal::Matrix4dToArry(Eigen::Matrix4d T, double ARR[16])
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            ARR[row * 4 + col] = T(row, col);
        }
    }
}

Eigen::Matrix4d IKcal::AngelPoseToTransform(std::vector<double> pose)
{
    std::vector<double> T(16);
    //calculate a transformation matrix out via angle axis

    double x = pose[0];
    double y = pose[1];
    double z = pose[2];
    double rx = pose[3];
    double ry = pose[4];
    double rz = pose[5];

    double angle = std::sqrt(rx * rx + ry * ry + rz * rz);


    double R00, R01, R02, R10, R11, R12, R20, R21, R22;

    double kx = rx / angle, ky = ry / angle, kz = rz / angle;
    double c = cos(angle), s = sin(angle), v = 1 - cos(angle);

    R00 = kx * kx * v + c;    R01 = kx * ky * v - kz * s; R02 = kx * kz * v + ky * s;
    R10 = kx * ky * v + kz * s; R11 = ky * ky * v + c;    R12 = ky * kz * v - kx * s;
    R20 = kx * kz * v - ky * s; R21 = ky * kz * v + kx * s; R22 = kz * kz * v + c;


    // ikke standt konvation den bruger  ROS - konvention så derfor står den inverse
    Eigen::Matrix4d T_BASE_TCP;

    T_BASE_TCP << 
    -R02, R00, R01, -x,
    -R12, R10, R11, -y,
    R22, -R20, -R21, z,
    0, 0, 0, 1;

    return T_BASE_TCP;
}
Eigen::Matrix4d IKcal::FindTCP_WORLD(Eigen::Matrix4d(T_TCP_BASE))
{
    Eigen::Matrix4d T_word_Base = repsetory.getT();
    Eigen::Matrix4d T_world_TCP = T_word_Base * T_TCP_BASE;

    return T_world_TCP;
}


std::vector<double> IKcal::Matrix4Todvec(Eigen::Matrix4d T)
{
    std::vector<double> vec(16);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            vec[row * 4 + col] = T(row, col);
        }
    }

    return vec;
}

Eigen::Matrix4d IKcal::MatrixIKcon(Eigen::Matrix4d T)
{
    Eigen::Matrix4d T_BASE_TCP;
    T_BASE_TCP << 
    -T(0, 2), T(0, 0), T(0, 1), -T(0, 3),
    -T(1, 2), T(1, 0), T(1, 1), -T(1, 3),
    T(2, 2), -T(2, 0), -T(2, 1), T(2, 3),
    0, 0, 0, 1;

    return T_BASE_TCP;
}

