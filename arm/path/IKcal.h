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



private:
	repsetory repsetory;
};

