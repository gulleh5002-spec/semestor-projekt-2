#pragma warning(disable: 4996 4267)

#include "BuildPlanConsolePrinter.h"
#include "BuildPlanJsonLoader.h"
#include "BuildPlanRobotMapper.h"
#include "BuildPlanTakeLayout.h"
#include "RobotArm.h"
#include "RobotBuildConstants.h"
#include "newgrid/Block.h"
#include "newgrid/Grid.h"
#include "ArUcoDetector.h"

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace
{
constexpr int robotGridUnit = {5};
constexpr int takeLayerCount = {1};
constexpr const char* defaultRobotIp = "192.168.1.11";
constexpr const char* defaultBuildPlanFileName = "build_plan.json";

bool hasArgument(int argc, char* argv[], const std::string& argument)
{
    for (int i = 1; i < argc; ++i) {
        if (argv[i] != nullptr && argument == argv[i]) {
            return true;
        }
    }

    return false;
}

std::optional<std::filesystem::path> buildPlanPathFromArguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == nullptr) {
            continue;
        }

        const std::string argument = argv[i];

        if (argument != "--dry-run"
            && argument != "--execute"
            && argument != "--manual-test" 
            && argument != "--help") {
            return std::filesystem::path{argument};
        }
    }

    return std::nullopt;
}

std::filesystem::path defaultBuildPlanPath(const char* executableName)
{
    const std::filesystem::path executablePath = executableName != nullptr
                                                    ? std::filesystem::path{executableName}
                                                    : std::filesystem::path{};

    if (executablePath.has_parent_path()) {
        return executablePath.parent_path() / RobotBuildConstants::defaultBuildPlanFileName;
    }

    return std::filesystem::current_path() / RobotBuildConstants::defaultBuildPlanFileName;
}

void printUsage(const char* executableName)
{
    std::cout << "Usage:\n"
              << "  " << executableName << " [build_plan.json] --dry-run\n"
              << "  " << executableName << " [build_plan.json] --execute\n"
              << "  " << executableName << " --manual-test\n"
              << "\nIf no JSON path is given, build_plan.json is read next to RobotArm.exe.\n";
}

std::optional<BuildPlan> loadBuildPlanFromArguments(int argc, char* argv[])
{
    std::filesystem::path filePath;
    const std::optional<std::filesystem::path> argumentFilePath = buildPlanPathFromArguments(argc, argv);

    if (argumentFilePath.has_value()) {
        filePath = argumentFilePath.value();
    } else {
        filePath = defaultBuildPlanPath(argv[0]);
    }

    std::cout << "Using build plan: " << filePath.string() << '\n';

    std::string errorMessage;
    const auto buildPlan = BuildPlanJsonLoader::loadFromFile(filePath, &errorMessage);

    if (!buildPlan.has_value()) {
        std::cerr << "Build plan load failed: " << errorMessage << '\n';
        return std::nullopt;
    }

    return buildPlan;
}

int gridSizeFromCells(int cellCount)
{
    if (cellCount < 1) {
        return RobotBuildConstants::robotGridUnit;
    }

    return cellCount * RobotBuildConstants::robotGridUnit;
}

Grid createPlaceGrid(const WorkspaceSize& workspace)
{
    return Grid(gridSizeFromCells(workspace.height),
                gridSizeFromCells(workspace.width),
                gridSizeFromCells(workspace.layerCount),
                {0.2, 0.2, 0, 0, 0, 0});
}

Grid createTakeGrid(int blockCount)
{
    return Grid(gridSizeFromCells(BuildPlanTakeLayout::takeGridLengthCells(blockCount)),
                gridSizeFromCells(BuildPlanTakeLayout::takeGridWidthCells(blockCount)),
                gridSizeFromCells(RobotBuildConstants::takeLayerCount),
                {0.6, 0.2, 0, 0, 0, 0});
}

int runBuildPlanDryRun(int argc, char* argv[])
{
    const std::optional<BuildPlan> buildPlan = loadBuildPlanFromArguments(argc, argv);

    if (!buildPlan.has_value()) {
        return 1;
    }

    BuildPlanConsolePrinter::print(buildPlan.value(), std::cout);
    return 0;
}

int runBuildPlanExecute(int argc, char* argv[])
{
    const std::optional<BuildPlan> buildPlan = loadBuildPlanFromArguments(argc, argv);

    if (!buildPlan.has_value()) {
        return 1;
    }

    BuildPlanConsolePrinter::print(buildPlan.value(), std::cout);
    std::cout << "Executing build plan with robot...\n";

    std::vector<Block> targetBlocks = BuildPlanRobotMapper::toRobotBlocks(buildPlan.value());
    std::vector<Block> takeBlocks = BuildPlanTakeLayout::createTakeBlocks(buildPlan.value());

    Grid place = createPlaceGrid(buildPlan->workspace);
    Grid take = createTakeGrid(static_cast<int>(takeBlocks.size()));

    RobotArm robot(defaultRobotIp, 1.0, 1.0);
    robot.build(take, place, targetBlocks, takeBlocks);

    return 0;
}

/*
    For manuel test med hardcoding uden json
    kør .\build\Release\RobotArm.exe --manual-test
    i powershell Terminal
*/
int runManualRobotTest()
{
   
    RobotArm robot(defaultRobotIp, 1.0, 1.0);
    
    std::vector<double> pose = robot.approsemate();
    std::cout << "Tray pose: X=" << pose[0] << " Y=" << pose[1] << " Z=" << pose[2]
              << " Rx=" << pose[3] << " Ry=" << pose[4] << " Rz=" << pose[5] << "\n";

   
    pose[2] = 0.01;
    pose[3] = 0;
    pose[4] = 0;
    Grid place(40, 40, 100, pose);
    Grid placeplace(40, 40, 100, {0.2, 0.2, 0 ,0 ,0 ,0});
   
    
    std::vector<Block> takeBlocks = {
        Block(1, {1, 2, 0}),
        Block(1, {2, 2, 0}),
        Block(1, {3, 2, 0}),
        Block(1, {1, 0, 0}),
        Block(1, {2, 0, 0}),
        Block(1, {3, 0, 0}),
    };

    std::vector<Block> placeBlocks = {
        Block(1, {1, 1, 0}),
        Block(1, {1, 3, 0}),
        Block(1, {2, 3, 0}),
        Block(1, {0, 3, 1}),
        Block(1, {1, 3, 1}),
        Block(1, {2, 3, 1}),
    };

    
    
    //robot.build(tray, place, placeBlocks, takeBlocks);
    //robot.movetool({0, 0, 0, 3.14, 0, 0}, 0.5, 0.5, pose, 1);
    robot.moveToGridPos(place, placeBlocks[0]);
    robot.take();
    robot.moveToGridPos(placeBlocks, )
    robot.getTCPPose();
    return 0;
}

}

int main(int argc, char* argv[])
{
    if (hasArgument(argc, argv, "--help")) {
        printUsage(argv[0]);
        return 0;
    }

    if (hasArgument(argc, argv, "--dry-run")) {
        return runBuildPlanDryRun(argc, argv);
    }

    if (hasArgument(argc, argv, "--execute")) {
        return runBuildPlanExecute(argc, argv);
    }

    if (hasArgument(argc, argv, "--manual-test")) {
        return runManualRobotTest();
    }

    if (argc > 1) {
        printUsage(argv[0]);
        return 1;
    }

    printUsage(argv[0]);
    return 1;
}
