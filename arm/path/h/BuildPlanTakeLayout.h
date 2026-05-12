#ifndef BUILDPLANTAKELAYOUT_H
#define BUILDPLANTAKELAYOUT_H

#include "BuildPlan.h"
#include "newgrid/Block.h"

#include <vector>

// Laver klodser robotten skal hente fra take-området
//TODO: Skal bruges til TRAY
class BuildPlanTakeLayout
{
public:
    static std::vector<Block> createTakeBlocks(const BuildPlan& buildPlan);
};

#endif // BUILDPLANTAKELAYOUT_H
