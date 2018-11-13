/* FileName:report_dialog.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The implementation of report module to draw the report of DB data
 */
#include "report_dialog.h"
#include "ui_report_dialog.h"

report_Dialog::report_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::report_Dialog)
{
    ui->setupUi(this);
    setFlight_Combox();
}

report_Dialog::~report_Dialog()
{
    delete ui;
}


void report_Dialog::setFlight_Combox()
{
    // set flight combox
    QComboBox* com=ui->FLIGHT_comboBox;
    com->clear();
    QSqlQuery query;
    QString opName="FLIGHTinfo";
    QString showItem="FLIGHT";
    query.prepare("select "+showItem+" FROM "+opName+" group by "+showItem);
    bool isOk = query.exec();
    flightNum=0;
    if(!isOk)
    {
        qDebug()<<"error!";
        return;
    }
    while (query.next())
    {
        com->addItem(query.value(0).toString());
        flightNum+=1;
    }
    com->addItem("ALL");
    com->setCurrentIndex(-1);
}

void report_Dialog::on_FLIGHT_comboBox_currentTextChanged(const QString &arg1)
{
    //this->resize(440, 300);
}



void report_Dialog::fun()
{
    QSqlQuery query;
    QString opName="FLIGHTinfo";
    QString showItem="FLIGHT";
    query.prepare("select "+showItem+" FROM "+opName+" group by "+showItem);
    bool isOk = query.exec();
    flightNum=0;
    if(!isOk)
    {
        return;
    }
    while (query.next())
    {
        flightNum+=1;
    }


    QComboBox * com=ui->FLIGHT_comboBox;
    QString text=com->currentText();
    if(text=="ALL") // show all report
    {
        qDebug()<<"ALL";
//        return;
    }
    QDate now= QDate::currentDate();
    QDate M1month=now.addMonths(-1);
    QDate M2month=now.addMonths(-2);
    QVector<QBarSet*> BarSetV;
//    QBarSet *set0 = new QBarSet("TOTAL");
//    BarSetV.append(set0);
    //2
    for(int i=0;i<flightNum;i++)
    {

        QString Flight=com->itemText(i);
        QBarSet *set = new QBarSet(Flight);
        BarSetV.append(set);
        QSqlQuery query;
        query.prepare("select * from FLIGHTinfo where Flight= '"+Flight+"' and date_format(FDATE, '%Y %m') = date_format(DATE_SUB(curdate(), INTERVAL 2 MONTH),'%Y %m')");
        query.bindValue(":FLIGHT",Flight);
        bool isOK=query.exec();
        if(!isOK)
        {
            qDebug()<<Flight<<"错误！";
        }
        int flightSell=0;
        while (query.next())
        {
            int FID=query.value(0).toInt();
            QSqlQuery query1;
            query1.prepare("select *,count(*) from FSTATUSinfo where FID= :FID group by USABLE order by USABLE");
            query1.bindValue(":FID",FID);
            query1.exec();
            query1.next();
            int sellNum=query1.value(4).toInt();
            if(sellNum>150)
            {
                query1.next();
                sellNum=query1.value(4).toInt();
            }
            flightSell+=sellNum;
        }
        qDebug()<<Flight<<flightSell<<"上上个月";
        set->append(flightSell);
    }
    //1
    for(int i=0;i<flightNum;i++)
    {

        QString Flight=com->itemText(i);
        QBarSet *set =BarSetV[i];
        BarSetV.append(set);
        QSqlQuery query;
        query.prepare("select * from FLIGHTinfo where Flight= '"+Flight+"' and date_format(FDATE, '%Y %m') = date_format(DATE_SUB(curdate(), INTERVAL 1 MONTH),'%Y %m')");
        query.bindValue(":FLIGHT",Flight);
        bool isOK=query.exec();
        if(!isOK)
        {
            qDebug()<<Flight<<"错误！";
        }
        int flightSell=0;
        while (query.next())
        {
            int FID=query.value(0).toInt();
            QSqlQuery query1;
            query1.prepare("select *,count(*) from FSTATUSinfo where FID= :FID group by USABLE order by USABLE");
            query1.bindValue(":FID",FID);
            query1.exec();
            query1.next();
            int sellNum=query1.value(4).toInt();
            if(sellNum>150)
            {
                query1.next();
                sellNum=query1.value(4).toInt();
            }
            flightSell+=sellNum;
        }
        qDebug()<<Flight<<flightSell<<"上个月";
        set->append(flightSell);
    }
    //0
    for(int i=0;i<flightNum;i++)
    {

        QString Flight=com->itemText(i);
        QBarSet *set =BarSetV[i];
        BarSetV.append(set);
        QSqlQuery query;
        query.prepare("select * from FLIGHTinfo where Flight= '"+Flight+"' and date_format(FDATE, '%Y %m') = date_format(DATE_SUB(curdate(), INTERVAL 0 MONTH),'%Y %m')");
        query.bindValue(":FLIGHT",Flight);
        bool isOK=query.exec();
        if(!isOK)
        {
            qDebug()<<Flight<<"错误！";
        }
        int flightSell=0;
        while (query.next())
        {
            int FID=query.value(0).toInt();
            QSqlQuery query1;
            query1.prepare("select *,count(*) from FSTATUSinfo where FID= :FID group by USABLE order by USABLE");
            query1.bindValue(":FID",FID);
            query1.exec();
            query1.next();
            int sellNum=query1.value(4).toInt();
            if(sellNum>150)
            {
                query1.next();
                sellNum=query1.value(4).toInt();
            }
            flightSell+=sellNum;
        }
        qDebug()<<Flight<<flightSell<<"这个月";
        set->append(flightSell);
    }

    QBarSeries *barseries = new QBarSeries();
    for(int i=0;i<flightNum;i++)
    {
        barseries->append(BarSetV[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(barseries);
    //chart->addSeries(lineseries);
    chart->setTitle("Report");

    QStringList categories;
    categories << QString::number(M2month.month(),10) << QString::number(M1month.month(),10) << QString::number(now.month(),10);
    //categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    //chart->setAxisX(axisX, lineseries);
    chart->setAxisX(axisX, barseries);
    //axisX->setRange(QString("Jan"), QString("Jun"));

    QValueAxis *axisY = new QValueAxis();
    //chart->setAxisY(axisY, lineseries);
    chart->setAxisY(axisY, barseries);
    axisY->setRange(0, 20);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewport(chartView);
}

void report_Dialog::on_show_pushButton_clicked()
{
    fun();
}
