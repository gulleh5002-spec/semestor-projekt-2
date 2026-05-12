#ifndef BUILDPLAN_H
#define BUILDPLAN_H

#include <string>
#include <vector>

// Beskriver arbejdsområdet fra GUI
struct WorkspaceSize
{
    int width = {0};
    int height = {0};
    int layerCount = {0};
    bool isCreated = false;
};

// Beskriver en klods fra json fil
struct BuildPlanBlock
{
    int id = {0};
    int x = {0};
    int y = {0};
    int z = {0};
    std::string type;
};


struct BuildPlan
{
    WorkspaceSize workspace;
    std::vector<BuildPlanBlock> blocks;
};

#endif // BUILDPLAN_H
