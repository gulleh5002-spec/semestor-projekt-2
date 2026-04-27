#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

// =======================UI WIDGETS=============================
{


    ui->setupUi(this);

    ui->spinBoxWitdh->setValue(gridWidth);
    ui->spinBoxHeight->setValue(gridHeight);
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
    gridWidth = ui->spinBoxWitdh->value();
    gridHeight = ui->spinBoxHeight->value();

    resetLayerData();
    setupTableGrid();

    workspaceCreated = true;
    setWorkspaceInputEnabled(false);
    ui->tableWidget->setEnabled(true);
    ui->pushButtonNewLayer->setEnabled(true);
    ui->pushButtonNewWorkspace->setEnabled(true);
    updateGrid();
    updateLayerControls();
    ui->statusbar->showMessage("Arbejdsområde oprettet. Størrelsen er låst indtil Nyt arbejdsområde.");
}

void MainWindow::onNewWorkspaceClicked()
{
    clearWorkspace();
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
    layers.clear();
    currentLayer = 0;
    workspaceCreated = false;

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

void MainWindow::resetLayerData()
{
    layers.clear();
    layers.push_back(std::vector<std::vector<int>>(gridHeight, std::vector<int>(gridWidth, 0)));
    currentLayer = 0;
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
    ui->tableWidget->setRowCount(gridHeight);
    ui->tableWidget->setColumnCount(gridWidth);

    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("");
            item->setBackground(Qt::white);
            ui->tableWidget->setItem(row, col, item);
        }
    }
}

void MainWindow::updateGrid()
{
    if (!workspaceCreated || layers.empty()) {
        return;
    }

    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            bool hasBlock = layers[currentLayer][row][col] == 1;

            item->setBackground(hasBlock ? Qt::blue : Qt::white);
        }
    }
}

void MainWindow::onNewLayerClicked() //Button ++Layer
{
    if (!workspaceCreated) {
        return;
    }

    layers.push_back(std::vector<std::vector<int>>(gridHeight, std::vector<int>(gridWidth, 0)));
    currentLayer = layers.size() - 1;
    updateGrid();
    updateLayerControls();
}

void MainWindow::onPreviousLayerClicked()
{
    if (!workspaceCreated || currentLayer == 0) {
        return;
    }

    --currentLayer;
    updateGrid();
    updateLayerControls();
}

void MainWindow::onNextLayerClicked()
{
    if (!workspaceCreated || currentLayer >= static_cast<int>(layers.size()) - 1) {
        return;
    }

    ++currentLayer;
    updateGrid();
    updateLayerControls();
}

//TODO: Moce CellClicked into its own class
void MainWindow::onCellClicked(int row, int column)
{
    if (!workspaceCreated) {
        return;
    }

    if (layers[currentLayer][row][column] == 0) {
        layers[currentLayer][row][column] = {1};
        ui->tableWidget->item(row, column)->setBackground(Qt::blue);
    } else {
        layers[currentLayer][row][column] = {0};
        ui->tableWidget->item(row, column)->setBackground(Qt::white);
    }

    qDebug() << "Lag:" << currentLayer
             << "Række:" << row
             << "Kolonne:" << column
             << "Værdi:" << layers[currentLayer][row][column];

    updateGrid();
    updateLayerControls();
}

void MainWindow::updateLayerControls()
{
    ui->labelCurrentLayer->setText(QString("Lag: %1").arg(currentLayer));
    ui->pushButtonPreviousLayer->setEnabled(workspaceCreated && currentLayer > 0);
    ui->pushButtonNextLayer->setEnabled(workspaceCreated && currentLayer < static_cast<int>(layers.size()) - 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
