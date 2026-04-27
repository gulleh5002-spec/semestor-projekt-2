#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <vector>
#include <QMainWindow>
#include "QHeaderView"
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QBrush>

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
    std::vector<std::vector<std::vector<int>>> layers;
    int currentLayer = {0};
    bool workspaceCreated = false;

    int gridHeight = {5};
    int gridWidth = {4};
    //int gridSize = {6};

    void configureTableWidget();
    void clearWorkspace();
    void resetLayerData();
    void setWorkspaceInputEnabled(bool enabled);
    void setupTableGrid();
    void updateGrid();
    void updateLayerControls();
};
#endif // MAINWINDOW_H
