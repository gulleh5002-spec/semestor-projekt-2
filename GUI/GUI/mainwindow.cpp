#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "debughelper.h"
#include "workspaceconstants.h"
#include <QResizeEvent>
#include <QSpinBox>
#include <QString>

 //Test json handler data
#include <QDebug>
#include "workspacedatajsonhandler.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

// =======================UI WIDGETS=============================
{


    ui->setupUi(this);

    ui->spinBoxWitdh->setPrefix("B: ");
    ui->spinBoxHeight->setPrefix("H: ");
    ui->spinBoxWitdh->setToolTip("Bredde i antal gridfelter");
    ui->spinBoxHeight->setToolTip("Højde i antal gridfelter");
    ui->spinBoxWitdh->setValue(WorkspaceConstants::defaultGridWidth);
    ui->spinBoxHeight->setValue(WorkspaceConstants::defaultGridHeight);

    gridRenderer.configureTable(ui->tableWidget);
    clearWorkspace();
    updateWorkspaceSizePreview();

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
    connect(ui->spinBoxWitdh, qOverload<int>(&QSpinBox::valueChanged),
            this, [this]() { updateWorkspaceSizePreview(); });
    connect(ui->spinBoxHeight, qOverload<int>(&QSpinBox::valueChanged),
            this, [this]() { updateWorkspaceSizePreview(); });
}
//==================================================================================
void MainWindow::onCreateGridClicked()
{
    const int gridWidth = ui->spinBoxWitdh->value();
    const int gridHeight = ui->spinBoxHeight->value();

    workspace.create(gridWidth, gridHeight);
    gridRenderer.setupGrid(ui->tableWidget, workspace);

    setWorkspaceInputEnabled(false);
    ui->tableWidget->setEnabled(true);
    ui->pushButtonNewLayer->setEnabled(true);
    ui->pushButtonNewWorkspace->setEnabled(true);
    gridRenderer.updateGrid(ui->tableWidget, workspace);
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

void MainWindow::clearWorkspace()
{
    workspace.clear();

    gridRenderer.clearTable(ui->tableWidget);
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

void MainWindow::onNewLayerClicked() //Button ++Layer
{
    if (!workspace.addLayer()) {
        return;
    }

    gridRenderer.updateGrid(ui->tableWidget, workspace);
    updateLayerControls();
    DebugHelper::layerChanged(workspace);
}

void MainWindow::onPreviousLayerClicked()
{
    if (!workspace.goToPreviousLayer()) {
        return;
    }

    gridRenderer.updateGrid(ui->tableWidget, workspace);
    updateLayerControls();
    DebugHelper::layerChanged(workspace);
}

void MainWindow::onNextLayerClicked()
{
    if (!workspace.goToNextLayer()) {
        return;
    }

    gridRenderer.updateGrid(ui->tableWidget, workspace);
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

    gridRenderer.updateGrid(ui->tableWidget, workspace);
    updateLayerControls();
    DebugHelper::blockPlacementUpdated(workspace, column, row);
    ui->statusbar->showMessage("Klodsplacering opdateret.");
    // Json handler test funktio
    qDebug().noquote() << WorkspaceDataJsonHandler::toJsonString(workspace);
}

void MainWindow::updateLayerControls()
{
    ui->labelCurrentLayer->setText(QString("Lag: %1").arg(workspace.currentLayer()));
    ui->pushButtonPreviousLayer->setEnabled(workspace.canGoToPreviousLayer());
    ui->pushButtonNextLayer->setEnabled(workspace.canGoToNextLayer());
}

void MainWindow::updateWorkspaceSizePreview()
{
    if (!ui->pushButtonCreateGrid->isEnabled()) {
        return;
    }

    ui->statusbar->showMessage(QString("Valgt arbejdsområde: %1 bredde x %2 højde. Tryk Opret grid.")
                                   .arg(ui->spinBoxWitdh->value())
                                   .arg(ui->spinBoxHeight->value()));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    gridRenderer.updateCellSizes(ui->tableWidget, workspace);
}

MainWindow::~MainWindow()
{
    delete ui;
}
