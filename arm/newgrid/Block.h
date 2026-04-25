#pragma once
#include <iostream>
#include <string>
#include <vector>

class Block
{
private:
    int id;
    std::vector<double> coordnate;
    

public:
    Block(int id,  std::vector<double> coordnate);
    ~Block();
    int getId();
    std::vector<double> getCoordnate();
};
