#pragma once

#include "BuildPlan.h"
#include "newgrid/Block.h"

#include <vector>

class BuildPlanRobotMapper
{
public:
    static std::vector<Block> toRobotBlocks(const BuildPlan& buildPlan);
};
