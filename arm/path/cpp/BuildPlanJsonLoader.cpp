#include "BuildPlanJsonLoader.h"

#include <fstream>
#include <nlohmann/json.hpp>

namespace
{
using json = nlohmann::json;

void setError(std::string* errorMessage, const std::string& message)
{
    if (errorMessage != nullptr) {
        *errorMessage = message;
    }
}

bool readIntValue(const json& object, const char* key, int& value, std::string* errorMessage)
{
    if (!object.contains(key) || !object.at(key).is_number_integer()) {
        setError(errorMessage, std::string{"Missing or invalid integer field: "} + key);
        return false;
    }

    value = object.at(key).get<int>();
    return true;
}

bool readBoolValue(const json& object, const char* key, bool& value, std::string* errorMessage)
{
    if (!object.contains(key) || !object.at(key).is_boolean()) {
        setError(errorMessage, std::string{"Missing or invalid boolean field: "} + key);
        return false;
    }

    value = object.at(key).get<bool>();
    return true;
}

bool readStringValue(const json& object, const char* key, std::string& value, std::string* errorMessage)
{
    if (!object.contains(key) || !object.at(key).is_string()) {
        setError(errorMessage, std::string{"Missing or invalid string field: "} + key);
        return false;
    }

    value = object.at(key).get<std::string>();
    return true;
}

bool readWorkspace(const json& rootObject, WorkspaceSize& workspace, std::string* errorMessage)
{
    if (!rootObject.contains("workspace") || !rootObject.at("workspace").is_object()) {
        setError(errorMessage, "Missing or invalid workspace object");
        return false;
    }

    const json& workspaceObject = rootObject.at("workspace");

    return readIntValue(workspaceObject, "width", workspace.width, errorMessage)
        && readIntValue(workspaceObject, "height", workspace.height, errorMessage)
        && readIntValue(workspaceObject, "layerCount", workspace.layerCount, errorMessage)
        && readBoolValue(workspaceObject, "isCreated", workspace.isCreated, errorMessage);
}

bool readBlock(const json& blockObject,
               int blockId,
               BuildPlanBlock& block,
               std::string* errorMessage)
{
    block.id = blockId;

    return readIntValue(blockObject, "x", block.x, errorMessage)
        && readIntValue(blockObject, "y", block.y, errorMessage)
        && readIntValue(blockObject, "z", block.z, errorMessage)
        && readStringValue(blockObject, "type", block.type, errorMessage);
}

bool readBlocks(const json& rootObject,
                std::vector<BuildPlanBlock>& blocks,
                std::string* errorMessage)
{
    if (!rootObject.contains("blocks") || !rootObject.at("blocks").is_array()) {
        setError(errorMessage, "Missing or invalid blocks array");
        return false;
    }

    const json& blockObjects = rootObject.at("blocks");
    int nextBlockId = 1;

    for (const json& blockObject : blockObjects) {
        if (!blockObject.is_object()) {
            setError(errorMessage, "Invalid block entry");
            return false;
        }

        BuildPlanBlock block;

        if (!readBlock(blockObject, nextBlockId, block, errorMessage)) {
            return false;
        }

        blocks.push_back(block);
        ++nextBlockId;
    }

    return true;
}
}

std::optional<BuildPlan> BuildPlanJsonLoader::loadFromFile(const std::filesystem::path& filePath,
                                                           std::string* errorMessage)
{
    std::ifstream file{filePath};

    if (!file.is_open()) {
        setError(errorMessage, "Could not open build plan file: " + filePath.string());
        return std::nullopt;
    }

    json rootObject;

    try {
        file >> rootObject;
    } catch (const json::exception& exception) {
        setError(errorMessage, "Could not parse build plan JSON: " + std::string{exception.what()});
        return std::nullopt;
    }

    BuildPlan buildPlan;

    if (!readWorkspace(rootObject, buildPlan.workspace, errorMessage)) {
        return std::nullopt;
    }

    if (!readBlocks(rootObject, buildPlan.blocks, errorMessage)) {
        return std::nullopt;
    }

    return buildPlan;
}
