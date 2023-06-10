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
int failedLoginCheck;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    failedLoginCheck = 0;
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->addPassword, SIGNAL(clicked()), this, SLOT(addPasswordWindow()));
    connect(ui->aboutCreator, SIGNAL(clicked()), this, SLOT(aboutMe()));
    connect(ui->deleteAll, SIGNAL(clicked()), this, SLOT(deleteAll()));
    QPalette palette = ui->loginInformation->palette();
    palette.setColor(ui->loginInformation->foregroundRole(), Qt::red);
    ui->loginInformation->setPalette(palette);

    QTableWidget *passwords = ui->passwordsShower;
    initialSetup(passwords);

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

void MainWindow::startApp(QString filename){
    QFile jsonFile(filename);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonFileData = jsonFile.readAll();
    jsonFile.close();
    QJsonDocument importantInfoDoc = QJsonDocument::fromJson(jsonFileData);
    QJsonObject importantInfoObj = importantInfoDoc.object();
    QJsonValue importantInfoVal = importantInfoObj.value("loginPassword");

    loginPassword = importantInfoVal.toString();

    if(loginPassword.isEmpty()){

        qDebug() << "is Empty";
        setPasswordWindow = new setPassword(this);
        setPasswordWindow->exec();

        loginPassword = hashPassword(setPasswordWindow->getter());

        setPasswordWindow->close();

        saveJson(getFolder());
        readJson(getFolder());
        ui->addPassword->setEnabled(true);
        ui->hidePassword->setEnabled(true);
        ui->unhidePassword->setEnabled(true);
        ui->deletePassword->setEnabled(true);
        ui->deleteAll->setEnabled(true);
        ui->editPassword->setEnabled(true);
        ui->startApp->setEnabled(false);
    }else{
        if(ui->loginPasswordText->text().isEmpty()){
            return;
        }else if(hashPassword(ui->loginPasswordText->text()) == loginPassword){
            ui->addPassword->setEnabled(true);
            ui->hidePassword->setEnabled(true);
            ui->unhidePassword->setEnabled(true);
            ui->deletePassword->setEnabled(true);
            ui->deleteAll->setEnabled(true);
            ui->editPassword->setEnabled(true);
            ui->startApp->setEnabled(false);
            readJson(getFolder());
        }else{
            failedLoginCheck++;
            ui->loginInformation->setText("Pozostało " + QString::number(3-failedLoginCheck) + " prób");
            if(failedLoginCheck == 3)
                ui->startApp->setEnabled(false);
        }
    }
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
    QJsonDocument importantInfoDoc = QJsonDocument::fromJson(jsonFileData);
    QJsonObject importantInfoObj = importantInfoDoc.object();
    QJsonValue importantInfoVal = importantInfoObj.value("loginPassword");

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
        saveJson(getFolder());
    }
}


void MainWindow::editPassword(QTableWidget *widget){
    QTableWidgetItem *siteChosen = widget->item(widget->currentRow(), 0);
    qDebug() << siteChosen->text();
    QString searcher = siteChosen->text();
    QString password;
    for(int i = 0; i < passwordArray.length(); i++){
        if(QString::compare(searcher, passwordArray[i][0]) == 0){
            password = passwordArray[i][1];
            break;
        }
    }

    newPasswordWindow = new addPassword(this);
    newPasswordWindow->siteSetter(searcher);
    newPasswordWindow->passwordSetter(password);
    newPasswordWindow->exec();

    if(newPasswordWindow->isAdded){
        for(int i = 0; i < passwordArray.length(); i++){
            if(QString::compare(siteChosen->text(), passwordArray[i][0]) == 0){
                passwordArray.removeAt(i);
            }
        }
        passwordArray.append(newPasswordWindow->helperAddPassword);
        fillTable(ui->passwordsShower);
        saveJson(getFolder());
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
        saveJson(getFolder());
    }
}

void MainWindow::deleteAll(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion", "Czy jesteś pewien",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        ui->passwordsShower->setRowCount(0);
        passwordArray.clear();
        fillTable(ui->passwordsShower);
        saveJson(getFolder());
    }
}

void MainWindow::saveJson(QString filename){
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
    QFile jsonFile(filename);
    jsonFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    jsonFile.write(JsonDocument.toJson(QJsonDocument::Indented));
    jsonFile.close();
}

//hashing passwords
QString MainWindow::hashPassword(QString toHash){
    QByteArray makeHash;
    QString salt = "0xcbf50fed";
    makeHash.append(toHash.toStdString().c_str());
    makeHash.append(salt.toStdString().c_str());
    QString hashedSaltedPsw = QCryptographicHash::hash(makeHash, QCryptographicHash::Blake2s_224).toHex();
    qDebug() << "encrypted psw: " << hashedSaltedPsw;
    return hashedSaltedPsw;
}

void MainWindow::loginPasswordManagment(){
    //Open JSON
    QFile jsonFile(getFolder());
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

void MainWindow::aboutMe(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "O mnie", "Przygotował\n\rTomasz Niedziela ISI 1",
                                  QMessageBox::Yes|QMessageBox::Ok);
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


void MainWindow::on_startApp_clicked()
{
    startApp(getFolder());
}


void MainWindow::on_editPassword_clicked()
{
    editPassword(ui->passwordsShower);
}

