cmake_minimum_required(VERSION 3.15)
project(RobotControllerSDK CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(RobotEngine 
    main.cpp 
    PicoController.cpp
)


target_include_directories(RobotEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 3. Tell CMake to link the Windows Sockets engine (ws2_32) ( IMPORT LINE ) !!!!
if(WIN32)
    target_link_libraries(RobotEngine PRIVATE ws2_32)
endif()
