
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
    std::vector<Block> compileplace(std::vector<Block> Blocks);
    std::vector<Block> compiletake(std::vector<Block> Blocks);

};

