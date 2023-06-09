#ifndef CHECKLOGINCREDENTIALS_H
#define CHECKLOGINCREDENTIALS_H

#include <QDialog>

namespace Ui {
class checkLoginCredentials;
}

class checkLoginCredentials : public QDialog
{
    Q_OBJECT

public:
    explicit checkLoginCredentials(QWidget *parent = nullptr);
    ~checkLoginCredentials();
    QString checkIfCorrect;
    QString Info;

private:
    Ui::checkLoginCredentials *ui;

private slots:
    void checkPassword();
};

#endif // CHECKLOGINCREDENTIALS_H
