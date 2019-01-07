/* FileName:addflight_dialog.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:
 * The definition of addflight module
 */

#ifndef ADDFLIGHT_DIALOG_H
#define ADDFLIGHT_DIALOG_H
#include <QDialog>
#include <QString>
#include <QSqlQuery>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QVector>
#include <QMessageBox>
#include <QDebug>
#include <QChart>

namespace Ui
{
    class addFlight_Dialog;
}

class addFlight_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit addFlight_Dialog(QWidget *parent = 0);
    ~addFlight_Dialog();

private:
    Ui::addFlight_Dialog *ui;
    bool isDateChanged=false;
    bool isTimeChanged=false;
    void setFMODEL_Combox();


private slots:
    void dateChanged();
    void timeChanged();
    void on_post_Button_clicked();
};

#endif // ADDFLIGHT_DIALOG_H
