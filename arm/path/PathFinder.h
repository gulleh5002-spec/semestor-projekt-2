#pragma once
#define _USE_MATH_DEFINES
#define UR5e_PARAMS
#include <string>
#include <vector>
#include <iostream>
#include "IKcal.h"
#include <cmath>

class PathFinder
{
public:

	PathFinder();
	~PathFinder();

	std::vector<std::vector<double>> resolutionpath(std::vector<double> koordinat, std::vector<double> startPose);

	std::vector<std::vector<double>> obstacalcheck(std::vector<std::vector<double>> path);

	std::vector<std::vector<double>> findPath(std::vector<double> startPose, std::vector<double> endPose, std::vector<double> lastJoint);

	std::vector<std::vector<std::vector<double>>>getPreferedInvseKinamtikSoltions(std::vector<std::vector<double>> points);

	std::vector<std::vector<double>>solutuonSorter(std::vector<std::vector<double>> JointList, std::string sol);

	std::vector<std::vector<double>>executeblejoints(std::vector<std::vector<std::vector<double>>> aprovedJoints, std::vector<double> lastJoint);




private:
	IKcal IK;

};

