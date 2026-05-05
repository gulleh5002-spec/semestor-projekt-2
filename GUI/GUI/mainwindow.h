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
    void updateLayerControls();
    void updateWorkspaceSizePreview();
};
#endif // MAINWINDOW_H
