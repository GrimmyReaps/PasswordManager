#include "addpassword.h"
#include "ui_addpassword.h"
#include "mainwindow.h"

addPassword::addPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPassword)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(addNewPassword()));
}

addPassword::~addPassword()
{
    delete ui;
}


void addPassword::addNewPassword(){

}
