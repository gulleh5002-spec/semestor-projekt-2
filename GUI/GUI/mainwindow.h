#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "QHeaderView"
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QBrush>
#include "robotworkspace.h"

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

private:
    Ui::MainWindow *ui;
    RobotWorkspace workspace;

    void configureTableWidget();
    void clearWorkspace();
    void setWorkspaceInputEnabled(bool enabled);
    void setupTableGrid();
    void updateGrid();
    void updateLayerControls();
    void updateWorkspaceSizePreview();
};
#endif // MAINWINDOW_H
