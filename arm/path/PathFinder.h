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

	// make a path from a start pose to an end pose with a given joint configuration as start point for the ik solver
	std::vector<std::vector<double>> resolutionpath(std::vector<double> koordinat, std::vector<double> startPose);


	// make a path from a start pose to an end pose with a given joint configuration as start point for the ik solver and return the path as a list of joint angles
	// uses the funktionen resolutionpath to make a path of points and then uses getPreferedInvseKinamtikSoltions to get the ik soltions for each point and then uses executeblejoints to find the shorts path through the ik soltions
	std::vector<std::vector<double>> findPath(std::vector<double> startPose, std::vector<double> endPose, std::vector<double> lastJoint);


	// take a list of points and return a list of lists of ik soltions for each point and if there is no soltions it push an emty list
	std::vector<std::vector<std::vector<double>>>getPreferedInvseKinamtikSoltions(std::vector<std::vector<double>> points);


	// take a list of ik soltions a and sorts unwanted soltions away 
	std::vector<std::vector<double>>solutuonSorter(std::vector<std::vector<double>> JointList, std::string sol);

	// find the shorts path from a list of ik soltions for each point
	std::vector<std::vector<double>>executeblejoints(std::vector<std::vector<std::vector<double>>> aprovedJoints, std::vector<double> lastJoint);




private:
	IKcal IK;

};

