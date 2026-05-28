
#pragma once
#include "Grid.h"
#include "Block.h"

#include <vector>

class compiler
{
private:


public:
    compiler();
    ~compiler();

    // make so the robot plaexe the blocks safe
    std::vector<Block> compileplace(std::vector<Block> Blocks);

    // makes so the robot take the blocks in a sfae way
    std::vector<Block> compiletake(std::vector<Block> Blocks);

};

