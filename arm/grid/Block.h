#ifndef BLOCK_H
#define BLOCK_H

class Block
{
private:
  int x;
  int y;
  int size; // its a cube so only one side lenght is needed

public:
  Block(int a, int b);

  int returnX();

  int returnY();

  void setPosition(int newX, int newY);
};

#endif