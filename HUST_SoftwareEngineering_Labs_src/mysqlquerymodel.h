/* FileName:mysqlquerymodel.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:This class inherits from the basical QSqlQueryModel to make the module can edit and autofit the UI
 * Main Funciton:
 * 1.   Judge the role to control the edit Authority
 * 2.   Auto fit the process table and get the tabel title from the DB
 * 3.   Implement the title to the DB dictionary mapping, which is choosable
 * 4.   Set the table properity automaticly
 */
#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H
#include "mainwindow.h"
#include <QSqlQueryModel>
#include <QVector>
#include <QMap>

/*
 * op tabel
 * 1:user
 * 2:FLIGHTinfo
 * 3:query info
 */


class MySqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:

    //public var
    int opView{};
    int  opTable{};
    QString opName;
    QString opPRI;
    int opPRI_col{};
    QVector<QString> opTitle;   //table title
    QVector<QString> addVec;    //the vec
    class MainWindow* mw{};
    QMap<QString,QString> map;

    // public function
    MySqlQueryModel();
    explicit MySqlQueryModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
    bool addData();
    bool deleteData(QModelIndex &index);
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool set_op();
    bool set_opTitle();
    bool set_opName();
    bool set_opIndex(QModelIndex &index);
    void refresh();

private:
    //private var
    int opRow{};
    int opCol{};

    //private funciton

};

#endif // MYSQLQUERYMODEL_H
