#include "BuildPlanTakeLayout.h"

namespace
{
inline constexpr int blocksPerTakeRow = 8;
inline constexpr int fixedTakeLayer = 0;

std::vector<int> takePositionForIndex(int blockIndex)
{
    const std::vector<std::vector<int>> hardcodedTakePositions = {
        {0, 0, fixedTakeLayer},
        {1, 0, fixedTakeLayer},
        {2, 0, fixedTakeLayer},
        {3, 0, fixedTakeLayer},
        {4, 0, fixedTakeLayer},
        {5, 0, fixedTakeLayer},
        {6, 0, fixedTakeLayer},
        {7, 0, fixedTakeLayer},
        {0, 1, fixedTakeLayer},
        {1, 1, fixedTakeLayer},
        {2, 1, fixedTakeLayer},
        {3, 1, fixedTakeLayer},
        {4, 1, fixedTakeLayer},
        {5, 1, fixedTakeLayer},
        {6, 1, fixedTakeLayer},
        {7, 1, fixedTakeLayer},
    };

    if (blockIndex < static_cast<int>(hardcodedTakePositions.size())) {
        return hardcodedTakePositions[blockIndex];
    }

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
