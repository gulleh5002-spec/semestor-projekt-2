#include "BuildPlanConsolePrinter.h"
#include "BuildPlanJsonLoader.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace
{
    void printUsage(const char* executableName)
    {
        std::cout << "Usage: " << executableName << " <build_plan.json>\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string errorMessage;
    const auto buildPlan = BuildPlanJsonLoader::loadFromFile(std::filesystem::path{argv[1]},
                                                             &errorMessage);

    if (!buildPlan.has_value()) {
        std::cerr << "Build plan dry-run failed: " << errorMessage << '\n';
        return 1;
    }

    BuildPlanConsolePrinter::print(buildPlan.value(), std::cout);
    return 0;
}
