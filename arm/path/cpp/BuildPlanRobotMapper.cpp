#include "BuildPlanRobotMapper.h"

namespace
{
    std::vector<int> robotBlockPosition(const BuildPlanBlock& block)
    {
        return {block.x, block.y, block.z};
    }
}

std::vector<Block> BuildPlanRobotMapper::toRobotBlocks(const BuildPlan& buildPlan)
{
    std::vector<Block> robotBlocks;
    robotBlocks.reserve(buildPlan.blocks.size());

    for (const BuildPlanBlock& block : buildPlan.blocks) {
        robotBlocks.emplace_back(block.id, robotBlockPosition(block));
    }

    return robotBlocks;
}
