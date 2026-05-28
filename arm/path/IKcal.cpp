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
            // debug mode to print all the solotions and if there is a singularty
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
                    // debug if a singularty is found
                    std::cout << "emty" << std::endl;
                }
            }
        }
    }
    return JointList;
}



void IKcal::makeTransformMatrix(std::vector<double> moveVector, double T[16])
{
    Eigen::Matrix4d T_BASE_TCP = poseToTransform(moveVector);
    T_BASE_TCP = MatrixIKcon(T_BASE_TCP);
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


Eigen::Matrix4d IKcal::MatrixIKcon(Eigen::Matrix4d T)
{
    Eigen::Matrix4d T_ROS;
    T_ROS << 
    -T(0, 2), T(0, 0), T(0, 1), -T(0, 3),
    -T(1, 2), T(1, 0), T(1, 1), -T(1, 3),
    T(2, 2), -T(2, 0), -T(2, 1), T(2, 3),
    0, 0, 0, 1;

    return T_ROS;
}

std::vector<double> IKcal::TransformToPose(const Eigen::Matrix4d& T)
{
    std::vector<double> pose(6);
    pose[0] = T(0,3);
    pose[1] = T(1,3);
    pose[2] = T(2,3);

    Eigen::AngleAxisd aa(T.block<3,3>(0,0));
    Eigen::Vector3d rv = aa.axis() * aa.angle();
    pose[3] = rv.x();
    pose[4] = rv.y();
    pose[5] = rv.z();
    return pose;
}


Eigen::Matrix4d IKcal::poseToTransform(std::vector<double> pose)
{
    double x = pose[0];
    double y = pose[1];
    double z = pose[2];
    double rx = pose[3];
    double ry = pose[4];
    double rz = pose[5];
    
    double angle = std::sqrt(rx*rx + ry*ry + rz*rz);
    
    Eigen::Matrix3d R;
    
    if (angle < 1e-10) {
        // Ingen rotation – brug identitetsmatrix
        R = Eigen::Matrix3d::Identity();
    } else {
        // Enhedsvektor for aksen
        double kx = rx / angle;
        double ky = ry / angle;
        double kz = rz / angle;
        
        double c = std::cos(angle);
        double s = std::sin(angle);
        double v = 1.0 - c;
        
        // Rodrigues' rotationsformel
        R << kx*kx*v + c,    kx*ky*v - kz*s, kx*kz*v + ky*s,
             kx*ky*v + kz*s, ky*ky*v + c,    ky*kz*v - kx*s,
             kx*kz*v - ky*s, ky*kz*v + kx*s, kz*kz*v + c;
    }
    
    // Sæt 4x4 transformationsmatrix sammen
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();
    T.block<3,3>(0,0) = R;
    T(0,3) = x;
    T(1,3) = y;
    T(2,3) = z;
    
    return T;
}
