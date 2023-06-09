#include "setpassword.h"
#include "ui_setpassword.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QPalette>

setPassword::setPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setPassword)
{
    ui->setupUi(this);

    connect(ui->Ok, SIGNAL(clicked()), this, SLOT(checkPassword()));
    //Zmiana koloru textu na czerwony
    QPalette palette = ui->information->palette();
    palette.setColor(ui->information->foregroundRole(), Qt::red);
    ui->information->setPalette(palette);
}

setPassword::~setPassword()
{
    delete ui;
}


void setPassword::checkPassword(){
    QRegularExpression expression("(?=.*[A-Za-z])(?=.*\\d)(?=.*[@$!%*#?&])[A-Za-z\\d@$!%*#?&]{8,}");
    QRegularExpressionMatch check = expression.match(ui->setPasswordText->text());

    if(check.hasMatch()){
        if(ui->setPasswordText->text() == ui->repeatPasswordText->text()){
            QString passwordToHash = ui->setPasswordText->text();
            close();
        }else{
            ui->information->setText("Hasła się nie zgadzają");
        }
    }else{
        ui->information->setText("Hasło wymaga minimum jednej dużej litery, małej litery, cyfry i znaku specjalnego");
    }
}
