#include "Block.h"



Block::Block(int id,  std::vector<double> coordnate)    : id(id), coordnate(coordnate)
{
}

Block::~Block()
{
}

int Block::getId()
{
    return id;
}

std::vector<double> Block::getCoordnate()
{
    return coordnate;
}