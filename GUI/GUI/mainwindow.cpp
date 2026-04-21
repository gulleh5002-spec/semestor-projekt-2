#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    matrix = std::vector<std::vector<int>>(5, std::vector<int>(5, 0));

    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(5);

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
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
}
//TODO: Moce CellClicked into its own class
void MainWindow::onCellClicked(int row, int column)
{
    if (matrix[row][column] == 0) {
        matrix[row][column] = 1;
        ui->tableWidget->item(row, column)->setBackground(Qt::blue);
    } else {
        matrix[row][column] = 0;
        ui->tableWidget->item(row, column)->setBackground(Qt::white);
    }

    qDebug() << "Input værdi : R [" << row << "] og K [" << column << "] =" << matrix[row][column];

    qDebug() << "Hele Matrix grid værdi:";
    for (int r = 0; r < 5; ++r) {
        QString line;
        for (int c = 0; c < 5; ++c) {
            line += QString::number(matrix[r][c]) + " ";
        }
        qDebug() << line;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
