#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "blocktype.h"

// Celle data i grid
struct GridCell
{
    bool hasBlock = false;
    BlockType blockType = BlockType::Unknown;
};

#endif // GRIDCELL_H
