#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Block.h"


class Grid {
private:
  int height;
  int width;
  int length;
  
  

public:
  Grid(int height, int width, int length, std::vector<double> grid_to_base);
  void makeGrid();
  void printGrid();
  void placeBlock(std::vector<Block> Blocks);
  std::vector<std::vector<std::vector<Block>>> grid;
  std::vector<double> findBlock(Block block);

  ~Grid();
  std::vector<double> grid_to_base;
};
