#pragma once
#include <vector>
#include <iostream>
#include <Eigen/Dense>
class repsetory
{

public:

	repsetory();
	~repsetory();
	std::vector<double> getTCP();
	Eigen::Matrix4d getT();


private:
	std::vector<double> tcp = {0, 0, 0};

	Eigen::Matrix4d T;


};

