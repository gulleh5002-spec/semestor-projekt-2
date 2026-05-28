#include "Grid.h"

Grid::Grid(int length, int width, int height, std::vector<double> grid_to_base, int tcp) : length(length), width(width), height(height), grid_to_base(grid_to_base), TCP(tcp)
{
    makeGrid();
}

Grid::~Grid() {}



// creates a 3D grid using 3 nested for loops and pushes blocks into a 3D vector grid.
// each block object is assigned the coordinate that matches its position in the grid frame
void Grid::makeGrid()
{
    int laneswidth;
    int laneslength;
    int lanesheight;

    laneslength = length / 5;
    laneswidth = width / 5;
    lanesheight = height / 5;
    for (int s = 0; s < laneswidth; s++)
    {
       std::vector<std::vector<Block>> Vector3d;

        for (int i = 0; i < laneslength; i++)
        {

            std::vector<Block> Vector2d;
           
            
            for (int j = 0; j < lanesheight; j++)
            {
                
                Vector2d.push_back(Block(0, {s, i, j}));
            }
             Vector3d.push_back(Vector2d);
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

// places a block in the grid by taking its index in the grid and setting its id to the chosen value
void Grid::placeBlock(std::vector<Block> Blocks)
{
    for (int i = 0; i < Blocks.size(); i++)
    {
        int s = Blocks[i].getplace()[0];
        int x = Blocks[i].getplace()[1];
        int y = Blocks[i].getplace()[2];

        
        if (s < 0 || s >= (int)grid.size() ||
            x < 0 || x >= (int)grid[s].size() ||
            y < 0 || y >= (int)grid[s][x].size())
        {
                std::cout << "Error: Block " << Blocks[i].getId() << " has invalid place coordinates (" << s << ", " << x << ", " << y << "). Skipping this block." << std::endl;
            continue;
        }
        grid[s][x][y].Id = Blocks[i].getId();
        grid[s][x][y].coordnate[6] = Blocks[i].coordnate[6];
    }
}



// finds a block in a given grid by checking if it has the same id, returns the coordinates of that block and removes it from the grid by setting its id to 0
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



