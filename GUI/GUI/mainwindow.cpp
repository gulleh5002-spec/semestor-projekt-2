#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "debughelper.h"
#include "workspaceconstants.h"
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

// =======================UI WIDGETS=============================
{


    ui->setupUi(this);

    ui->spinBoxWitdh->setValue(WorkspaceConstants::defaultGridWidth);
    ui->spinBoxHeight->setValue(WorkspaceConstants::defaultGridHeight);
    ui->statusbar->showMessage("Angiv størrelse på arbejdsområdet og tryk Opret grid");

    configureTableWidget();
    clearWorkspace();

    connect(ui->tableWidget, &QTableWidget::cellClicked,
            this, &MainWindow::onCellClicked);
    connect(ui->pushButtonNewLayer, &QPushButton::clicked,
            this, &MainWindow::onNewLayerClicked);
    connect(ui->pushButtonPreviousLayer, &QPushButton::clicked,
            this, &MainWindow::onPreviousLayerClicked);
    connect(ui->pushButtonNextLayer, &QPushButton::clicked,
            this, &MainWindow::onNextLayerClicked);
    connect(ui->pushButtonCreateGrid, &QPushButton::clicked,
            this, &MainWindow::onCreateGridClicked);
    connect(ui->pushButtonNewWorkspace, &QPushButton::clicked,
            this, &MainWindow::onNewWorkspaceClicked);
}
//==================================================================================
void MainWindow::onCreateGridClicked()
{
    const int gridWidth = ui->spinBoxWitdh->value();
    const int gridHeight = ui->spinBoxHeight->value();

    workspace.create(gridWidth, gridHeight);
    setupTableGrid();

    setWorkspaceInputEnabled(false);
    ui->tableWidget->setEnabled(true);
    ui->pushButtonNewLayer->setEnabled(true);
    ui->pushButtonNewWorkspace->setEnabled(true);
    updateGrid();
    updateLayerControls();
    DebugHelper::workspaceCreated(workspace);
    ui->statusbar->showMessage("Arbejdsområde oprettet. Størrelsen er låst indtil Nyt arbejdsområde.");
}

void MainWindow::onNewWorkspaceClicked()
{
    clearWorkspace();
    DebugHelper::workspaceCleared();
    ui->statusbar->showMessage("Nyt arbejdsområde: angiv størrelse og tryk Opret grid");
}

void MainWindow::configureTableWidget()
{
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
}

void MainWindow::clearWorkspace()
{
    workspace.clear();

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setEnabled(false);

    setWorkspaceInputEnabled(true);
    ui->pushButtonNewLayer->setEnabled(false);
    ui->pushButtonPreviousLayer->setEnabled(false);
    ui->pushButtonNextLayer->setEnabled(false);
    ui->pushButtonNewWorkspace->setEnabled(false);
    ui->labelCurrentLayer->setText("Lag: -");
}

void MainWindow::setWorkspaceInputEnabled(bool enabled)
{
    ui->spinBoxWitdh->setEnabled(enabled);
    ui->spinBoxHeight->setEnabled(enabled);
    ui->pushButtonCreateGrid->setEnabled(enabled);
}

void MainWindow::setupTableGrid()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(workspace.height());
    ui->tableWidget->setColumnCount(workspace.width());

    for (int row = 0; row < workspace.height(); ++row) {
        for (int col = 0; col < workspace.width(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("");
            item->setBackground(Qt::white);
            ui->tableWidget->setItem(row, col, item);
        }
    }
}

void MainWindow::updateGrid()
{
    if (!workspace.isCreated()) {
        return;
    }

    for (int row = 0; row < workspace.height(); ++row) {
        for (int col = 0; col < workspace.width(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            bool hasBlock = workspace.hasBlockAtCurrentLayer(col, row);

            if (hasBlock && !workspace.canRemoveBlockAtCurrentLayer(col, row)) {
                item->setBackground(Qt::green);
            } else if (hasBlock) {
                item->setBackground(Qt::blue);
            } else if (workspace.currentLayer() > 0 && workspace.canPlaceBlockAtCurrentLayer(col, row)) {
                item->setBackground(Qt::yellow);
            } else {
                item->setBackground(Qt::white);
            }
        }
    }
}

void MainWindow::onNewLayerClicked() //Button ++Layer
{
    if (!workspace.addLayer()) {
        return;
    }

    updateGrid();
    updateLayerControls();
    DebugHelper::layerChanged(workspace);
}

void MainWindow::onPreviousLayerClicked()
{
    if (!workspace.goToPreviousLayer()) {
        return;
    }

    updateGrid();
    updateLayerControls();
    DebugHelper::layerChanged(workspace);
}

void MainWindow::onNextLayerClicked()
{
    if (!workspace.goToNextLayer()) {
        return;
    }

    updateGrid();
    updateLayerControls();
    DebugHelper::layerChanged(workspace);
}

//TODO: Moce CellClicked into its own class
void MainWindow::onCellClicked(int row, int column)
{
    if (workspace.hasBlockAtCurrentLayer(column, row)
        && !workspace.canRemoveBlockAtCurrentLayer(column, row)) {
        DebugHelper::blockRemovalRejected(workspace, column, row);
        ui->statusbar->showMessage("Kan ikke fjerne klods: der er en klods ovenpå.");
        return;
    }

    if (!workspace.toggleBlockAtCurrentLayer(column, row)) {
        DebugHelper::blockPlacementRejected(workspace, column, row);
        ui->statusbar->showMessage("Kan ikke placere klods: der mangler en klods nedenunder.");
        return;
    }

    updateGrid();
    updateLayerControls();
    DebugHelper::blockPlacementUpdated(workspace, column, row);
    ui->statusbar->showMessage("Klodsplacering opdateret.");
}

void MainWindow::updateLayerControls()
{
    ui->labelCurrentLayer->setText(QString("Lag: %1").arg(workspace.currentLayer()));
    ui->pushButtonPreviousLayer->setEnabled(workspace.canGoToPreviousLayer());
    ui->pushButtonNextLayer->setEnabled(workspace.canGoToNextLayer());
}

MainWindow::~MainWindow()
{
    delete ui;
}
