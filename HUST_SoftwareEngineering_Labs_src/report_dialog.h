/* FileName:report_dialog.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The declaration of report module to draw the report of DB data
 */
#ifndef REPORT_DIALOG_H
#define REPORT_DIALOG_H

#include <QDialog>
#include <QString>
#include <QSql>
#include <QSqlQuery>
#include <QComboBox>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QtWidgets>
#include <QtCharts>
#include <QWidget>
#include <QGraphicsView>


namespace Ui
{
    class report_Dialog;
}

class report_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit report_Dialog(QWidget *parent = 0);
    ~report_Dialog();

private slots:
    void on_FLIGHT_comboBox_currentTextChanged(const QString &arg1);

    void on_show_pushButton_clicked();

private:
    Ui::report_Dialog *ui;
    int flightNum;
    void setFlight_Combox();
    void fun();
};

#endif // REPORT_DIALOG_H
