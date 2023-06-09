#include "checklogincredentials.h"
#include "ui_checklogincredentials.h"
#include <QCryptographicHash>

checkLoginCredentials::checkLoginCredentials(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkLoginCredentials)
{
    ui->setupUi(this);
    connect(ui->Confirm, SIGNAL(clicked()), this, SLOT(checkPassword()));

    QPalette palette = ui->Information->palette();
    palette.setColor(ui->Information->foregroundRole(), Qt::red);
    ui->Information->setPalette(palette);
    ui->Information->text() = Info;

}

checkLoginCredentials::~checkLoginCredentials()
{
    delete ui;
}


void checkLoginCredentials::checkPassword(){
    QString checkIfCorrect = ui->enterdPassword->text();
    close();
}
