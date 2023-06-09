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
#include <QMessageBox>
#include <QCryptographicHash>
#include <QByteArray>
#include <QStandardPaths>
#include <QPushButton>
#include <QDialogButtonBox>

QList<QStringList> passwordArray;
QString loginPassword;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->addPassword, SIGNAL(clicked()), this, SLOT(addPasswordWindow()));

    QTableWidget *passwords = ui->passwordsShower;
    initialSetup(passwords);
    readJson(getFolder());

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
void MainWindow::readJson(QString filename){
    //QString filename = QFileDialog::getOpenFileName(this);
    //QFile jsonFile("C:\\Users\\tomek\\Desktop\\PW\\Projekt\\PasswordManager\\Passwords.json");
    //Open JSON
    QFile jsonFile(filename);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonFileData = jsonFile.readAll();
    jsonFile.close();

    if(jsonFileData.isEmpty()){
        qDebug() << "is Empty";
        setPasswordWindow = new setPassword(this);
        setPasswordWindow->exec();

        hashPassword(setPasswordWindow->passwordToHash);
    }

    //Make document into an object
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFileData);
    QJsonObject jsonObject = jsonDocument.object();

    //Get information into an array
    QJsonValue pairsArrayValue = jsonObject.value("pairsArray");
    QJsonArray pairsArray = pairsArrayValue.toArray();

    //Get all the information into the Global QList
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
    fillTable(ui->passwordsShower);
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
void MainWindow::deletePassword(QTableWidget *widget){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion", "Czy jesteś pewien",
                                  QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){
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

//hashing passwords
QString MainWindow::hashPassword(QString toHash){
    QByteArray makeHash;
    makeHash.append(toHash.toStdString().c_str());
    QString hashedSaltedPsw = QCryptographicHash::hash(makeHash, QCryptographicHash::Blake2s_224).toHex();
    qDebug() << "encrypted psw: " << hashedSaltedPsw;
    return hashedSaltedPsw;
}

void MainWindow::loginPasswordManagment(){
    //Open JSON
    QFile jsonFile("C:\\Users\\tomek\\Desktop\\PW\\Projekt\\PasswordManager\\Passwords.json");
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonFileData = jsonFile.readAll();
    jsonFile.close();

    //Make document into an object
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFileData);
    QJsonObject jsonObject = jsonDocument.object();

    //Get Login Value
    QJsonValue loginPasswordValue = jsonObject.value("loginPassword");

}

//Make or Enter folder stored in documents
QString MainWindow::getFolder(){
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if(dir.mkpath("PasswordManager")){
        dir.cd("PasswordManager");
    }else{
        dir.cd("PasswordManager");
    }
    qDebug() << dir;

    QString filePath;
    QFileInfo fi(dir, "PasswordManager.json");
    filePath = fi.absoluteFilePath();
    return filePath;
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

