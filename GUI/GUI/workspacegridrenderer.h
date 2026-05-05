#ifndef WORKSPACEGRIDRENDERER_H
#define WORKSPACEGRIDRENDERER_H

class QTableWidget;
class RobotWorkspace;
// Klasse for at render UI grid og dens pas størelse
class WorkspaceGridRenderer
{
public:
    void configureTable(QTableWidget *table) const;
    void clearTable(QTableWidget *table) const;
    void setupGrid(QTableWidget *table, const RobotWorkspace& workspace) const;
    void updateGrid(QTableWidget *table, const RobotWorkspace& workspace) const;
    void updateCellSizes(QTableWidget *table, const RobotWorkspace& workspace) const;
};

#endif // WORKSPACEGRIDRENDERER_H
