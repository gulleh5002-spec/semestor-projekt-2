#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QHeaderView"
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::onCellClicked(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, column);

    if (item->background() == QBrush(Qt::blue)) {
        item->setBackground(Qt::white);
    } else {
        item->setBackground(Qt::blue);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
