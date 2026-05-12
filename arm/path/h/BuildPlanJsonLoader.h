#ifndef BUILDPLANJSONLOADER_H
#define BUILDPLANJSONLOADER_H

#include "BuildPlan.h"

#include <filesystem>
#include <optional>
#include <string>

class BuildPlanJsonLoader
{
public:
    static std::optional<BuildPlan> loadFromFile(const std::filesystem::path& filePath,
                                                 std::string* errorMessage = nullptr);
};

#endif // BUILDPLANJSONLOADER_H
