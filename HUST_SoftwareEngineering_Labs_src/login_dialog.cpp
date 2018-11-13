/* FileName:login_dialog.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The implementation of login module
 */
#include "login_dialog.h"
#include "ui_login_dialog.h"

login_dialog::login_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_dialog)
{
    ui->setupUi(this);
    ui->usrname_lineEdit->setText("hover");
    ui->password_lineEdit->setText("123456");
    ui->password_lineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->login_pushButton->setFocus();
    // set qss
    QFile qssfile(":/test.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    setStyleSheet(qss);
}

login_dialog::~login_dialog()
{
    delete ui;
}

void login_dialog::on_login_pushButton_clicked()
{
    QString username=ui->usrname_lineEdit->text();
    QString password=ui->password_lineEdit->text();
    QSqlQuery query;
    query.exec("select DBCourseDesign;");
    query.prepare("select * from user where `username`=:username and password=:password;");
    query.bindValue(0, username);
    query.bindValue(1, password);
    query.exec();
    bool isRoot=false;
    if (query.next())
    {
        qDebug() << query.value(3).toString();
        if(query.value(3).toInt()==1)
        {
            isRoot=true;
        }
    }
    else
    {
        QMessageBox::warning(NULL,"警告","用户名或者密码错误！");
        return;
    }
    MainWindow* w=new MainWindow(NULL,isRoot);
    // set user information
    w->UID=query.value(0).toInt();
    w->userName=query.value(1).toString();
    w->isRoot=isRoot;
    w->db=this->db;
    this->close();
    w->show();
}

void login_dialog::on_password_lineEdit_editingFinished()
{
    ui->login_pushButton->setFocus();
}
