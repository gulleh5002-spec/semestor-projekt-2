#include "robotworkspace.h"

void RobotWorkspace::create(int width, int height)
{
    m_data.create(width, height);
    m_currentLayer = {firstLayer};
}

void RobotWorkspace::clear()
{
    m_data.clear();
    m_currentLayer = {firstLayer};
}

bool RobotWorkspace::isCreated() const
{
    return m_data.isCreated();
}

int RobotWorkspace::width() const
{
    return m_data.width();
}

int RobotWorkspace::height() const
{
    return m_data.height();
}

int RobotWorkspace::currentLayer() const
{
    return m_currentLayer;
}

int RobotWorkspace::layerCount() const
{
    return m_data.layerCount();
}

bool RobotWorkspace::addLayer()
{
    if (!m_data.addLayer()) {
        return false;
    }

    m_currentLayer = layerCount() - 1;
    return true;
}

bool RobotWorkspace::canGoToPreviousLayer() const
{
    return m_data.isCreated() && m_currentLayer > firstLayer;
}

bool RobotWorkspace::canGoToNextLayer() const
{
    return m_data.isCreated() && m_currentLayer < layerCount() - 1;
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
    return m_data.hasBlockAtPosition(position);
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
    if (!m_data.isValidPosition(position) || hasBlockAtPosition(position)) {
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
    if (!m_data.isValidPosition(position) || !hasBlockAtPosition(position)) {
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

    if (!m_data.isValidPosition(position)) {
        return false;
    }

    if (hasBlockAtPosition(position)) {
        if (!canRemoveBlockAtPosition(position)) {
            return false;
        }

        return m_data.setCellAtPosition(position, GridCell{});
    }

    if (!canPlaceBlockAtPosition(position)) {
        return false;
    }

    return m_data.setCellAtPosition(position, {true, BlockType::Default});
}

GridPosition RobotWorkspace::currentLayerPosition(int x, int y) const
{
    return positionAtLayer(x, y, m_currentLayer);
}

GridPosition RobotWorkspace::positionAtLayer(int x, int y, int layer) const
{
    return {x, y, layer};
}
