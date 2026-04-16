#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QHeaderView"
#include <QAbstractItemView>
#include <QTableWidgetItem>

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
            ui->tableWidget->setItem(row, col, item);
        }
    }

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            ui->tableWidget->item(row, col)->setBackground(Qt::white);
        }
    }

    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
}


