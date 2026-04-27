#ifndef ROBOTWORKSPACE_H
#define ROBOTWORKSPACE_H

#include <vector>

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

    // Validering for at man ikke skriver og læser uden for m_layers
    bool hasBlockAtLayer(int x, int y, int layer) const;
    bool canPlaceBlockAtCurrentLayer(int x, int y) const;
    bool canPlaceBlockAtLayer(int x, int y, int layer) const;
    bool toggleBlockAtCurrentLayer(int x, int y);
//=================================================================

private:
    // Default lag
    static constexpr int firstLayer = {0};

    // Validere om gyldig input
    bool isValidPosition(int x, int y, int z) const;

    int m_width = {0};
    int m_height = {0};
    int m_currentLayer = {firstLayer};

    // Validerings check variabel for addLayer()
    bool m_created = false;
    std::vector<std::vector<std::vector<int>>> m_layers;
};

#endif // ROBOTWORKSPACE_H
