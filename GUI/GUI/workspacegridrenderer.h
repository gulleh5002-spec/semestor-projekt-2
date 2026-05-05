#ifndef WORKSPACEGRIDRENDERER_H
#define WORKSPACEGRIDRENDERER_H

class QTableWidget;
class RobotWorkspace;
// Klasse for at render UI grid og dens passende størrelse.
class WorkspaceGridRenderer
{
public:
    void configureTable(QTableWidget *table) const;
    void clearTable(QTableWidget *table) const;
    void showWorkspace(QTableWidget *table, const RobotWorkspace& workspace) const;
    void refreshWorkspace(QTableWidget *table, const RobotWorkspace& workspace) const;

private:
    void createCells(QTableWidget *table, const RobotWorkspace& workspace) const;
    void updateCellColors(QTableWidget *table, const RobotWorkspace& workspace) const;
    void updateCellSizes(QTableWidget *table, const RobotWorkspace& workspace) const;
};

#endif // WORKSPACEGRIDRENDERER_H
