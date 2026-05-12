#pragma once
#include <iostream>
#include <string>
#include <vector>

class Block
{
private:
    
    
    std::vector<int> place;
    

public:
    // if status is 0 its nothing if its 1 its bulding block if its 2 its a scematik if its 3 its a placed block
    Block(int id, std::vector<int> place );
    ~Block();
    int getId();
    std::vector<double> getCoordnate();
    std::vector<int> getplace();
    int Id;
    std::vector<double> coordnate;
};
