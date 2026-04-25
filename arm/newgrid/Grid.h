#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Block.h"

class Grid {
private:
  int height;
  int width;
  
  std::vector<std::vector<Block>> grid;

public:
  Grid(int height, int width);
  void makeGrid();
  void makeBuldingGrid(int height, int width);
  void makeTakingGride(int height, int width);
  void printGrid();
  ~Grid();
};
