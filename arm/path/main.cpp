#pragma warning(disable: 4996 4267)
#include "BuildPlanConsolePrinter.h"
#include "BuildPlanJsonLoader.h"
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

        if (argument != "--dry-run" && argument != "--help") {
            return std::filesystem::path{argument};
        }
    }

    return std::nullopt;
}

void printUsage(const char* executableName)
{
    std::cout << "Usage:\n"
              << "  " << executableName << " <build_plan.json> --dry-run\n"
              << "  " << executableName << '\n';
}

int runBuildPlanDryRun(int argc, char* argv[])
{
    const std::optional<std::filesystem::path> filePath = buildPlanPathFromArguments(argc, argv);

    if (!filePath.has_value()) {
        printUsage(argv[0]);
        return 1;
    }

    std::string errorMessage;
    const auto buildPlan = BuildPlanJsonLoader::loadFromFile(filePath.value(), &errorMessage);

    if (!buildPlan.has_value()) {
        std::cerr << "Build plan dry-run failed: " << errorMessage << '\n';
        return 1;
    }

    BuildPlanConsolePrinter::print(buildPlan.value(), std::cout);
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



