#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>
#include <iostream>

using namespace ur_rtde;

int main()
{
  std::string robot_ip = "192.168.64.3";

  RTDEControlInterface rtde_control(robot_ip);
  RTDEReceiveInterface rtde_receive(robot_ip);

  std::vector<double> pose = rtde_receive.getActualTCPPose();

  std::cout << "Current TCP pose:\n";
  for (double v : pose)
    std::cout << v << " ";

  std::cout << std::endl;

  std::vector<double> target = {
      pose[0],
      pose[1],
      pose[2] + 0.05,
      pose[3],
      pose[4],
      pose[5]};

  rtde_control.moveL(target, 0.2, 0.2);

  rtde_control.stopScript();
}