#include "addpassword.h"
#include "ui_addpassword.h"

QStringList helperAddPassword;

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
    QString newSite = ui->siteText->toPlainText();
    qDebug() << newSite;
    QString newPassword = ui->passwordText->toPlainText();
    qDebug() << newPassword;

    helperAddPassword.clear();
    helperAddPassword.append(newSite);
    helperAddPassword.append(newPassword);
}
