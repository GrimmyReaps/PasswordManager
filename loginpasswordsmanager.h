#ifndef LOGINPASSWORDSMANAGER_H
#define LOGINPASSWORDSMANAGER_H

#include <QDialog>

namespace Ui {
class loginPasswordsManager;
}

class loginPasswordsManager : public QDialog
{
    Q_OBJECT

public:
    explicit loginPasswordsManager(QWidget *parent = nullptr);
    ~loginPasswordsManager();
     Ui::loginPasswordsManager *ui;

private:

};

#endif // LOGINPASSWORDSMANAGER_H
