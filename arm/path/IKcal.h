#pragma once
#include <iostream>
#include <string>
#include <ur_kin.h>
#include <vector>
#include <cmath>
#include "repsetory.h"
#include <Eigen/Dense>
class IKcal
{
public:
	
	IKcal();

	std::vector<std::vector<double>> getInvseKinamtiksList(std::vector<double> points, bool debug);

	void makeTransformMatrix(std::vector<double> moveVector, double T[16]);

	void Matrix4dToArry(Eigen::Matrix4d T, double ARR[16]);

	std::vector<double> Matrix4Todvec(Eigen::Matrix4d);

	Eigen::Matrix4d AngelPoseToTransform(std::vector<double> pose);

	Eigen::Matrix4d FindTCP_WORLD(Eigen::Matrix4d(T_TCP_BASE));

	Eigen::Matrix4d MatrixIKcon(Eigen::Matrix4d tans);

	std::vector<double> TransformToPose(const Eigen::Matrix4d& T);
	 
	Eigen::Matrix4d poseToTransform(std::vector<double> pose);

private:
	repsetory repsetory;
};

