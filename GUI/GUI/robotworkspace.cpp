#include "robotworkspace.h"

void RobotWorkspace::create(int width, int height)
{
    clear();

    m_width = width;
    m_height = height;
    m_currentLayer = {firstLayer};
    m_created = true;
    m_layers.push_back(std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0)));
}

void RobotWorkspace::clear()
{
    m_width = {0};
    m_height = {0};
    m_currentLayer = {firstLayer};
    m_created = false;
    m_layers.clear();
}

bool RobotWorkspace::isCreated() const
{
    return m_created;
}

int RobotWorkspace::width() const
{
    return m_width;
}

int RobotWorkspace::height() const
{
    return m_height;
}

int RobotWorkspace::currentLayer() const
{
    return m_currentLayer;
}

int RobotWorkspace::layerCount() const
{
    return static_cast<int>(m_layers.size());
}

bool RobotWorkspace::addLayer()
{
    if (!m_created) {
        return false;
    }

    m_layers.push_back(std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0)));
    m_currentLayer = layerCount() - 1;
    return true;
}

bool RobotWorkspace::canGoToPreviousLayer() const
{
    return m_created && m_currentLayer > 0;
}

bool RobotWorkspace::canGoToNextLayer() const
{
    return m_created && m_currentLayer < layerCount() - 1;
}

bool RobotWorkspace::goToPreviousLayer()
{
    if (!canGoToPreviousLayer()) {
        return false;
    }

    --m_currentLayer;
    return true;
}

bool RobotWorkspace::goToNextLayer()
{
    if (!canGoToNextLayer()) {
        return false;
    }

    ++m_currentLayer;
    return true;
}

bool RobotWorkspace::hasBlockAtCurrentLayer(int x, int y) const
{
    return hasBlockAtPosition(currentLayerPosition(x, y));
}

bool RobotWorkspace::hasBlockAtLayer(int x, int y, int layer) const
{
    return hasBlockAtPosition(positionAtLayer(x, y, layer));
}

bool RobotWorkspace::hasBlockAtPosition(const GridPosition& position) const
{
    if (!isValidPosition(position)) {
        return false;
    }

    return m_layers[position.z][position.y][position.x] == 1;
}

bool RobotWorkspace::canPlaceBlockAtCurrentLayer(int x, int y) const
{
    return canPlaceBlockAtPosition(currentLayerPosition(x, y));
}

bool RobotWorkspace::canPlaceBlockAtLayer(int x, int y, int layer) const
{
    return canPlaceBlockAtPosition(positionAtLayer(x, y, layer));
}

bool RobotWorkspace::canPlaceBlockAtPosition(const GridPosition& position) const
{
    if (!isValidPosition(position) || hasBlockAtPosition(position)) {
        return false;
    }

    if (position.z == firstLayer) {
        return true;
    }

    return hasBlockAtPosition({position.x, position.y, position.z - 1});
}

bool RobotWorkspace::canRemoveBlockAtCurrentLayer(int x, int y) const
{
    return canRemoveBlockAtPosition(currentLayerPosition(x, y));
}

bool RobotWorkspace::canRemoveBlockAtLayer(int x, int y, int layer) const
{
    return canRemoveBlockAtPosition(positionAtLayer(x, y, layer));
}

bool RobotWorkspace::canRemoveBlockAtPosition(const GridPosition& position) const
{
    if (!isValidPosition(position) || !hasBlockAtPosition(position)) {
        return false;
    }

    const int layerAbove = position.z + 1;
    if (layerAbove >= layerCount()) {
        return true;
    }

    return !hasBlockAtPosition({position.x, position.y, layerAbove});
}

bool RobotWorkspace::toggleBlockAtCurrentLayer(int x, int y)
{
    const GridPosition position = currentLayerPosition(x, y);

    if (!isValidPosition(position)) {
        return false;
    }

    if (hasBlockAtPosition(position)) {
        if (!canRemoveBlockAtPosition(position)) {
            return false;
        }

        m_layers[position.z][position.y][position.x] = 0;
        return true;
    }

    if (!canPlaceBlockAtPosition(position)) {
        return false;
    }

    m_layers[position.z][position.y][position.x] = 1;
    return true;
}

GridPosition RobotWorkspace::currentLayerPosition(int x, int y) const
{
    return positionAtLayer(x, y, m_currentLayer);
}

GridPosition RobotWorkspace::positionAtLayer(int x, int y, int layer) const
{
    return {x, y, layer};
}

bool RobotWorkspace::isValidPosition(const GridPosition& position) const
{
    return m_created
           && position.z >= firstLayer
           && position.z < layerCount()
           && position.y >= 0
           && position.y < m_height
           && position.x >= 0
           && position.x < m_width;
}
