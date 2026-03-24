#pragma once
#pragma warning(disable: 4996 4267)
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>

#include <vector>
#include <cmath>
#include <string>
#define UR5e_PARAMS
#include <ur_kin.h>



class RobotArm
{
public:
	RobotArm();
	RobotArm(std::string init_ip, double init_gridWidth, double init_gridLength);
	~RobotArm();


	void home();

	void movetool(std::vector<double> koordinat, double speed = 0.5, double acceleration = 0.5);

	std::vector<std::vector<double>>validMove(std::vector<double> koordinat, double tcpRadius);

	std::vector<double> tcpPose();
	
	void makeTransformMatrix(std::vector<double> moveVector, double T[16]);

	std::vector<std::vector<double>> getInvseKinamtiksList(std::vector<double> points, bool debug= false);

	std::vector<std::vector<std::vector<double>>>getPreferedInvseKinamtikSoltions(std::vector<std::vector<double>> points, int sol);

	std::vector<std::vector<double>>solutuonSorter(std::vector<std::vector<double>> JointList, std::string sol);
	//
	std::vector<std::vector<double>>executeblejoints(std::vector<double> koordinat);



	//lav en funktion som sørger for at albuen altid er over
	void getTcpInfo(std::vector<double> point);

	void getRTDEinfor();
	




private:
	std::string ip;
	double gridWidth;
	double gridLength;
	ur_rtde::RTDEReceiveInterface rtde_r;
	ur_rtde::RTDEControlInterface rtde_c;



	double basexLow = -74.5;
	double basexHigh = 74.5;
	double baseyLow = -74.5;
	double baseyHigh = 74.5;
	double basezLow = 0.0;
	double basezHigh = 300.0;
	
};

// regn på albuernes postioner og sørgde for der ikke kommer koliktions med sigselv og klodeser samt finde den korteste bevælgese
//evt brug moveC(via_punkt, slut_punkt, hastighed, acceleration, blend) til at kommer over basen