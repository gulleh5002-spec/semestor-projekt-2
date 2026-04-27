#include "Grid.h"

Grid::Grid(int length, int width, int height, std::vector<double> grid_to_base) : length(length), width(width), height(height), grid_to_base(grid_to_base)
{}

Grid::~Grid() {}

void Grid::makeGrid()
{
    int laneswidth;
    int laneslength;
    int lanesheight;

    laneslength = length / 5;
    laneswidth = width / 5;
    lanesheight = height / 5;
    for (int s = 0; s < lanesheight; s++)
    {
       std::vector<std::vector<Block>> Vector3d;

        for (int i = 0; i < laneslength; i++)
        {

            std::vector<Block> Vector2d;
            Vector3d.push_back(Vector2d);
            
            for (int j = 0; j < laneswidth; j++)
            {
                
                Vector2d.push_back(Block(0, {2.5+i*5, 2.5+j*5, 2.5+s*5, 3.14, 0.0, 0.0}, 0, {i, j}));
            }
            
        }
        grid.push_back(Vector3d);
    }
}

void Grid::printGrid()
{

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            for (int k = 0; k < grid[i][j].size(); k++)
            {
                std::cout << "id: " << grid[i][j][k].Id << " coordnate: " << grid[i][j][k].getCoordnate()[0] << ", " << grid[i][j][k].getCoordnate()[1] << ", " << grid[i][j][k].getCoordnate()[2] << std::endl;
            }
        }
    }
}
void Grid::placeBlock(std::vector<std::vector<double>> coordinates, int id)
{
     for (int i = 0; i < coordinates.size(); i++)
    {
        grid[coordinates[i][0]][coordinates[i][1]][coordinates[i][2]].Id = id;
    }
}

std::vector<double> Grid::findBlock(Block block)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            for (int k = 0; k < grid[i][j].size(); k++)
            {
                if (grid[i][j][k].Id == block.Id)
                {
                    grid[i][j][k].Id = 0;
                    return grid[i][j][k].getCoordnate();
                }
            }
        }
    }
    return {};
    
}
    