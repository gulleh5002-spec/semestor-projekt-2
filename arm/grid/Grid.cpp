#include "Grid.h"
#include <iostream>
using namespace std;

// Constructor
Grid::Grid(int b, int h)
{
  bredde = b;
  hoejde = h;

  // Lav 2D grid fyldt med 0
  grid.resize(hoejde, vector<int>(bredde, 0));
}

// Sæt en klods (1)
void Grid::saetKlods(int x, int y)
{
  if (x >= 0 && x < bredde && y >= 0 && y < hoejde)
  {
    grid[y][x] = 1;
  }
}

// Fjern klods (0)
void Grid::fjernKlods(int x, int y)
{
  if (x >= 0 && x < bredde && y >= 0 && y < hoejde)
  {
    grid[y][x] = 0;
  }
}

// Flyt klods fra (x1,y1) til (x2,y2)
void Grid::flytKlods(int x1, int y1, int x2, int y2)
{
  if (x1 >= 0 && x1 < bredde && y1 >= 0 && y1 < hoejde &&
      x2 >= 0 && x2 < bredde && y2 >= 0 && y2 < hoejde)
  {

    if (grid[y1][x1] == 1)
    {
      grid[y1][x1] = 0;
      grid[y2][x2] = 1;
    }
  }
}

// Print grid
void Grid::printGrid()
{
  for (int y = 0; y < hoejde; y++)
  {
    for (int x = 0; x < bredde; x++)
    {
      cout << grid[y][x] << " ";
    }
    cout << endl;
  }
}