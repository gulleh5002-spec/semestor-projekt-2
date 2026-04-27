#pragma once
#include <iostream>
#include <string>
#include <vector>

class Block
{
private:
    
    std::vector<double> coordnate;
    std::vector<int> place;
    

public:
    // if status is 0 its nothing if its 1 its bulding block if its 2 its a scematik if its 3 its a placed block
    Block(int id,  std::vector<double> coordnate, int status, std::vector<int> place );
    ~Block();
    int getId();
    std::vector<double> getCoordnate();
    int getstatus();
    std::vector<int> getplace();
    int status;
    int Id;
};
