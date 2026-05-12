#include "BuildPlanTakeLayout.h"

namespace
{
inline constexpr int blocksPerTakeRow = 8;
inline constexpr int fixedTakeLayer = 1;

std::vector<int> takePositionForIndex(int blockIndex)
{
    const int x = blockIndex % blocksPerTakeRow;
    const int y = blockIndex / blocksPerTakeRow;

    return {x, y, fixedTakeLayer};
}
}

std::vector<Block> BuildPlanTakeLayout::createTakeBlocks(const BuildPlan& buildPlan)
{
    std::vector<Block> takeBlocks;
    takeBlocks.reserve(buildPlan.blocks.size());

    for (int index = 0; index < static_cast<int>(buildPlan.blocks.size()); ++index) {
        const BuildPlanBlock& targetBlock = buildPlan.blocks[index];
        takeBlocks.emplace_back(targetBlock.id, takePositionForIndex(index));
    }

    return takeBlocks;
}
