#include "Block.h"



Block::Block(int id, std::vector<int> place): Id(id), place(place)
{
    coordnate = {0.045+place[0]*0.05, 0.05 + place[1]*0.05, 0.025+place[2]*0.05, 3.14,0,0,0};
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