#include "Block.h"



Block::Block(int id,  std::vector<double> coordnate, int status, std::vector<int> place): Id(id), coordnate(coordnate), status(status), place(place)
{
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
    return ;
}

int Block::getstatus()
{
    return status;
}
std::vector<int> Block::getplace()
{
    return place;
}