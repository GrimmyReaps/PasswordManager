#ifndef ADDPASSWORD_H
#define ADDPASSWORD_H

#include <QDialog>

namespace Ui {
class addPassword;
}

class addPassword : public QDialog
{
    Q_OBJECT

public:
    explicit addPassword(QWidget *parent = nullptr);
    ~addPassword();

private:
    Ui::addPassword *ui;
};

#endif // ADDPASSWORD_H