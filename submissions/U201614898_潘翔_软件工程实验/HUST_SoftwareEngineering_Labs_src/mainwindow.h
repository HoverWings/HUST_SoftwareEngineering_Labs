/* FileName:mainwindow.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The definition of mainwindow slot,function and the interface with other module
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QUrl>
#include <QComboBox>
#include <QWebChannel>
#include <QPrinter>
#include <QPalette>
#include <QTableWidget>
#include "mysqlquerymodel.h"
#include "chooseseat_dialog.h"
#include "addflight_dialog.h"
#include "print_dialog.h"
#include "report_dialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int UID{};
    QString userName;
    bool isRoot=false;
    QString seatName;

    QSqlDatabase* db{};
    explicit MainWindow(QWidget *parent = 0,bool isRoot = false);
    class MySqlQueryModel* myModel=NULL;
    QTableView * pOpView=NULL;
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int selRow{};
    int selCol{};
    bool isDataChanged=false;
    bool isTimeChanged=false;
    void setFlight_Combox();
    void setTab();
    void setTab2();
    void transQuery();


    //debug funciton
    void printSQLError();



public slots:
    void on_action_F_triggered();

private slots:
    //void on_deleteButton_clicked();
    //void on_addButton_clicked();
    void on_queryButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_postButton_clicked();
    void on_Unsubscribe_pushButton_clicked();


    void dateChanged();
    void timeChanged();


    void on_addFlightButton_clicked();
    void on_deleteFlightButton_clicked();
    void on_checkin_pushButton_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
