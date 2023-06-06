#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTableWidgetItem>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    //Place for Array that will hold all info

    QTableWidget *passwords = ui->passwordsShower;
    initialSetup(passwords);
    readJson();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Table initialization function
void MainWindow::initialSetup(QTableWidget *widget){
    widget->horizontalHeader()->setStretchLastSection(true);
    widget->insertColumn(0);
    widget->insertColumn(1);
    QStringList passwordlabel = {"Strony", "Hasła"};
    widget->setHorizontalHeaderLabels(passwordlabel);
    //widget->insertRow(widget->rowCount());
    //int currentRow = widget->rowCount() - 1;

    //widget->setItem(currentRow, 0, new QTableWidgetItem("wordpress"));
}

//reads JSON file
//TODO add JSON info to that fucking 2D array please
void MainWindow::readJson(){
    QFile jsonFile("C:\\Users\\tomek\\Desktop\\PW\\Projekt\\PasswordManager\\Passwords.json");
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonFileData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFileData);
    QJsonObject jsonObject = jsonDocument.object();

    QJsonValue pairsArrayValue = jsonObject.value("pairsArray");
    QJsonArray pairsArray = pairsArrayValue.toArray();

    foreach(const QJsonValue & v, pairsArray){
        qDebug() << v.toObject().value("site").toString();
        qDebug() << v.toObject().value("password").toString();

        qDebug() << "---------------------------";
    }
}
