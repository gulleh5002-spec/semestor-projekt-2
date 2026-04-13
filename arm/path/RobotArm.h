#pragma once
#pragma warning(disable: 4996 4267)
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>s
#include "PathFinder.h"
#include <vector>
#include <cmath>
#include "repsetory.h"




class RobotArm
{
public:
	RobotArm();
	RobotArm(std::string init_ip, double acceleration, double speed);
	~RobotArm();

	//keep
	void home();

	//keep
	void movetool(std::vector<double> koordinat, double speed = 0.5, double acceleration = 0.5);

	void getTcpInfo(std::vector<double> point);

	void getRTDEinfor();

	void moveblock(std::vector<double> koordinat1, std::vector<double> koordinat2);
	




private:
	std::string ip;
	double acceleration;
	double speed;
	ur_rtde::RTDEReceiveInterface rtde_r;
	ur_rtde::RTDEControlInterface rtde_c;
	PathFinder pf;
	repsetory repsetory;
};

// regn på albuernes postioner og sørgde for der ikke kommer koliktions med sigselv og klodeser samt finde den korteste bevælgese
//evt brug moveC(via_punkt, slut_punkt, hastighed, acceleration, blend) til at kommer over basen