#ifndef BUILDRULES_H
#define BUILDRULES_H

#include "gridposition.h"
#include "robotworkspacedata.h"

class BuildRules
{
public:
    // Håndtere om man kan fjerne eller placere en klods
    static bool canPlaceBlockAt(const RobotWorkspaceData& data, const GridPosition& position);
    static bool canRemoveBlockAt(const RobotWorkspaceData& data, const GridPosition& position);
};

#endif // BUILDRULES_H
