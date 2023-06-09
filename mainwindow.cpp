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
#include <QList>
#include <QStringList>
#include <QFileDialog>

QList<QStringList> passwordArray;
QString loginPassword;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->addPassword, SIGNAL(clicked()), this, SLOT(addPasswordWindow()));

    loginPassword = "0000";

    QTableWidget *passwords = ui->passwordsShower;
    initialSetup(passwords);
    readJson();
    fillTable(passwords);

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
}

//reads JSON file and adds information to Global QList
void MainWindow::readJson(){
    //QString filename = QFileDialog::getOpenFileName(this);

    //QFile jsonFile(filename);
    QFile jsonFile("C:\\Users\\tomek\\Desktop\\PW\\Projekt\\PasswordManager\\Passwords.json");
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonFileData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFileData);
    QJsonObject jsonObject = jsonDocument.object();

    QJsonValue pairsArrayValue = jsonObject.value("pairsArray");
    QJsonArray pairsArray = pairsArrayValue.toArray();

    foreach(const QJsonValue & v, pairsArray){
        QStringList helper;
        helper.append(v.toObject().value("site").toString());
        helper.append(v.toObject().value("password").toString());

        qDebug() << v.toObject().value("site").toString();
        qDebug() << v.toObject().value("password").toString();
        qDebug() << "---------------------------";

        passwordArray.append(helper);
        helper.clear();
    }

    qDebug() << passwordArray;
}

//Fill our beautiful TableWidget
void MainWindow::fillTable(QTableWidget *widget){
    widget->setRowCount(0);
    for(int i = 0; i < passwordArray.length(); i++){
        widget->insertRow(widget->rowCount());
        widget->setItem(i, 0, new QTableWidgetItem(passwordArray[i][0]));
        widget->setItem(i, 1, new QTableWidgetItem("hidden"));
    }
}

//Shows password
void MainWindow::unhide(QTableWidget *widget){
    QTableWidgetItem *siteChosen = widget->item(widget->currentRow(), 0);
    qDebug() << siteChosen->text();

    for(int i = 0; i < passwordArray.length(); i++){
        if(QString::compare(siteChosen->text(), passwordArray[i][0]) == 0){
            widget->setItem(widget->currentRow(), 1, new QTableWidgetItem(passwordArray[i][1]));
        }
    }

}

//Hides password
void MainWindow::hide(QTableWidget *widget){
    widget->setItem(widget->currentRow(), 1, new QTableWidgetItem("hidden"));
}

//new window for password creation
void MainWindow::addPasswordWindow(){
    newPasswordWindow = new addPassword(this);
    newPasswordWindow->exec();

    if(newPasswordWindow->isAdded){
        passwordArray.append(newPasswordWindow->helperAddPassword);
        fillTable(ui->passwordsShower);
        saveJson();
    }
}

//Deltes password from everywhere
//ToDo confirmation box
void MainWindow::deletePassword(QTableWidget *widget){
    QTableWidgetItem *siteChosen = widget->item(widget->currentRow(), 0);
    qDebug() << siteChosen->text();

    for(int i = 0; i < passwordArray.length(); i++){
        if(QString::compare(siteChosen->text(), passwordArray[i][0]) == 0){
            passwordArray.removeAt(i);
        }
    }
    fillTable(widget);
    saveJson();
}

void MainWindow::saveJson(){
    QJsonObject passwordDetails;
    QJsonArray passwordDetailsArray;
    QJsonObject credentials;
    //Json object for password hash
    credentials["password"] = loginPassword;

    //Creating array for passwords storage
    for(int i = 0; i < passwordArray.length(); i++){
        passwordDetails["password"] = passwordArray[i][1];
        passwordDetails["site"] = passwordArray[i][0];

        passwordDetailsArray.push_back(passwordDetails);
    }

    //Bundling the objects up
    QJsonObject obj;
    obj["loginPassword"] = loginPassword;
    obj["pairsArray"] = passwordDetailsArray;

    //Creating document
    QJsonDocument JsonDocument;
    JsonDocument.setObject(obj);

    //Writing it to JSON :D
    QFile jsonFile("C:\\Users\\tomek\\Desktop\\PW\\Projekt\\PasswordManager\\Passwords.json");
    jsonFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    jsonFile.write(JsonDocument.toJson(QJsonDocument::Indented));
    jsonFile.close();
}

//Button Click
void MainWindow::on_unhidePassword_clicked()
{
    unhide(ui->passwordsShower);
}

//Button Click
void MainWindow::on_hidePassword_clicked()
{
    hide(ui->passwordsShower);
}


//Button Click
void MainWindow::on_deletePassword_clicked()
{
    deletePassword(ui->passwordsShower);
}

