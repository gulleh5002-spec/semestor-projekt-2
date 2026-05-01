#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  
#include "newgrid/Grid.h"
#include "newgrid/Block.h"
#include "GripperClient.h"


int main()
{
    //RobotArm gulle("192.168.1.100", 1, 1.0);
    RobotArm magnum("127.0.0.1", 1.0, 1.0);
    std::vector<double> point1 = { 0.4, 0.4, 0.1, 3.14, 0.0, 0.0 };
    std::vector<double> point2 = { 0.2, 0.2, 0.3, 3.14, 0.0, 0.0 };
    //gulle.moveblock(point1, point2);
    //gulle.moveblock(point1, point2);
    //magnum.movetool(point1);
    //gulle.movetool({ 0.4, 0.4, 0.15, 3.14, 0.0, 0.0 }, 0.5, 0.5, {-0.2 ,-0.2 ,0, 0, 0, 2.3732 });
    //gulle>.movetool({ 0.4, 0, 0.5, 3.14, 0.0, 0.0 });
    //gulle.home();
    std::vector<Block> blocks = 
    {
    Block(1, {0, 0, 1}),
    //Block(1, {1, 2, 1}),
    //Block(1, {4, 2, 1}),
    //Block(1, {0, 2, 1}),
    //Block(1, {0, 2, 2})
    };
    Grid place(40, 40, 100, {0.2 ,0.2 ,0, 0, 0, 0});
    Grid take(40, 40, 100, {0.6 ,0.2, 0, 0, 0, 0});
    
    take.placeBlock(blocks);
    
    magnum.build(take, place, blocks);
    //magnum.moveToGridPos(place, blocks[1]);

    // take skal ganges på place evt lav en frame med kun rotaton og igne translation der efter gange identits matrice med translation eller trai på den
    // evt lav en test for at tjekke siguleretet
    //gulle.moveToGridPos(take, blocks[0]);
    //gulle.moveToGridPos(place, blocks[0]);
    return 0;
}



//to doo
// fix matrix multiplaction så de passer evt med nogle funktioner
// går grid location letter 
// tcp offset og forbindelse
// lav en en funktion som kan placere blokke i midten
// mere smooth bevæglse



