#pragma once
#include <iostream>
#include <string>
#include <vector>

class Block
{
private:
    
    
    std::vector<int> place;
    

public:
    // do so the block kooridnati is in the middle of a grid cell
    Block(int id, std::vector<int> place );
    ~Block();
    int getId();
    std::vector<double> getCoordnate();
    std::vector<int> getplace();
    int Id;
    std::vector<double> coordnate;
    int moveMethod = 0;
};
