#pragma once
#pragma warning(disable : 4996 4267)
#define _USE_MATH_DEFINES
#include "PathFinder.h"
#include "repsetory.h"
#include "IKcal.h"
#include <cmath>
#include <iostream>
#include <string>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>
#include <vector>
#include "newgrid/Grid.h"
#include "GripperClient.h"
#include "newgrid/compiler.h"
#include "ArUcoDetector.h"
class RobotArm {
public:
  RobotArm();
  RobotArm(std::string init_ip, double acceleration, double speed);
  ~RobotArm();

  // keep
  void home();


  void movetool(std::vector<double> koordinatWorld , double speed, double acceleration, std::vector<double> gridFrame, int flange = 0);

  void getTcpInfo(std::vector<double> point);

  std::vector<double> getTCPPose();

  void moveblock(std::vector<double> koordinat1, std::vector<double> koordinat2, Grid& gridFrame1, Grid& gridFrame2, int method);

  void build(Grid& Gridblocks, Grid& Gridplace, std::vector<Block> buildBlocks, std::vector<Block> takeBlocks);
 
  std::vector<double> approsemate();

  std::vector<double> getTrayFrame();

  void moveToGridPos(Grid grid, Block block, bool m);
  void take();
  void drop();
  


private:
  std::string ip;
  double acceleration;
  double speed;
  ur_rtde::RTDEReceiveInterface rtde_r;
  ur_rtde::RTDEControlInterface rtde_c;
  PathFinder pf;
  repsetory repsetory;
  IKcal IKcal;
  GripperClient gripper;
  compiler compile;
  ArUcoDetector camera;
    
};
