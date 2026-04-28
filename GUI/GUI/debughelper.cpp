#include "debughelper.h"

#include <QDebug>

void DebugHelper::workspaceCreated(const RobotWorkspace& workspace)
{
    qDebug() << "[arbejdsomraarde] Oprettet"
             << "X:" << workspace.width()
             << "Y:" << workspace.height()
             << "Aktivt lag:" << workspace.currentLayer()
             << "Antal lag:" << workspace.layerCount();
}

void DebugHelper::workspaceCleared()
{
    qDebug() << "[Arbejdsomraarde] Ryddet";
}

void DebugHelper::layerChanged(const RobotWorkspace& workspace)
{
    qDebug() << "[Lag] Aktivt lag:" << workspace.currentLayer()
             << "Antal lag:" << workspace.layerCount();
}

void DebugHelper::blockPlacementUpdated(const RobotWorkspace& workspace, int x, int y)
{
    qDebug() << "[Klods] Opdateret"
             << "X:" << x
             << "Y:" << y
             << "Z:" << workspace.currentLayer()
             << "Har klods:" << workspace.hasBlockAtCurrentLayer(x, y);
}

void DebugHelper::blockPlacementRejected(const RobotWorkspace& workspace, int x, int y)
{
    qDebug() << "[Klods] Afvist"
             << "X:" << x
             << "Y:" << y
             << "Z:" << workspace.currentLayer()
             << "grund: mangler klods nedenunder";
}

void DebugHelper::blockRemovalRejected(const RobotWorkspace& workspace, int x, int y)
{
    qDebug() << "[Klods] Fjernelse afvist"
             << "X:" << x
             << "Y:" << y
             << "Z:" << workspace.currentLayer()
             << "grund: der staar en klods ovenpaa";
}
