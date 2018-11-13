/* FileName:main.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description: The main loop of the total Program and link the DB
 */
#include "mainwindow.h"
#include "login_dialog.h"
#include <QApplication>
#include <QSqlQuery>
#include <QMessageBox>
#include <QBuffer>
#include <QDir>
int main(int argc, char *argv[])
{
    //link DB
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("DBCourseDesign");
    db.setUserName("root");
    db.setPassword("990123");
    bool ok = db.open();
    if(ok)
    {
          qDebug()<<"成功连接数据库";
    }
    else
    {
          QMessageBox::warning(NULL,"警告","无法连接数据库");
    }
    //configure icon
    a.setWindowIcon(QIcon(":/main.ico"));

    QString currentPath;
    QDir dir;
    currentPath=dir.currentPath();
    qDebug()<<"path"<<currentPath;

    //prepare view data
    QSqlQuery query;
    query.prepare("select * from `FLIGHTinfo` as f where exists(select * from `FSTATUSinfo` as s where s.`FID`=f.`FID` and s.`USABLE`='1') and f.`FID`=3;");
    bool isOK=query.exec();
    while(query.next())
    {
        qDebug()<<query.value(0).toString();
    }
    login_dialog l;
    l.db=&db;
    l.show();

    return a.exec();
}
