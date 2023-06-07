#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initialSetup(QTableWidget *widget);
    void readJson();
    void fillTable(QTableWidget *widget);
    void unhide(QTableWidget *widget);
    void hide(QTableWidget *widget);

private slots:
    void on_unhidePassword_clicked();
    void on_hidePassword_clicked();
};
#endif // MAINWINDOW_H
