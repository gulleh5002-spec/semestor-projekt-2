#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

// =======================UI WIDGETS=============================
{
    ui->setupUi(this);

    layers.push_back(std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize, 0)));

    ui->tableWidget->setRowCount(gridSize);
    ui->tableWidget->setColumnCount(gridSize);

    for (int row = {0}; row < gridSize; ++row) {
        for (int col = {0}; col < gridSize; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("");
            item->setBackground(Qt::white);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    connect(ui->tableWidget, &QTableWidget::cellClicked,
            this, &MainWindow::onCellClicked);
    connect(ui->pushButtonNewLayer, &QPushButton::clicked,
            this, &MainWindow::onNewLayerClicked);
}
//==================================================================================

void MainWindow::updateGrid()
{
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            bool hasBlock = layers[currentLayer][row][col] == 1;

            item->setBackground(hasBlock ? Qt::blue : Qt::white);
        }
    }
}

void MainWindow::onNewLayerClicked() //Button ++Layer
{
    layers.push_back(std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize, 0)));
    currentLayer = layers.size() - 1;
    updateGrid();
}

//TODO: Moce CellClicked into its own class
void MainWindow::onCellClicked(int row, int column)
{
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
