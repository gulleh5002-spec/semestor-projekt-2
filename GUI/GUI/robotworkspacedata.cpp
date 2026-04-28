#include "robotworkspacedata.h"

void RobotWorkspaceData::create(int width, int height)
{
    clear();

    m_width = width;
    m_height = height;
    m_created = true;
    addEmptyLayer();
}

void RobotWorkspaceData::clear()
{
    m_width = {0};
    m_height = {0};
    m_created = false;
    m_layers.clear();
}

bool RobotWorkspaceData::isCreated() const
{
    return m_created;
}

int RobotWorkspaceData::width() const
{
    return m_width;
}

int RobotWorkspaceData::height() const
{
    return m_height;
}

int RobotWorkspaceData::layerCount() const
{
    return static_cast<int>(m_layers.size());
}

bool RobotWorkspaceData::addLayer()
{
    if (!m_created) {
        return false;
    }

    addEmptyLayer();
    return true;
}

bool RobotWorkspaceData::isValidPosition(const GridPosition& position) const
{
    return m_created
           && position.z >= firstLayer
           && position.z < layerCount()
           && position.y >= 0
           && position.y < m_height
           && position.x >= 0
           && position.x < m_width;
}

bool RobotWorkspaceData::hasBlockAtPosition(const GridPosition& position) const
{
    if (!isValidPosition(position)) {
        return false;
    }

    return m_layers[position.z][position.y][position.x].hasBlock;
}

bool RobotWorkspaceData::setCellAtPosition(const GridPosition& position, const GridCell& cell)
{
    if (!isValidPosition(position)) {
        return false;
    }

    m_layers[position.z][position.y][position.x] = cell;
    return true;
}

void RobotWorkspaceData::addEmptyLayer()
{
    m_layers.push_back(std::vector<std::vector<GridCell>>(m_height, std::vector<GridCell>(m_width)));
}
