#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "robotworkspace.h"
#include "workspacegridrenderer.h"

class QResizeEvent;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
// main klasse for GUI
class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void onCellClicked(int row, int column);
    void onNewLayerClicked();
    void onPreviousLayerClicked();
    void onNextLayerClicked();
    void onCreateGridClicked();
    void onNewWorkspaceClicked();
    void onBuildClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    WorkspaceGridRenderer gridRenderer;
    RobotWorkspace workspace;

    void clearWorkspace();
    void setWorkspaceInputEnabled(bool enabled);
    void updateBlockCountControls();
    void updateLayerControls();
    void updateWorkspaceSizePreview();
};
#endif // MAINWINDOW_H
