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
    std::vector<double> TCP = repsetory.getTCP();
   //calculate a transformation matrix out via angle axis
   
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

    T[0] = -R02; T[1] = R00; T[2] = R01; T[3] = -x;
    T[4] = -R12; T[5] = R10; T[6] = R11; T[7] = -y;
    T[8] = R22; T[9] = -R20; T[10] = -R21; T[11] = z;
    T[12] = 0;   T[13] = 0;   T[14] = 0;   T[15] = 1;
    std::cout << "R: " << R00 << " " << R01 << " " << R02 << std::endl;
    std::cout << "R: " << R10 << " " << R11 << " " << R12 << std::endl;
    std::cout << "R: " << R20 << " " << R21 << " " << R22 << std::endl;
    // tager hensyn til TCP
   
    if (angle < 1e-10) {

        // Ingen rotation - brug identitetsmatrix
        T[3] += T[0] * TCP[0] + T[1] * TCP[1] + T[2] * TCP[2];
        T[7] += T[4] * TCP[0] + T[5] * TCP[1] + T[6] * TCP[2];
        T[11] += T[8] * TCP[0] + T[9] * TCP[1] + T[10] * TCP[2];
    }
    else
    {
        T[3] -= T[1] * TCP[0] + T[2] * TCP[1] + T[0] * TCP[2];
        T[7] -= T[5] * TCP[0] + T[6] * TCP[1] + T[4] * TCP[2];
        T[11] -= T[9] * TCP[0] + T[10] * TCP[1] + T[8] * TCP[2];
    }
  

    Eigen::Matrix4d T1 = Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>(T);

    Eigen::Matrix4d T2 = repsetory.getT();

    Eigen::Matrix4d T_W_TCP = T2 * T1;
    std::vector<double> vec(16);
    Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>(vec.data()) = T_W_TCP;

    std::copy(vec.begin(), vec.end(), T);


}


