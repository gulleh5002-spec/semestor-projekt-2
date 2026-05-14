#include "BuildPlanTakeLayout.h"

namespace
{
constexpr int blocksPerTakeColumn = {8};
constexpr int fixedTakeLayer = {0};

int validBlockCount(int blockCount)
{
    if (blockCount < 1) {
        return 1;
    }

    return blockCount;
}

std::vector<int> takePositionForIndex(int blockIndex)
{
        // Fylder Y-aksen foer X, saa take-layoutet er roteret 90 grader.
        const int x = blockIndex / blocksPerTakeColumn;
        const int y = blockIndex % blocksPerTakeColumn;

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

int BuildPlanTakeLayout::takeGridLengthCells(int blockCount)
{
    const int count = validBlockCount(blockCount);

    if (count < blocksPerTakeColumn) {
        return count;
    }

    return blocksPerTakeColumn;
}

int BuildPlanTakeLayout::takeGridWidthCells(int blockCount)
{
    const int count = validBlockCount(blockCount);

    return (count + blocksPerTakeColumn - 1) / blocksPerTakeColumn;
}
