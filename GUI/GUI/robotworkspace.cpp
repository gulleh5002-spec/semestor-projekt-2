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
    return hasBlockAtLayer(x, y, m_currentLayer);
}

bool RobotWorkspace::hasBlockAtLayer(int x, int y, int layer) const
{
    if (!isValidPosition(x, y, layer)) {
        return false;
    }

    return m_layers[layer][y][x] == 1;
}

bool RobotWorkspace::toggleBlockAtCurrentLayer(int x, int y)
{
    if (!isValidPosition(x, y, m_currentLayer)) {
        return false;
    }

    m_layers[m_currentLayer][y][x] = m_layers[m_currentLayer][y][x] == 0 ? 1 : 0;
    return true;
}

bool RobotWorkspace::isValidPosition(int x, int y, int z) const
{
    return m_created
           && z >= firstLayer
           && z < layerCount()
           && y >= 0
           && y < m_height
           && x >= 0
           && x < m_width;
}
