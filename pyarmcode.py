import rtde_control
import rtde_receive
import math

class RobotMovements:
    def __init__(self, ip):
        self.ip = ip
        self.rtde_c = rtde_control.RTDEControlInterface(ip)
        self.rtde_r = rtde_receive.RTDEReceiveInterface(ip)
        print("forbundet til: " + ip)

    def get_position(self):
        pose = self.rtde_r.getActualTCPPose()
        return pose
    
    def move_to(self, x, y ,z):
        punkt = [x, y, z, 0, 3.14, 0]
        self.rtde_c.moveL(punkt, 0.5, 0.5)
        print(f"Flyttet til: X={x}, Y={y}, Z={z}")
        
    def home(self):
        self.rtde_c.moveJ([0, -1.57, 1.57, 0, 0, 0], 0.5, 0.5)
        print("home")

    def rotate_tool():
        pass




robot = RobotMovements("192.168.64.3")

robot.home()


