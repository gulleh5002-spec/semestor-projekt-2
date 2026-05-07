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

class RobotArm {
public:
  RobotArm();
  RobotArm(std::string init_ip, double acceleration, double speed);
  ~RobotArm();

  // keep
  void home();

  // keep
  void movetool(std::vector<double> koordinatWorld , double speed, double acceleration, std::vector<double> gridFrame);

  void getTcpInfo(std::vector<double> point);

  void getRTDEinfor();

  void moveblock(std::vector<double> koordinat1,
                 std::vector<double> koordinat2,
                 std::vector<double> gridFrame1,
                 std::vector<double> gridFrame2);

  void build(Grid& Gridblocks, Grid& Gridplace, std::vector<Block> buildBlocks, std::vector<Block> takeBlocks);
  void moveToGridPos(Grid grid, Block block);


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
};

// regn på albuernes postioner og sørgde for der ikke kommer koliktions med
// sigselv og klodeser samt finde den korteste bevælgese
// evt brug moveC(via_punkt, slut_punkt, hastighed, acceleration, blend) til at
// kommer over basen