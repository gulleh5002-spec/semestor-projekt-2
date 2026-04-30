#include "repsetory.h"


repsetory::repsetory()
{
	T << -0.6947, 0.7193, 0, 0,
		-0.7193, -0.6947, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	//T << 1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, 1, 0,
	//	0, 0, 0, 1;	
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