#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>
#include "Block.h"
using namespace std;

struct Coord
{
  double x;
  double y;
  double z;
};

class Grid
{
private:
  vector<vector<unique_ptr<Block>>> grid;
  int length;
  int height;

  double blockSize; // fx i mm
  double baseZ;     // fast højde
  

public:
  Grid(int l, int h, double size, double z, int tcp = 0);

  bool setBlock(unique_ptr<Block> block, int x, int y);
  void deleteBlock(int x, int y);
  bool moveBlock(int x1, int y1, int x2, int y2);
  void printGrid();

  Coord getWorldCoord(int x, int y);
  Coord getBlockCoord(int x, int y);
  int TCP;
};

#endif