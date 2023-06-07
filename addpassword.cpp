#include "addpassword.h"
#include "ui_addpassword.h"

addPassword::addPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPassword)
{
    ui->setupUi(this);
}

addPassword::~addPassword()
{
    delete ui;
}
