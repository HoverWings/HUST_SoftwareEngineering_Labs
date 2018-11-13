/* FileName:chooseseat_dialog.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The definition of chooseseat module
 */
#ifndef CHOOSESEAT_DIALOG_H
#define CHOOSESEAT_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QList>
#include <QString>
#include "mysqlquerymodel.h"
#include <QButtonGroup>
#include <QMessageBox>
#include <QGraphicsView>

namespace Ui
{
    class chooseSeat_Dialog;
}

class chooseSeat_Dialog : public QDialog
{
    Q_OBJECT

public:
    int FID;
    void queryFSTATUS(int FID);
    class MainWindow* mw;
    explicit chooseSeat_Dialog(class MainWindow *parent = 0,int FID=-1);
    ~chooseSeat_Dialog();
    QString seatName;

private:
    QButtonGroup *buttonGroup;
    QMap<QString, QPushButton*> pushButtonMap;
    Ui::chooseSeat_Dialog *ui;
    void setButtonGroup(int FID);
    void setImage();
    void initButton();
    void buttonJudge(int buttonId);
    class MySqlQueryModel* StatusModel=NULL;

private slots:
    void on_chooseSteat_pushButton_clicked();
};

#endif // CHOOSESEAT_DIALOG_H
