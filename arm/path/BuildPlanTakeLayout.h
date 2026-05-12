#pragma once

#include "BuildPlan.h"
#include "newgrid/Block.h"

#include <vector>

class BuildPlanTakeLayout
{
public:
    static std::vector<Block> createTakeBlocks(const BuildPlan& buildPlan);
};
