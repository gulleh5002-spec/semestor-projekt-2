#include "Block.h"

Block::Block(int a, int b)
{
  x = a;
  y = b;
  size = 10;
}

int Block::returnX()
{
  return x;
}

int Block::returnY()
{
  return y;
}

void Block::setPosition(int newX, int newY)
{
  x = newX;
  y = newY;
}