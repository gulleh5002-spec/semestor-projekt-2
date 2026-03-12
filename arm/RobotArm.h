#pragma once
#pragma warning(disable: 4996 4267)
#include <iostream>
#include <string>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>
#include <vector>
class RobotArm
{
public:
	RobotArm();
	RobotArm(std::string init_ip, double init_gridWidth, double init_gridLength);
	~RobotArm();


	void home();

	void movetool(std::vector<double> koordinat, double speed = 0.5, double acceleration = 0.5);


private:
	std::string ip;
	double gridWidth;
	double gridLength;
	ur_rtde::RTDEReceiveInterface rtde_r;
	ur_rtde::RTDEControlInterface rtde_c;

};

