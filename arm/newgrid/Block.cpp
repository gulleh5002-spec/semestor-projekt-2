#include "Block.h"



Block::Block(int id, std::vector<int> place): Id(id), place(place)
{
    coordnate = {2.5+place[0]*5, 2.5+place[1]*5, 2.5+place[2]*5, 0,0,3.944};
}

Block::~Block()
{
}

int Block::getId()
{
    return Id;
}

std::vector<double> Block::getCoordnate()
{
    return coordnate;
}


std::vector<int> Block::getplace()
{
    return place;
}