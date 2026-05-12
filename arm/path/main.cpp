#pragma warning(disable: 4996 4267)
#include "RobotArm.h"
#include <iostream>
#include <vector>  
#include "newgrid/Grid.h"
#include "newgrid/Block.h"
#include "GripperClient.h"
#include "newgrid/Compiler.h"





int main()
{


    RobotArm gulle("192.168.1.100", 1, 1.0);
    //RobotArm magnum("127.0.0.1", 1.0, 1.0);
    std::vector<double> point1 = { 0.4, 0.4, 0.2, 3.14, 0.0, 0.0 };
    std::vector<double> point2 = { 0.2, 0.2, 0.2, 3.14, 0.0, 0.0 };
    //gulle.moveblock(point1, point2);
    //gulle.moveblock(point1, point2);
    
    //gulle.movetool({ 0.4, 0.4, 0.15, 3.14, 0.0, 0.0 }, 0.5, 0.5, {-0.2 ,-0.2 ,0, 0, 0, 2.3732 });
    //gulle>.movetool({ 0.4, 0, 0.5, 3.14, 0.0, 0.0 });
    //gulle.home();
    std::vector<Block> takeblocks = 
    {
    Block(1, {1, 2, 0}),
    Block(1, {2, 2, 0}),
    Block(1, {3, 2, 0}),
    Block(1, {1, 0, 0}),
    Block(1, {2, 0, 0}),
    Block(1, {3, 0, 0})
    };

    std::vector<Block> placeblocks = 
    {
    Block(1, {0, 3, 0}),
    Block(1, {1, 3, 0}),
    Block(1, {2, 3, 0}),
    Block(1, {0, 3, 1}),
    Block(1, {1, 3, 1}),
    Block(1, {2, 3, 1}),
    //Block(1, {0, 2, 2})
    };
   


    Grid place(40, 40, 100, {0.2 ,0.2 ,0, 0, 0, 0});
    Grid take(40, 40, 100, {0.6 ,0.2, 0, 0, 0, 0});
    
    //structur.placeBlock(placeblocks);
    //gulle.drop();
    gulle.build(take, place, placeblocks, takeblocks);

     
    //gulle.drop();
    //gulle.moveToGridPos(place, takeblocks[1]);

    // take skal ganges på place evt lav en frame med kun rotaton og igne translation der efter gange identits matrice med translation eller trai på den
    // evt lav en test for at tjekke siguleretet

   
    //gulle.moveToGridPos(take, placeblocks[0]);
    //gulle.take();

    return 0;

    
}



//to doo
// fix matrix multiplaction så de passer evt med nogle funktioner
// går grid location letter 
// tcp offset og forbindelse
// lav en en funktion som kan placere blokke i midten
// mere smooth bevæglse



