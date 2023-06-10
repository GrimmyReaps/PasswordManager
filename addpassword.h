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
    QStringList helperAddPassword;
    bool isAdded;
    void passwordSetter(QString password);
    void siteSetter(QString site);

private:
    Ui::addPassword *ui;

private slots:
    void addNewPassword();
    void cancelNewPassword();
};

#endif // ADDPASSWORD_H
