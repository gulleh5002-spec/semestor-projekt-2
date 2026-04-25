#include "Grid.h"

Grid::Grid(int height, int width) : height(height), width(width) {}

Grid::~Grid() {}

void Grid::makeGrid()
{
    int laneswidth;
    int lanesheight;

    lanesheight = height / 5;
    laneswidth = width / 5;

    for (int i = 0; i < lanesheight; i++)
    {
        std::vector<Block> newVector;
        for (int j = 0; j < laneswidth; j++)
        {
            
            newVector.push_back(Block(0, {i*2.5, j*2.5}));
        }
        grid.push_back(newVector);
    }
}

void Grid::printGrid()
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            std::cout << "Block ID: " << grid[i][j].getId() << " Coordinate: (" << grid[i][j].getCoordnate()[0] << ", " << grid[i][j].getCoordnate()[1] << ") ";
        }
        std::cout << std::endl;
    }
}

