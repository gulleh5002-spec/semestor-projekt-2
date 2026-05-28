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


	// calculate all the invse kinamtik solotions for a point and returns the soltions
	std::vector<std::vector<double>> getInvseKinamtiksList(std::vector<double> points, bool debug);

	//make a transformation matrix via the funktions poseToTransform and MatrixIKcon and make it to a array of 16 doubles so ur kinematics kan use it
	void makeTransformMatrix(std::vector<double> moveVector, double T[16]);


	// helper funktion to convert a 4x4 matrix to an array of 16 doubles
	void Matrix4dToArry(Eigen::Matrix4d T, double ARR[16]);

	// konverterer så ur kinematics kan bruge det, da den bruger en anden konvention end den vi bruger i resten af koden
	Eigen::Matrix4d MatrixIKcon(Eigen::Matrix4d T);

	// make a transformation matrix to a pose.
	std::vector<double> TransformToPose(const Eigen::Matrix4d& T);
	 

	// make a pose to a transformation matrix 
	Eigen::Matrix4d poseToTransform(std::vector<double> pose);

private:
};

