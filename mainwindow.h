#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "addpassword.h"
#include "loginpasswordsmanager.h"
#include "setpassword.h"


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
    setPassword *setPasswordWindow;
    void initialSetup(QTableWidget *widget);
    void readJson(QString filename);
    void fillTable(QTableWidget *widget);
    void unhide(QTableWidget *widget);
    void hide(QTableWidget *widget);
    void deletePassword(QTableWidget *widget);
    void editPassword(QTableWidget *widget);
    void saveJson(QString filename);
    QString hashPassword(QString toHash);
    void loginPasswordManagment();
    QString loginPassword;
    QString getFolder();
    void startApp(QString filename);
    int failedLoginCheck;

private slots:
    void on_unhidePassword_clicked();
    void on_hidePassword_clicked();
    void addPasswordWindow();
    void on_deletePassword_clicked();
    void on_startApp_clicked();
    void aboutMe();
    void on_editPassword_clicked();
    void deleteAll();
};
#endif // MAINWINDOW_H
