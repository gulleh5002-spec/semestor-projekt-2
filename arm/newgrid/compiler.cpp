#include "compiler.h" 




compiler::compiler(/* args */)
{
}

compiler::~compiler()
{
}




std::vector<Block> compiler::compileplace(std::vector<Block> Blocks)
{
    for (int i = 0; i < Blocks.size(); i++)
    {
        for (int j = i + 1; j < Blocks.size(); j++)
        {
            bool SameZ = Blocks[i].getplace()[2] == Blocks[j].getplace()[2];
            bool sameY = Blocks[i].getplace()[1] == Blocks[j].getplace()[1];
            bool adjacentX = abs(Blocks[i].getplace()[0] - Blocks[j].getplace()[0]) == 1;

            if (sameY && adjacentX && SameZ)
            {
                Blocks[j].coordnate[6] = -3.14/2;
            }
        }
    }

    return Blocks;
}


std::vector<Block> compiler::compiletake(std::vector<Block> Blocks)
{
     for (int i = 0; i < Blocks.size(); i++)
    {
        for (int j = i; j < Blocks.size(); j++)
        {
            bool SameZ = Blocks[i].getplace()[2] == Blocks[j].getplace()[2];
            bool sameY = Blocks[i].getplace()[1] == Blocks[j].getplace()[1];
            bool adjacentX = abs(Blocks[i].getplace()[0] - Blocks[j].getplace()[0]) == 1;

            if (sameY && adjacentX && SameZ)
            {
                Blocks[i].coordnate[6] = -3.14/2;
                Blocks[j].coordnate[6] = -3.14/2;
            }
        }
    }

    return Blocks;
}