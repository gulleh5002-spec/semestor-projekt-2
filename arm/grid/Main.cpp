#include <iostream>
#include "Grid.h"

using namespace std;

int main()
{
  Grid mitGrid(5, 4); // bredde = 5, højde = 4

  mitGrid.saetKlods(1, 1);
  mitGrid.saetKlods(2, 2);

  cout << "Start grid:" << endl;
  mitGrid.printGrid();

  mitGrid.flytKlods(1, 1, 3, 0);

  cout << "\nEfter flytning:" << endl;
  mitGrid.printGrid();

  mitGrid.fjernKlods(2, 2);

  cout << "\nEfter fjernelse:" << endl;
  mitGrid.printGrid();

  return 0;
}