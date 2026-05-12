#ifndef BUILDPLANROBOTMAPPER_H
#define BUILDPLANROBOTMAPPER_H

#include "BuildPlan.h"
#include "newgrid/Block.h"

#include <vector>

// Omsætter klodser fra BuildPlan til eksisterende Block Klasse
class BuildPlanRobotMapper
{
public:
    static std::vector<Block> toRobotBlocks(const BuildPlan& buildPlan);
};

#endif // BUILDPLANROBOTMAPPER_H
