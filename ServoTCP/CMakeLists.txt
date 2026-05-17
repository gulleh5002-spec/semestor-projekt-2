cmake_minimum_required(VERSION 3.15)
project(RobotControllerSDK CXX)

# Force the project to use modern C++17 properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Register all your source files here
add_executable(RobotEngine 
    main.cpp 
    PicoController.cpp
)

# 2. Tell CMake to pull headers from your current directory
target_include_directories(RobotEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 3. Tell CMake to link the Windows Sockets engine (ws2_32)
if(WIN32)
    target_link_libraries(RobotEngine PRIVATE ws2_32)
endif()
