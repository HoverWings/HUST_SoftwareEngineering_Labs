/* FileName:login_dialog.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The definition of login module
 */
#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QVector>
#include <QMessageBox>
#include "mainwindow.h"
#include "string"

namespace Ui
{
    class login_dialog;
}

class login_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit login_dialog(QWidget *parent = 0);
    QSqlDatabase* db;
    ~login_dialog();

private slots:
    void on_login_pushButton_clicked();

    void on_password_lineEdit_editingFinished();

private:
    Ui::login_dialog *ui;

};

#endif // LOGIN_DIALOG_H
