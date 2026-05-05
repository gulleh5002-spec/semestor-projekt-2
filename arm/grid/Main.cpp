#include "Grid.h"
#include "Block.h"

#include <iostream>
#include <memory>

using namespace std;

int main()
{
  Grid grid(5, 5, 100, 0);

  // Lav blocks
  unique_ptr<Block> b1 = make_unique<Block>(0, 0);
  unique_ptr<Block> b2 = make_unique<Block>(0, 0);

  // Sæt dem i grid
  grid.setBlock(std::move(b1), 1, 1);

  grid.printGrid();

  Coord from = grid.getBlockCoord(1, 1);
  Coord too = grid.getWorldCoord(3, 3);

  cout << from.x << ", " << from.y << ", " << from.z << endl;
  cout << too.x << ", " << too.y << ", " << too.z << endl;

  grid.moveBlock(1, 1, 3, 3);
  // cout << "\nFlytter block...\n";
  // grid.moveBlock(1, 1, 3, 3);

  // grid.printGrid();

  // cout << "\nFjerner block...\n";
  // grid.deleteBlock(2, 2);

  grid.printGrid();

  return 0;
}