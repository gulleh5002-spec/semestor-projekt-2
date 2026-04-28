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
    for (int s = 0; s < lanesheight; s++)
    {
       std::vector<std::vector<Block>> Vector3d;

        for (int i = 0; i < laneslength; i++)
        {

            std::vector<Block> Vector2d;
           
            
            for (int j = 0; j < laneswidth; j++)
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
        grid[Blocks[i].getplace()[0]][Blocks[i].getplace()[1]][Blocks[i].getplace()[2]].Id = Blocks[i].getId();
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
    