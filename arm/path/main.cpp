#pragma warning(disable: 4996 4267)
#include "BuildPlanConsolePrinter.h"
#include "BuildPlanJsonLoader.h"
#include "BuildPlanRobotMapper.h"
#include "BuildPlanTakeLayout.h"
#include "RobotArm.h"
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>  
#include "newgrid/Grid.h"
#include "newgrid/Block.h"
#include "GripperClient.h"

namespace
{
inline constexpr int gridCellSize = 5;
inline constexpr int takeBlocksPerRow = 8;
inline constexpr int takeLayerCount = 2;

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

        if (argument != "--dry-run" && argument != "--execute" && argument != "--help") {
            return std::filesystem::path{argument};
        }
    }

    return std::nullopt;
}

void printUsage(const char* executableName)
{
    std::cout << "Usage:\n"
              << "  " << executableName << " <build_plan.json> --dry-run\n"
              << "  " << executableName << " <build_plan.json> --execute\n"
              << "  " << executableName << '\n';
}

std::optional<BuildPlan> loadBuildPlanFromArguments(int argc, char* argv[])
{
    const std::optional<std::filesystem::path> filePath = buildPlanPathFromArguments(argc, argv);

    if (!filePath.has_value()) {
        printUsage(argv[0]);
        return std::nullopt;
    }

    std::string errorMessage;
    const auto buildPlan = BuildPlanJsonLoader::loadFromFile(filePath.value(), &errorMessage);

    if (!buildPlan.has_value()) {
        std::cerr << "Build plan load failed: " << errorMessage << '\n';
        return std::nullopt;
    }

    return buildPlan;
}

int gridSizeFromCells(int cellCount)
{
    if (cellCount < 1) {
        return gridCellSize;
    }

    return cellCount * gridCellSize;
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

    take.placeBlock(takeBlocks);

    RobotArm magnum("127.0.0.1", 1.0, 1.0);
    magnum.build(take, place, targetBlocks);

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

    if (argc > 1) {
        printUsage(argv[0]);
        return 1;
    }

    //RobotArm gulle("192.168.1.100", 1, 1.0);
    RobotArm magnum("127.0.0.1", 1.0, 1.0);
    std::vector<double> point1 = { 0.4, 0.4, 0.1, 3.14, 0.0, 0.0 };
    std::vector<double> point2 = { 0.2, 0.2, 0.3, 3.14, 0.0, 0.0 };
    //gulle.moveblock(point1, point2);
    //gulle.moveblock(point1, point2);
    //magnum.movetool(point1);
    //gulle.movetool({ 0.4, 0.4, 0.15, 3.14, 0.0, 0.0 }, 0.5, 0.5, {-0.2 ,-0.2 ,0, 0, 0, 2.3732 });
    //gulle>.movetool({ 0.4, 0, 0.5, 3.14, 0.0, 0.0 });
    //gulle.home();
    std::vector<Block> blocks = 
    {
    Block(1, {0, 0, 1}),
    //Block(1, {1, 2, 1}),
    //Block(1, {4, 2, 1}),
    //Block(1, {0, 2, 1}),
    //Block(1, {0, 2, 2})
    };
    Grid place(40, 40, 100, {0.2 ,0.2 ,0, 0, 0, 0});
    Grid take(40, 40, 100, {0.6 ,0.2, 0, 0, 0, 0});
    
    take.placeBlock(blocks);
    
    magnum.build(take, place, blocks);
    //magnum.moveToGridPos(place, blocks[1]);

    // take skal ganges på place evt lav en frame med kun rotaton og igne translation der efter gange identits matrice med translation eller trai på den
    // evt lav en test for at tjekke siguleretet
    //gulle.moveToGridPos(take, blocks[0]);
    //gulle.moveToGridPos(place, blocks[0]);
    return 0;
}



//to doo
// fix matrix multiplaction så de passer evt med nogle funktioner
// går grid location letter 
// tcp offset og forbindelse
// lav en en funktion som kan placere blokke i midten
// mere smooth bevæglse



