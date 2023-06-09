#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "addpassword.h"
#include "loginpasswordsmanager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<QStringList> passwordArray;

private:
    Ui::MainWindow *ui;
    addPassword *newPasswordWindow;
    loginPasswordsManager *changeSetPassword;
    void initialSetup(QTableWidget *widget);
    void readJson(QString filename);
    void fillTable(QTableWidget *widget);
    void unhide(QTableWidget *widget);
    void hide(QTableWidget *widget);
    void deletePassword(QTableWidget *widget);
    void saveJson();
    void hashPassword(QString *toHash);
    void loginPasswordManagment();
    QString loginPassword;
    QString getFolder();

private slots:
    void on_unhidePassword_clicked();
    void on_hidePassword_clicked();
    void addPasswordWindow();
    void on_deletePassword_clicked();
};
#endif // MAINWINDOW_H
