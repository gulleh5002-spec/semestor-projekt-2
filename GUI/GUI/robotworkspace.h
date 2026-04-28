#ifndef ROBOTWORKSPACE_H
#define ROBOTWORKSPACE_H

#include "gridposition.h"
#include "robotworkspacedata.h"

class RobotWorkspace
{
public:
    // Initialisering samt livscyklus af grid
    void create(int width, int height);
    void clear();
    // Tjek om grid er skapt
    bool isCreated() const;

    // Dimensioner af grid
    int width() const;
    int height() const;
//=================================================================

    // Forward initialisering af lag funktioner
    int currentLayer() const;
    int layerCount() const;
    bool addLayer();

    // Lag validering
    bool canGoToPreviousLayer() const;
    bool canGoToNextLayer() const;

    bool goToPreviousLayer();
    bool goToNextLayer();
//================================================================

    // Klods data på lag
    bool hasBlockAtCurrentLayer(int x, int y) const;

    // Validering og regler for klodsdata
    bool hasBlockAtLayer(int x, int y, int layer) const;
    bool hasBlockAtPosition(const GridPosition& position) const;
    bool canPlaceBlockAtCurrentLayer(int x, int y) const;
    bool canPlaceBlockAtLayer(int x, int y, int layer) const;
    bool canPlaceBlockAtPosition(const GridPosition& position) const;
    bool canRemoveBlockAtCurrentLayer(int x, int y) const;
    bool canRemoveBlockAtLayer(int x, int y, int layer) const;
    bool canRemoveBlockAtPosition(const GridPosition& position) const;
    bool toggleBlockAtCurrentLayer(int x, int y);
//=================================================================

private:
    // Default lag
    static constexpr int firstLayer = {0};

    // Position helpers
    GridPosition currentLayerPosition(int x, int y) const;
    GridPosition positionAtLayer(int x, int y, int layer) const;
    int m_currentLayer = {firstLayer};
    RobotWorkspaceData m_data;
};

#endif // ROBOTWORKSPACE_H
