#pragma warning(disable: 4996 4267)

#include "BuildPlanConsolePrinter.h"
#include "BuildPlanJsonLoader.h"
#include "BuildPlanRobotMapper.h"
#include "BuildPlanTakeLayout.h"
#include "RobotArm.h"
#include "newgrid/Block.h"
#include "newgrid/Grid.h"

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace
{
inline constexpr int robotGridUnit = 5;
inline constexpr int takeBlocksPerRow = 8;
inline constexpr int takeLayerCount = 1;
inline constexpr const char* defaultRobotIp = "192.168.1.100";
inline constexpr const char* defaultBuildPlanFileName = "build_plan.json";

bool hasArgument(int argc, char* argv[], const std::string& argument)
{
    for (int i = 1; i < argc; ++i) {
        if (argv[i] != nullptr && argument == argv[i]) {
            return true;
        }
    }

    // return false;
    // RobotArm gulle("192.168.1.100", 1, 1.0);
    // //RobotArm magnum("127.0.0.1", 1.0, 1.0);
    // std::vector<double> point1 = { 0.4, 0.4, 0.2, 3.14, 0.0, 0.0 };
    // std::vector<double> point2 = { 0.2, 0.2, 0.2, 3.14, 0.0, 0.0 };
    // //gulle.moveblock(point1, point2);
    // //gulle.moveblock(point1, point2);
    
    // //gulle.movetool({ 0.4, 0.4, 0.15, 3.14, 0.0, 0.0 }, 0.5, 0.5, {-0.2 ,-0.2 ,0, 0, 0, 2.3732 });
    // //gulle>.movetool({ 0.4, 0, 0.5, 3.14, 0.0, 0.0 });
    // //gulle.home();
    // std::vector<Block> takeblocks = 
    // {
    // Block(1, {1, 2, 0}),
    // Block(1, {2, 2, 0}),
    // Block(1, {3, 3, 0}),
    // //Block(1, {2, 2, 0}),
    // //Block(1, {2, 3, 0}),
    // //Block(1, {0, 2, 2})
    // };

    // std::vector<Block> placeblocks = 
    // {
    // Block(1, {0, 3, 0}),
    // Block(1, {1, 3, 0}),
    // Block(1, {2, 3, 0}),
    // //Block(1, {0, 2, 2})
    // };
   


    // Grid place(40, 40, 100, {0.2 ,0.2 ,0, 0, 0, 0});
    // Grid take(40, 40, 100, {0.6 ,0.2, 0, 0, 0, 0});
    
    // //structur.placeBlock(placeblocks);
    // //gulle.drop();
    // gulle.build(take, place, placeblocks, takeblocks);

     
    // //gulle.drop();
    // //gulle.moveToGridPos(place, takeblocks[1]);

    // // take skal ganges på place evt lav en frame med kun rotaton og igne translation der efter gange identits matrice med translation eller trai på den
    // // evt lav en test for at tjekke siguleretet

   
    // //gulle.moveToGridPos(take, placeblocks[0]);
    // //gulle.take();

    // return 0;

    
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
        return executablePath.parent_path() / defaultBuildPlanFileName;
    }

    return std::filesystem::current_path() / defaultBuildPlanFileName;
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
        return robotGridUnit;
    }

    return cellCount * robotGridUnit;
}

int takeRowCount(int blockCount)
{
    if (blockCount < 1) {
        return 1;
    }

    return (blockCount + takeBlocksPerRow - 1) / takeBlocksPerRow;
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
    return Grid(gridSizeFromCells(takeRowCount(blockCount)),
                gridSizeFromCells(takeBlocksPerRow),
                gridSizeFromCells(takeLayerCount),
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

int runManualRobotTest()
{
    RobotArm robot(defaultRobotIp, 1.0, 1.0);

    std::vector<Block> takeBlocks = {
        Block(1, {1, 2, 0}),
        Block(1, {2, 2, 0}),
        Block(1, {3, 3, 0}),
    };

    std::vector<Block> placeBlocks = {
        Block(1, {0, 3, 0}),
    };

    Grid place(40, 40, 100, {0.2, 0.2, 0, 0, 0, 0});
    Grid take(40, 40, 100, {0.6, 0.2, 0, 0, 0, 0});

    robot.build(take, place, placeBlocks, takeBlocks);
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
