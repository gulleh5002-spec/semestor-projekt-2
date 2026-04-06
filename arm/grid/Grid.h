#ifndef GRID_H
#define GRID_H

#include <vector>
using namespace std;

class Grid
{
private:
  vector<vector<int>> grid;
  int bredde;
  int hoejde;

public:
  Grid(int b, int h);

  void saetKlods(int x, int y);
  void fjernKlods(int x, int y);
  void flytKlods(int x1, int y1, int x2, int y2);
  void printGrid();
};

#endif