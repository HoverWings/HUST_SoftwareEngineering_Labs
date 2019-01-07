/* FileName:addflight_dialog.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:
 * The implementation of addflight module
 */
#include "addflight_dialog.h"
#include "ui_addflight_dialog.h"

addFlight_Dialog::addFlight_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addFlight_Dialog)
{
    ui->setupUi(this);

    // set time edit
    ui->dateEdit->setDisplayFormat("yyyy/MM/dd");
    ui->dateEdit->setCalendarPopup(true);
    ui->timeEdit->setDisplayFormat("HH:mm");

    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->timeEdit->setDateTime(QDateTime::currentDateTime());

    ui->dateEdit_2->setDisplayFormat("yyyy/MM/dd");
    ui->dateEdit_2->setCalendarPopup(true);
    ui->timeEdit_2->setDisplayFormat("HH:mm");

    ui->dateEdit_2->setDateTime(QDateTime::currentDateTime());
    ui->timeEdit_2->setDateTime(QDateTime::currentDateTime());

    isDateChanged=false;
    isTimeChanged=false;

    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(dateChanged()));
    connect(ui->timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(timeChanged()));
    setFMODEL_Combox();

    //set FSTATE_combox
    ui->FSTATE_comboBox->addItem("SCHEDULED");
    ui->FSTATE_comboBox->addItem("CANCELLED");

    //set FID
    ui->FID_lineEdit->setValidator(new QIntValidator(0, 10000, this));

    //set qss
    QFile qssfile(":/test.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    setStyleSheet(qss);

}


void addFlight_Dialog::dateChanged()
{
    isDateChanged=true;
}

void addFlight_Dialog::timeChanged()
{
    isTimeChanged=true;
}


void addFlight_Dialog::setFMODEL_Combox()
{
    // set flight combox
    QComboBox* com=ui->FMODEL_comboBox;
    com->clear();
    QSqlQuery query;
    QString opName="FMODELinfo";
    QString showItem="FMODEL";
    query.prepare("select "+showItem+" FROM "+opName+" group by "+showItem);
    bool isOk = query.exec();
    if(!isOk)
    {
        return;
    }
    while (query.next())
    {
        com->addItem(query.value(0).toString());
    }
    //com->addItem("ALL");
    com->setCurrentIndex(-1);
}

addFlight_Dialog::~addFlight_Dialog()
{
    delete ui;
}


void addFlight_Dialog::on_post_Button_clicked()
{
    if(!isDateChanged || !isTimeChanged)
    {
         QMessageBox::information(nullptr, "Attention!", "请设置时间！", QMessageBox::Yes);
         return;
    }

    QString FDATE=ui->dateEdit->text();
    QString FTIME=ui->timeEdit->text();
    QString ARRDATE=ui->dateEdit_2->text();
    QString ARRTIME=ui->timeEdit_2->text();
    QVector<QString> vec;
    QString FID=ui->FID_lineEdit->text();
    QString Flight=ui->Flight_lineEdit->text();
    QString FMODEL=ui->FMODEL_comboBox->currentText();
    QString FSTATE=ui->FSTATE_comboBox->currentText();
    QString FFROM=ui->FFROM_lineEdit->text();
    QString FTO=ui->FTO_lineEdit->text();
    QString FSTATUS="NOT FULL";
    vec.append(FID);
    vec.append(Flight);
    vec.append(FMODEL);
    vec.append(FSTATE);
    vec.append(FFROM);
    vec.append(FTO);
    QVectorIterator<QString>  i(vec);
    while (i.hasNext())
    {
        if(i.next().isEmpty())
        {
            QMessageBox::information(nullptr, "Please Fit the form", "请填写完整表单！", QMessageBox::Yes);
            return;
        }
    }
    QSqlQuery query;
    QString str="";
    bool isOK;
    str+="insert into FLIGHTinfo values(:FID,:Flight,:FMODEL,:FSTATE,:FFROM,:FTO,:FDATE,:FTIME,:ARRDATE,:ARRTIME,:FSTATUS)";
    query.prepare(str);
    query.bindValue(":FID",FID);
    query.bindValue(":Flight",Flight);
    query.bindValue(":FMODEL",FMODEL);
    query.bindValue(":FSTATE",FSTATE);
    query.bindValue(":FFROM",FFROM);
    query.bindValue(":FTO",FTO);
    query.bindValue(":FDATE",FDATE);
    query.bindValue(":FTIME",FTIME);
    query.bindValue(":ARRDATE",ARRDATE);
    query.bindValue(":ARRTIME",ARRTIME);
    query.bindValue(":FSTATUS",FSTATUS);
    isOK=query.exec();
    if(!isOK)
    {
        QMessageBox::information(nullptr, "information", "插入失败！数据错误！", QMessageBox::Yes);
        qDebug()<<"插入失败!";
        return;
    }
    QMessageBox::information(nullptr, "information", "插入成功！", QMessageBox::Yes);
    //process FSTATUS info
    query.clear();
    str="select * from FMODELinfo where FMODEL = :FMODEL";
    query.prepare(str);
    query.bindValue(":FMODEL",FMODEL);
    isOK=query.exec();
    QString F;
    QString C;
    QString Y;
    if(query.next())
    {
        qDebug()<<query.value(0).toString();
        F=query.value(2).toString();
        qDebug()<<query.value(2).toString();
        C=query.value(3).toString();
        qDebug()<<query.value(3).toString();
        Y=query.value(4).toString();
        qDebug()<<query.value(4).toString();
    }
    int Frow=F.split("*")[0].toInt();
    int Fcol=F.split("*")[1].toInt();
    int Crow=C.split("*")[0].toInt();
    int Ccol=C.split("*")[1].toInt();
    int Yrow=Y.split("*")[0].toInt();
    int Ycol=Y.split("*")[1].toInt();


    QString SID;
    query.clear();
    str="insert into FSTATUSinfo values(:FID,:SID,:USABLE,:SeatRank)";
    query.prepare(str);
    query.bindValue(":FID",FID);
    query.bindValue(":USABLE","1");
    int begin;
    int end;
    begin=1;
    end=Frow+1;
    query.bindValue(":SeatRank","F");
    for(int i=begin;i<end;i++)
    {
        for(int j=0;j<Fcol;j++)
        {
            SID=QString::number(i,10)+QString('A'+j);
            query.bindValue(":SID",SID);
            isOK=query.exec();
            if(!isOK)
            {
                qDebug()<<SID<<"插入失败";
            }
        }
    }
    begin=Frow+1;
    end=Frow+Crow+1;
    query.bindValue(":SeatRank","C");
    for(int i=begin;i<end;i++)
    {
        for(int j=0;j<Ccol;j++)
        {
            SID=QString::number(i,10)+QString('A'+j);
            query.bindValue(":SID",SID);
            isOK=query.exec();
            if(!isOK)
            {
                qDebug()<<SID<<"插入失败";
            }
        }
    }
    begin=Frow+Crow+1;
    end=Frow+Crow+Yrow+1;
    query.bindValue(":SeatRank","Y");
    for(int i=begin;i<end;i++)
    {
        for(int j=0;j<Ycol;j++)
        {
            SID=QString::number(i,10)+QString('A'+j);
            query.bindValue(":SID",SID);
            isOK=query.exec();
            if(!isOK)
            {
                qDebug()<<SID<<"插入失败";
            }
        }
    }
    this->close();
}

