#include "addpassword.h"
#include "ui_addpassword.h"

QStringList helperAddPassword;
bool isAdded;

addPassword::addPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPassword)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelNewPassword()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(addNewPassword()));
}

addPassword::~addPassword()
{
    delete ui;
}


void addPassword::addNewPassword(){
    QString newSite = ui->siteText->text();
    qDebug() << newSite;
    QString newPassword = ui->passwordText->text();
    qDebug() << newPassword;

    helperAddPassword.clear();
    helperAddPassword.append(newSite);
    helperAddPassword.append(newPassword);

    isAdded = true;
    close();
}

void addPassword::cancelNewPassword(){
    isAdded = false;
    close();
}

void addPassword::siteSetter(QString site){
    ui->siteText->setText(site);
}

void addPassword::passwordSetter(QString password){
    ui->passwordText->setText(password);
}
