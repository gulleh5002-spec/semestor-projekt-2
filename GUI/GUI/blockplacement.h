#ifndef BLOCKPLACEMENT_H
#define BLOCKPLACEMENT_H

#include "blocktype.h"
#include "gridposition.h"


// Block position data
struct BlockPlacement
{
    GridPosition position;
    BlockType blockType = BlockType::Unknown;
};

#endif // BLOCKPLACEMENT_H
