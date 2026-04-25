#pragma once
#include <string>
#include <vector>
#include <iostream>


class ObjDet
{
public:
ObjDet(double xLow, double xHigh, double yLow, double yHigh, double zLow, double zHigh);

~ObjDet();

std::vector<std::vector<double>>objektAvoid(std::vector<std::vector<double>> path, double tcpRadius);


private:

	double xLow;
	double xHigh;
	double yLow;
	double yHigh;
	double zLow;
	double zHigh;
};

