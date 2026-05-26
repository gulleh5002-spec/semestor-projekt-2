#include "PicoController.h"
#include <iostream>
#include <chrono>
#include <thread>


// A main to show how to use the code:

int main() {
    // 1. Create your robot workspace object
    Robot robot;

    // 2. Connect using the static IP address to the Pico server
    if (!robot.connectToPico("10.174.5.49", 5000)) {
        std::cerr << "[Error] Could not reach the Pico server. Check your Wi-Fi connections.\n";
        return 1;
    }

    std::cout << "Control loop initializing...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 3. Execute your professional, abstract robot routines
    std::cout << "\n--- Beginning Automated Sequence ---\n";
    
    robot.close(); // Sends "GRIP\n" automatically in the background
    
    std::this_thread::sleep_for(std::chrono::seconds(3));

    robot.setAngle(80);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    robot.open();  // Sends "OPEN\n" automatically in the background

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Automated sequence completed successfully.\n";
    
    return 0;
}
