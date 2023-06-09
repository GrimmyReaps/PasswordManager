#ifndef SETPASSWORD_H
#define SETPASSWORD_H

#include <QDialog>

namespace Ui {
class setPassword;
}

class setPassword : public QDialog
{
    Q_OBJECT

public:
    explicit setPassword(QWidget *parent = nullptr);
    ~setPassword();
    QString passwordToHash;

private:
    Ui::setPassword *ui;

private slots:
    void checkPassword();
};

#endif // SETPASSWORD_H
