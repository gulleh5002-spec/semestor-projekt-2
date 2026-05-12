#ifndef BUILDPLANJSONLOADER_H
#define BUILDPLANJSONLOADER_H

#include "BuildPlan.h"

#include <filesystem>
#include <optional>
#include <string>
// læser GUI'ens build_plan.json og laver den om til en BuildPlan

class BuildPlanJsonLoader
{
public:
    static std::optional<BuildPlan> loadFromFile(const std::filesystem::path& filePath,
                                                 std::string* errorMessage = nullptr);
};

#endif // BUILDPLANJSONLOADER_H
