#include "Grid.h"

Grid::Grid(int length, int width, int height, std::vector<double> grid_to_base) : length(length), width(width), height(height), grid_to_base(grid_to_base)
{
    makeGrid();
}

Grid::~Grid() {}



// laver et 3d grid ved at lave 3 nested for loops og pushe blocks ind i et 3d vector grid. 
// her bliver blocke objeterne så dannet med det koordiant der passer i deres frame
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

// placere en block i gridet ved at tagede dens index i gridet og sætte id lig det du valgte
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
            std::cout << "placeBlock: [" << s << "][" << x << "][" << y
                      << "] out of bounds (grid size: " << grid.size() << "x"
                      << (grid.empty() ? 0 : grid[0].size()) << "x"
                      << (grid.empty() || grid[0].empty() ? 0 : grid[0][0].size()) << ")\n";
            continue;
        }
        grid[s][x][y].Id = Blocks[i].getId();
        grid[s][x][y].coordnate[6] = Blocks[i].coordnate[6];
    }
}



// finder en block i i et given grid via at tjekke om de har samme id og returnere koordinaterne for den block og slette den block fra gridet ved at sætte id til 0
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



