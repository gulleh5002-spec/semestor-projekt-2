#include "PicoController.h"
#include <iostream>
#include <chrono>
#include <thread>



int main() {
    PicoController pico;
    
    // Start the server on port 5000
    if (!pico.startServer(5000)) {
        std::cerr << "CRITICAL ERROR: Failed to host SDK server port.\n";
        return 1;
    }

    std::cout << "Robot main control loop initialized. System actively tracking at 50Hz...\n";

    bool gripperHasCommanded = false;

    // --- Main Loop ---
    while (true) {
        // 1. Constantly look for incoming Pico connections without pausing
        pico.updateConnection();

        // 2. Perform parallel mission-critical robot math
       // insert runKinematicsCalculation();

        // 3. Automated state evaluation if Pico is active
        if (pico.isConnected()) {
            
            // Example automated condition:
           /* if (checkRobotArmPosition() && !gripperHasCommanded) {
                std::cout << "Arm in position. 'Deploying 'GRIP' package'...\n";
                pico.sendCommand("GRIP");
                gripperHasCommanded = true; // Mark as sent so we don't spam it
            }*/

            // 4. Read feedback asynchronously. If Pico hasn't typed anything, this returns "" instantly.
            std::string feedback = pico.readResponse();
            if (!feedback.empty()) {
                std::cout << "Feedback packet caught: " << feedback << "\n";
                
                // You can add logic strings processing here
                if (feedback.find("GRIPPED") != std::string::npos) {
                     std::cout << "Object secure. Proceeding with lift sequencing...\n";
                }
            }
        }

        // Maintain constant loop execution rate (50Hz = ~20ms interval per tick)
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return 0;
}
