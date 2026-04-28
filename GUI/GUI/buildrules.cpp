#include "buildrules.h"

bool BuildRules::canPlaceBlockAt(const RobotWorkspaceData& data, const GridPosition& position)
{
    if (!data.isValidPosition(position) || data.hasBlockAtPosition(position)) {
        return false;
    }

    if (position.z == firstLayer) {
        return true;
    }

    return data.hasBlockAtPosition({position.x, position.y, position.z - 1});
}

bool BuildRules::canRemoveBlockAt(const RobotWorkspaceData& data, const GridPosition& position)
{
    if (!data.isValidPosition(position) || !data.hasBlockAtPosition(position)) {
        return false;
    }

    const GridPosition positionAbove{position.x, position.y, position.z + 1};
    if (!data.isValidPosition(positionAbove)) {
        return true;
    }

    return !data.hasBlockAtPosition(positionAbove);
}
