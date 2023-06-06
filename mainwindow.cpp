#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QTableWidget *passwords = ui->passwordsShower;
    passwords->horizontalHeader()->setStretchLastSection(true);
    passwords->insertColumn(0);
    passwords->insertColumn(1);
    QStringList passwordlabel = {"Strony", "Hasła"};
    passwords->setHorizontalHeaderLabels(passwordlabel);
    passwords->insertRow(passwords->rowCount());
    int currentRow = passwords->rowCount() - 1;

    passwords->setItem(currentRow, 0, new QTableWidgetItem("wordpress"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

