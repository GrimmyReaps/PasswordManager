#include "loginpasswordsmanager.h"
#include "ui_loginpasswordsmanager.h"

loginPasswordsManager::loginPasswordsManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginPasswordsManager)
{
    ui->setupUi(this);
}

loginPasswordsManager::~loginPasswordsManager()
{
    delete ui;
}
