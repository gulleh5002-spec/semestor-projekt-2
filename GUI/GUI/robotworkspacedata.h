#ifndef ROBOTWORKSPACEDATA_H
#define ROBOTWORKSPACEDATA_H

#include "gridcell.h"
#include "gridposition.h"

#include <vector>

class RobotWorkspaceData
{
public:
    void create(int width, int height);
    // Clear funktion for grid
    void clear();

    // Data for grid
    bool isCreated() const;
    int width() const;
    int height() const;
    int layerCount() const;

    bool addLayer();
    bool isValidPosition(const GridPosition& position) const;
    bool hasBlockAtPosition(const GridPosition& position) const;
    bool setCellAtPosition(const GridPosition& position, const GridCell& cell);

private:
    static constexpr int firstLayer = {0};

    void addEmptyLayer();

    int m_width = {0};
    int m_height = {0};
    bool m_created = false;

    // m_layers gemmer indholder [z][y][x] som en vektor
    std::vector<std::vector<std::vector<GridCell>>> m_layers;
};

#endif // ROBOTWORKSPACEDATA_H
