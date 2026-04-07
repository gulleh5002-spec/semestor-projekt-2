#include "Grid.h"
#include <iostream>
using namespace std;

// Constructor
Grid::Grid(int l, int h, double size, double z)
{
  length = l;
  height = h;
  blockSize = size;
  baseZ = z;

  // Lav 2D grid fyldt med tomme pointers
  grid.resize(height);

  for (int y = 0; y < height; y++)
  {
    grid[y].resize(length);
  }
}

// Sæt en klods (peger på en klods)
bool Grid::setBlock(unique_ptr<Block> block, int x, int y)
{
  if (grid[y][x] == nullptr)
  {
    block->setPosition(x, y);
    grid[y][x] = std::move(block); // 🔥 ownership flyttes
    return true;
  }
  return false;
}

// Fjern block (auto delete)
void Grid::deleteBlock(int x, int y)
{
  grid[y][x] = nullptr;
}

// Flyt klods fra (x1,y1) til (x2,y2)
bool Grid::moveBlock(int x1, int y1, int x2, int y2)
{
  if (grid[y1][x1] != nullptr && grid[y2][x2] == nullptr)
  {

    grid[y2][x2] = std::move(grid[y1][x1]); // flyt pointer
    grid[y1][x1] = nullptr;

    grid[y2][x2]->setPosition(x2, y2);

    return true;
  }
  return false;
}

// Print grid
void Grid::printGrid()
{
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < length; x++)
    {
      if (grid[y][x] == nullptr)
        cout << "0 ";
      else
        cout << "1 ";
    }
    cout << endl;
  }
}

Coord Grid::getWorldCoord(int x, int y)
{
  Coord c;
  c.x = x * blockSize;
  c.y = y * blockSize;
  c.z = baseZ;
  return c;
}

Coord Grid::getBlockCoord(int x, int y)
{
  if (grid[y][x] != nullptr)
  {
    return getWorldCoord(x, y);
  }

  // hvis tomt → return 0
  return {0, 0, 0};
}