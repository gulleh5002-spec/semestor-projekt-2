#include "BuildPlanConsolePrinter.h"

#include <ostream>

namespace BuildPlanConsolePrinter
{
void print(const BuildPlan& buildPlan, std::ostream& output)
{
    output << "Workspace: width=" << buildPlan.workspace.width
           << " height=" << buildPlan.workspace.height
           << " layers=" << buildPlan.workspace.layerCount
           << " created=" << std::boolalpha << buildPlan.workspace.isCreated << '\n';

    output << "Blocks: " << buildPlan.blocks.size() << '\n';

    for (const BuildPlanBlock& block : buildPlan.blocks) {
        output << "Block " << block.id
               << ": x=" << block.x
               << " y=" << block.y
               << " z=" << block.z
               << " type=" << block.type << '\n';
    }
}
}
