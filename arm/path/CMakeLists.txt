cmake_minimum_required(VERSION 3.16)
project(RobotArm)

set(CMAKE_CXX_STANDARD 17)

# Find required libraries
find_package(Eigen3 REQUIRED)
find_package(ur_rtde REQUIRED)

# Source files
add_executable(RobotArm
    main.cpp
    RobotArm.cpp
    PathFinder.cpp
    IKcal.cpp
    repsetory.cpp
    ObjDet.cpp
    ur_kin.cpp
)

# Include directories
target_include_directories(RobotArm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EIGEN3_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(RobotArm
    ur_rtde::rtde
    Eigen3::Eigen
)
