#include "workspacegridrenderer.h"

#include "robotworkspace.h"
#include "workspaceconstants.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <algorithm>

namespace
{
Qt::GlobalColor cellColorForPosition(const RobotWorkspace& workspace, int x, int y)
{
    const bool hasBlock = workspace.hasBlockAtCurrentLayer(x, y);

    if (hasBlock && !workspace.canRemoveBlockAtCurrentLayer(x, y)) {
        return Qt::green;
    }

    if (hasBlock) {
        return Qt::blue;
    }

    if (workspace.currentLayer() > WorkspaceConstants::firstLayer
        && workspace.canPlaceBlockAtCurrentLayer(x, y)) {
        return Qt::yellow;
    }

    return Qt::white;
}

int calculatedCellSize(QTableWidget *table, const RobotWorkspace& workspace)
{
    const int cellWidth = table->viewport()->width() / workspace.width();
    const int cellHeight = table->viewport()->height() / workspace.height();
    const int availableCellSize = std::min(cellWidth, cellHeight);

    return std::clamp(availableCellSize,
                      WorkspaceConstants::minimumGridCellSize,
                      WorkspaceConstants::maximumGridCellSize);
}
}

void WorkspaceGridRenderer::configureTable(QTableWidget *table) const
{
    table->horizontalHeader()->setVisible(false);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void WorkspaceGridRenderer::clearTable(QTableWidget *table) const
{
    table->clear();
    table->setRowCount(0);
    table->setColumnCount(0);
}

void WorkspaceGridRenderer::showWorkspace(QTableWidget *table, const RobotWorkspace& workspace) const
{
    table->clear();
    table->setRowCount(workspace.height());
    table->setColumnCount(workspace.width());

    createCells(table, workspace);
    refreshWorkspace(table, workspace);
}

void WorkspaceGridRenderer::refreshWorkspace(QTableWidget *table, const RobotWorkspace& workspace) const
{
    if (!workspace.isCreated()) {
        return;
    }

    updateCellColors(table, workspace);
    updateCellSizes(table, workspace);
}

void WorkspaceGridRenderer::createCells(QTableWidget *table, const RobotWorkspace& workspace) const
{
    for (int row = 0; row < workspace.height(); ++row) {
        for (int column = 0; column < workspace.width(); ++column) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("");
            item->setBackground(Qt::white);
            table->setItem(row, column, item);
        }
    }
}

void WorkspaceGridRenderer::updateCellColors(QTableWidget *table, const RobotWorkspace& workspace) const
{
    for (int row = 0; row < workspace.height(); ++row) {
        for (int column = 0; column < workspace.width(); ++column) {
            QTableWidgetItem *item = table->item(row, column);

            if (item == nullptr) {
                item = new QTableWidgetItem();
                table->setItem(row, column, item);
            }

            item->setBackground(cellColorForPosition(workspace, column, row));
        }
    }
}

void WorkspaceGridRenderer::updateCellSizes(QTableWidget *table, const RobotWorkspace& workspace) const
{
    if (!workspace.isCreated() || workspace.width() <= 0 || workspace.height() <= 0) {
        return;
    }

    const int cellSize = calculatedCellSize(table, workspace);

    for (int column = 0; column < workspace.width(); ++column) {
        table->setColumnWidth(column, cellSize);
    }

    for (int row = 0; row < workspace.height(); ++row) {
        table->setRowHeight(row, cellSize);
    }
}
