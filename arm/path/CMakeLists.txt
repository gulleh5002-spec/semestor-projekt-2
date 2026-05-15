cmake_minimum_required(VERSION 3.16)
project(RobotArm)
set(CMAKE_CXX_STANDARD 17)

# Brug Boost fra vcpkg (fuldt installeret, matcher 1.90)
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows" ${CMAKE_PREFIX_PATH})

# Eigen (header-only)
set(EIGEN3_INCLUDE_DIR "C:/Users/GusHansen/Desktop/c++/Robotarm/Robotarm")

find_package(nlohmann_json CONFIG REQUIRED)

set(OpenCV_DIR "C:/Users/GusHansen/Downloads/opencv-4.12.0-windows/opencv/build")
find_package(OpenCV REQUIRED)

add_library(BuildPlanSupport STATIC
    h/BuildPlan.h
    cpp/BuildPlanConsolePrinter.cpp
    h/BuildPlanConsolePrinter.h
    cpp/BuildPlanJsonLoader.cpp
    h/BuildPlanJsonLoader.h
    cpp/BuildPlanRobotMapper.cpp
    h/BuildPlanRobotMapper.h
    cpp/BuildPlanTakeLayout.cpp
    h/BuildPlanTakeLayout.h
    h/RobotBuildConstants.h
)

target_include_directories(BuildPlanSupport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/h
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(BuildPlanSupport PUBLIC
    nlohmann_json::nlohmann_json
)

add_executable(BuildPlanDryRun
    cpp/BuildPlanDryRun.cpp
)

target_link_libraries(BuildPlanDryRun PRIVATE
    BuildPlanSupport
)

# ur_rtde
find_package(ur_rtde QUIET PATHS "C:/Users/GusHansen/ur_rtde/build/ur_rtde")

if(ur_rtde_FOUND)
    add_executable(RobotArm
        main.cpp
        RobotArm.cpp
        GripperClient.cpp
        PathFinder.cpp
        IKcal.cpp
        repsetory.cpp
        ur_kin.cpp
        ../newgrid/Grid.cpp
        ../newgrid/Block.cpp
        ../newgrid/compiler.cpp
        ../../camera/cv.cpp
    )

    target_include_directories(RobotArm PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${EIGEN3_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../camera
    )

    target_link_libraries(RobotArm PRIVATE
        BuildPlanSupport
        ur_rtde::rtde
        ${OpenCV_LIBS}
    )

    if(WIN32)
        target_link_libraries(RobotArm PRIVATE
            ws2_32
        )
    endif()

    add_custom_command(TARGET RobotArm POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/Users/GusHansen/ur_rtde/bin/Release"
            $<TARGET_FILE_DIR:RobotArm>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/vcpkg/installed/x64-windows/bin"
            $<TARGET_FILE_DIR:RobotArm>
    )
else()
    message(WARNING "ur_rtde was not found. RobotArm target will not be built, but BuildPlanDryRun is available.")
endif()
