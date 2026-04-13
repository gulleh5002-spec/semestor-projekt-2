#include "repsetory.h"


repsetory::repsetory()
{
	T << 0.9272, -0.3746, 0, 0,
		0.3746, 0.9272, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
}

repsetory::~repsetory()
{

}
std::vector<double> repsetory::getTCP()
{
	return tcp;
}

Eigen::Matrix4d repsetory::getT() {
	return T;
}