#ifndef ROBOTWORKSPACE_H
#define ROBOTWORKSPACE_H

#include <vector>

class RobotWorkspace
{
public:
    // Initialisering samt livs cyklus af Grid
    void create(int width, int height);
    void clear();
    bool isCreated() const;

    // Deminsioner af Grid
    int width() const;
    int height() const;

    // Lag
    int currentLayer() const;
    int layerCount() const;
    bool addLayer();
    bool canGoToPreviousLayer() const;
    bool canGoToNextLayer() const;
    bool goToPreviousLayer();
    bool goToNextLayer();

    // Klods data
    bool hasBlockAtCurrentLayer(int x, int y) const;
    bool hasBlock(int x, int y, int z) const;
    bool toggleBlockAtCurrentLayer(int x, int y);

    // Member funktioner og variabler
private:
    bool isValidPosition(int x, int y, int z) const;

    int m_width = {0};
    int m_height = {0};
    int m_currentLayer = {0};
    bool m_created = false;
    std::vector<std::vector<std::vector<int>>> m_layers;
};

#endif // ROBOTWORKSPACE_H
