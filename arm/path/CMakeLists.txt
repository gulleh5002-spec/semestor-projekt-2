cmake_minimum_required(VERSION 3.16)
project(RobotArm)
set(CMAKE_CXX_STANDARD 17)

# Brug Boost fra vcpkg (fuldt installeret, matcher 1.90)
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" ${CMAKE_PREFIX_PATH})

# Eigen (header-only)
set(EIGEN3_INCLUDE_DIR "C:/Users/GusHansen/Desktop/c++/Robotarm/Robotarm")

# ur_rtde
find_package(ur_rtde REQUIRED PATHS "C:/Users/GusHansen/ur_rtde/build/ur_rtde")
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(RobotArm
    main.cpp
    BuildPlan.h
    BuildPlanJsonLoader.cpp
    BuildPlanJsonLoader.h
    RobotArm.cpp
    GripperClient.cpp
    PathFinder.cpp
    IKcal.cpp
    repsetory.cpp
    ur_kin.cpp
    ../newgrid/Grid.cpp
    ../newgrid/Block.cpp
)


target_include_directories(RobotArm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${EIGEN3_INCLUDE_DIR}
)

target_link_libraries(RobotArm PRIVATE
    ur_rtde::rtde
    nlohmann_json::nlohmann_json
    ws2_32
)

add_custom_command(TARGET RobotArm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/Users/GusHansen/ur_rtde/bin/Release"
        $<TARGET_FILE_DIR:RobotArm>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/vcpkg/installed/x64-windows/bin"
        $<TARGET_FILE_DIR:RobotArm>
)
