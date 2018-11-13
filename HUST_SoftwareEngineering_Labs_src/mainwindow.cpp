/* FileName:login_dialog.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The implementation of mainwindow slot,function and the interface with other module
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,bool isRoot) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set model pointer
    MySqlQueryModel* myModel = new MySqlQueryModel(this);
    this->myModel=myModel;
    myModel->mw=this;

    // set table vie selection
    ui->tableView_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    // set
    ui->tableView_0->horizontalHeader()->setStretchLastSection(true);        //最后一列补全所有空白位置
    ui->tableView_0->verticalHeader()->hide();
    ui->tableView_1->horizontalHeader()->setStretchLastSection(true);        //最后一列补全所有空白位置
    ui->tableView_1->verticalHeader()->hide();
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);        //最后一列补全所有空白位置
    ui->tableView_2->verticalHeader()->hide();
    //ui->tableView_2->horizontalHeader()->set                               //最后一列补全所有空白位置
    // set default datetime
    ui->dateEdit->setDisplayFormat("yyyy/MM/dd");
    ui->dateEdit->setCalendarPopup(true);
    ui->timeEdit->setDisplayFormat("HH:mm");
    //this->setStyleSheet("background-color:black;");

    // set qss
//    QFile qssfile(":/test.qss");
//    qssfile.open(QFile::ReadOnly);
//    QString qss;
//    qss = qssfile.readAll();
//    setStyleSheet(qss);

    //set root
    this->isRoot=isRoot;
    if(!isRoot)
    {
        ui->addFlightButton->hide();
        ui->deleteFlightButton->hide();
    }

    //set map

//    QWebEnginePage *page=new QWebEnginePage(this);
//    QWebChannel *channel=new QWebChannel(this);
//    page->load(QUrl("FILE:./baidumap/demo/1_0.html"));
//    page->setWebChannel(channel);
//    ui->webEngineView->load(page);
//    ui->webEngineView->show();

//    QWebEngineView *view=new QWebEngineView(this);
//    view->load(QUrl("FILE:./baidumap/demo/1_0.html"));
//    setCentralWidget(view);
//    resize(1024,680);
//    ui->map_widget->setUrl(QUrl("https://map.baidu.com/"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_F_triggered() // print FLIGHTinfor 1 to view
{
    // show flight info
    myModel->opTable=0;
    myModel->set_op();
    myModel->setQuery("select * from "+myModel->opName);
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        myModel->setHeaderData(i, Qt::Horizontal, myModel->opTitle[i]);
    }
    pOpView->setModel(myModel);
    return;
}

//void MainWindow::on_deleteButton_clicked()
//{
//    qDebug()<<"delete!!!!";
//    //myModel->set_op();
//    //myModel->set_opIndex(index);
//    QModelIndex selIndex=pOpView->currentIndex();
//    selRow=selIndex.row();
//    selCol=selIndex.column();
//    myModel->deleteData(selIndex);
//}

//void MainWindow::on_addButton_clicked()
//{
//    myModel->set_op();
//    QSqlQuery query;
//    query.prepare("insert into "+myModel->opName+" values()");
//    bool isOk = query.exec();
//    if(!isOk)
//    {
//        return;
//    }
//    myModel->refresh();
//}

//Query by the condition
void MainWindow::on_queryButton_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        transQuery();
    }
    QString str;
    str="select * from "+myModel->opName+" as f where ";
    if(ui->FFROM_lineEdit->text().isEmpty())
    {
         QMessageBox::about(NULL, "Attention!", "请输入起始地！");
         return;
    }
    else
    {
        // the fitst condition and don't need 'and'
        qDebug()<<"起始地"<<ui->FFROM_lineEdit->text();
        str+="FFROM =";
        str+=" '";
        str+=ui->FFROM_lineEdit->text();
        str+="' ";
    }
    if(ui->FTO_lineEdit->text().isEmpty())
    {
         QMessageBox::about(NULL, "Attention!", "请输入到达地！");
         return;

    }
    else
    {
        qDebug()<<"到达地"<<ui->FTO_lineEdit->text();
        str+=" and ";
        str+="FTO =";
        str+=" '";
        str+=ui->FTO_lineEdit->text();
        str+="' ";
    }
    if(isDataChanged)
    {
        QDate date = ui->dateEdit->date();
        QString dateStr=date.toString("yyyy-MM-dd");
        qDebug()<<dateStr;
        str+=" and ";
        str+="FDATE =";
        str+=" '";
        str+=dateStr;
        str+="' ";
    }
    if(isTimeChanged) // the flight time should before the time
    {
        QTime time = ui->timeEdit->time();
        QString timeStr=time.toString("HH:mm:ss");
        qDebug()<<timeStr;
        str+=" and ";
        str+="FTIME <=";
        str+=" '";
        str+=timeStr;
        str+="' ";
    }
    if(ui->FLIGHT_comboBox->currentText()!="ALL")
    {
        QString Flight=ui->FLIGHT_comboBox->currentText();
        qDebug()<<"FLIGHT:"<<Flight;
        str+=" and ";
        str+="Flight =";
        str+=" '";
        str+=Flight;
        str+="' ";
    }
    if(ui->order_comboBox->currentText()=="最近")
    {
        //str+=" and ";
        str+="order by ";
        //str+=" '";
        str+="FDATE";
        //str+="' ";
    }
    //query.prepare("select * from `FLIGHTinfo` as f where exists(select * from `FSTATUSinfo` as s where s.`FID`=f.`FID` and s.`USABLE`='1') and f.`FID`=3;");
    //str+=" and ";
    //str+=" exists( select * from `FSTATUSinfo` where `FSTATUSinfo`.`FID`=`FLIGHTinfo`.`FID`)";
//and FSTATUSinfo.USABLE=1
    myModel->setQuery(str);
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        myModel->setHeaderData(i, Qt::Horizontal, myModel->opTitle[i]);
    }
    pOpView->setModel(myModel);
    return;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    myModel->clear();
    switch(index)
    {
        case 0:
        {
            myModel->opTable=0; //FLIGHTinfo
            myModel->opView=0;  //add and delete
            pOpView=ui->tableView_0;
            setTab();
            break;
        }
        case 1:
        {
            myModel->opTable=0;
            myModel->opView=1;
            myModel->set_op();
            pOpView=ui->tableView_1;
            setFlight_Combox();
            break;
        }
        case 2:
        {
            //order table
            myModel->opTable=1;
            myModel->opView=2;
            myModel->set_op();
            pOpView=ui->tableView_2;
            setTab2();
            break;
        }
        case 3:
        {
            //bill table
            QSqlQuery query;
            QString str="";
            QString viewName=userName+"_ORDER";
            qDebug()<<viewName;
            qDebug()<<UID;
            str="create or replace VIEW "+ viewName +" AS select OID, FLIGHTinfo.FID , SID , FFROM, FTO, FDATE-DAY(1) as 'pick up time', FDATE, FTIME , OSTATE FROM FLIGHTinfo , ORDERinfo where FLIGHTinfo.FID=ORDERinfo.FID and OSTATE!='Unsubscribe' AND UID = "+QString::number(UID,10);
            query.prepare(str);
            bool isOK=query.exec();
            if(!isOK)
            {
                qDebug()<<"create user-order fail!";
                return;
            }
            myModel->opName=viewName;
            myModel->set_opTitle();
            pOpView=ui->tableView_3;
            setTab();
            break;
        }
    }
}

void MainWindow::transQuery()
{
    QSqlQuery query;

}



void MainWindow::setTab()
{
    //set title
//    QPalette palette;
//    palette.setBrush(pOpView->backgroundRole(), Qt::black);
//    pOpView->setPalette(palette);
    myModel->clear();
    myModel->setQuery("select * from "+myModel->opName);
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        myModel->setHeaderData(i, Qt::Horizontal, myModel->opTitle[i]);
    }
    pOpView->setModel(myModel);
}


void MainWindow::setTab2()
{
    myModel->clear();
    myModel->setQuery("select * from "+myModel->opName+" where UID ="+QString::number(UID,10));
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        myModel->setHeaderData(i, Qt::Horizontal, myModel->opTitle[i]);
    }
    pOpView->setModel(myModel);
}


void MainWindow::dateChanged()
{
    isDataChanged=true;
}

void MainWindow::timeChanged()
{
    isTimeChanged=true;
}

void MainWindow::setFlight_Combox()
{
    // set time edit
    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->timeEdit->setDateTime(QDateTime::currentDateTime());
    isDataChanged=false;
    isTimeChanged=false;
    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(dateChanged()));
    connect(ui->timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(timeChanged()));

    // set flight combox
    QComboBox* com=ui->FLIGHT_comboBox;
    com->clear();
    QSqlQuery query;
    QString opName="FLIGHTinfo";
    QString showItem="FLIGHT";
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
    com->addItem("ALL");
    com->setCurrentIndex(-1);
}



void MainWindow::on_postButton_clicked()
{
    QString opName="ORDERinfo";
    seatName="";
//    if (res == QDialog::Accepted)
//    {
//       QMessageBox::information(this, "INFORMATION", "You clicked OK button!");
//    }
//    if (res == QDialog::Rejected)
//    {
//       QMessageBox::information(this, "INFORMATION", "You clicked CANCEL button!");

//    }
    qDebug()<<seatName;
    qDebug()<<myModel->opView;
    int row= pOpView->currentIndex().row();
    if(row==-1)
    {
         QMessageBox::information(NULL, "订单错误", "请先查询", QMessageBox::Yes, QMessageBox::Yes);
         return;
    }
    QAbstractItemModel *model =pOpView->model();
    QVector<QString> chsVecs;
    //int row=0;
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        QModelIndex index= model->index(row,i);//选中行第一列的内容
        QVariant data = model->data(index);
        qDebug()<<data.toString();
        chsVecs.append(data.toString());
    }
    int FID=chsVecs[0].toInt();
    int UID=this->UID;

    // process choose seat;
    chooseSeat_Dialog* chooseSeatD=new chooseSeat_Dialog(this,FID);
    chooseSeatD->exec(); // to get the seat id

    // close the window without choose the seat
    if(seatName=="")
    {
        QMessageBox::information(NULL, "订单错误！", "未选择座位", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QString OTIME =QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<OTIME;
    QString OSTATE="unfinish";
    QSqlQuery query;
    query.prepare("insert into "+opName+" (FID, UID,SID, OTIME, OSTATE) VALUES(:FID,:UID,:SID,:OTIME,:OSTATE)");
    query.bindValue(":FID", FID);
    query.bindValue(":UID", UID);
    query.bindValue(":SID", seatName);
    query.bindValue(":OTIME", OTIME);
    query.bindValue(":OSTATE", OSTATE);
    bool isOK1 = query.exec();
    // decrease seat
    query.clear();
    opName="FSTATUSinfo";
    query.prepare("update "+opName+" set USABLE = 0 where SID = :SID ");
    query.bindValue(":SID",seatName);
    bool isOk2 = query.exec();
    if(isOK1&&isOk2)
    {
        setTab();
        qDebug()<<"预订成功";
    }
    else
    {
        // roll backs
        printSQLError();
        qDebug()<<"预订失败";
        return;
    }
}

// debug function
void MainWindow::printSQLError()
{

    QSqlDatabase* db=(this->db);
    QSqlError error;
    error=db->lastError();
    qDebug()<<error;
    if(error.isValid())//发生错误时isValid()返回true
    {
        switch (error.type())
        {
            case QSqlError::NoError:
                qDebug()<<"无错误";
                break;
            case QSqlError::ConnectionError://连接错语
                qDebug()<<error.text();
                break;
            case QSqlError::StatementError://语句错语
                qDebug()<<error.text();
                break;
            case QSqlError::TransactionError://事务错误
                qDebug()<<error.text();
                break;
            default://未知错误
                qDebug()<<error.text();
                break;
        }
    }
}


void MainWindow::on_Unsubscribe_pushButton_clicked()
{
    QString orderstr="";
    int row= pOpView->currentIndex().row();
    QAbstractItemModel *model =pOpView->model ();
    QVector<QString> chsVecs;
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        QModelIndex index= model->index(row,i);//选中行第一列的内容
        QVariant data = model->data(index);
        qDebug()<<data.toString();
        chsVecs.append(data.toString());
    }
    int OID=chsVecs[0].toInt();
    QString OSTATE=chsVecs.last();
    if(OSTATE=="Unsubscribe")
    {
        //QMessageBox::StandardButton rb = QMessageBox::information(NULL, "退订错误！", "订单状态错误", QMessageBox::Yes, QMessageBox::Yes);
        QMessageBox::information(NULL, "退订错误！", "订单状态错误", QMessageBox::Yes, QMessageBox::Yes);
    }
    //qDebug()<<chsVecs;
    //QMessageBox::StandardButton rb = QMessageBox::question(NULL, "订单退订！", "你确定要退订订单:"+QString::number(OID,10)+"从"+FFROM+"飞往"+FTO+" 于 "+FTIME, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "订单退订！", "你确定要退订订单:"+QString::number(OID,10), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        //process ORDERinfo
        QSqlQuery query;
        //qDebug()<<"opName:"<<myModel->opName;
        query.prepare("update "+myModel->opName+" set OSTATE = :OSTATE where OID = :OID ");
        query.bindValue(":OSTATE","Unsubscribe");
        query.bindValue(":OID",OID);
        bool isOk1 = query.exec();

        // process the FSTATUS
        query.clear();
        QString opName="FSTATUSinfo";
        query.prepare("update "+opName+" set USABLE = 1 where SID = (select SID from ORDERinfo where OID= :OID) ");
        query.bindValue(":OID",OID);
        bool isOk2 = query.exec();
        if(isOk1&&isOk2)
        {
            QMessageBox::about(NULL, "Attention", "退订成功");
            setTab();
            qDebug()<<"退订成功";
            return;
        }
        else
        {
            // roll back
            QMessageBox::about(NULL, "Attention", "退订失败");
            qDebug()<<"删除失败";
            return;
        }
    }
}



void MainWindow::on_addFlightButton_clicked()
{
    addFlight_Dialog* addflightdialog=new addFlight_Dialog(NULL);
    addflightdialog->exec();

}


void MainWindow::on_deleteFlightButton_clicked()
{
    int row= pOpView->currentIndex().row();
    QAbstractItemModel *model =pOpView->model();
    QVector<QString> chsVecs;
    //int row=0;
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        QModelIndex index= model->index(row,i);//选中行第一列的内容
        QVariant data = model->data(index);
        qDebug()<<data.toString();
        chsVecs.append(data.toString());
    }
    int FID=chsVecs[0].toInt();
    int UID=this->UID;
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "航班删除！", "你确定要删除航班:"+QString::number(FID,10), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("delete from FLIGHTinfo where FID=:FID");
        query.bindValue(":FID",FID);
        bool isOK=query.exec();
        if(isOK)
        {
            QMessageBox::information(this, "INFORMATION", "删除成功!");
            query.clear();
            QString str="delete from FSTATUSinfo where FID = :FID";
            query.prepare(str);
            query.bindValue(":FID",FID);
            isOK=query.exec();
            qDebug()<<"删除座位"<<isOK;
            setTab();
        }
        else
        {
            QMessageBox::information(this, "INFORMATION", "删除失败!存在顾客订单，无法删除");
        }
    }
    else
    {
        return;
    }

}


void MainWindow::on_checkin_pushButton_clicked()
{
    QString orderstr="";

    int row= pOpView->currentIndex().row();
    QAbstractItemModel *model =pOpView->model ();
    QVector<QString> chsVecs;
    for(int i=0;i<myModel->opTitle.size();i++)
    {
        QModelIndex index= model->index(row,i);//选中行第一列的内容
        QVariant data = model->data(index);
        qDebug()<<data.toString();
        chsVecs.append(data.toString());
    }
    int OID=chsVecs[0].toInt();
    int FID=chsVecs[1].toInt();

    QDateTime now= QDateTime::currentDateTime();
    QDateTime before=now.addDays(-1);
    //bill table
    QSqlQuery query;
    query.prepare("select FDATE, FTIME from FLIGHTinfo where FID =:FID");
    query.bindValue(":FID",FID);
    query.exec();
    QDateTime FDT;
    if(query.next())
    {
        QDate FDATE=query.value(0).toDate();
        QTime FTIME=query.value(1).toTime();
        //FDT=QDateTime(FDATE)+QDate(FTIME.QTime);
        FDT=QDateTime(FDATE);
    }
    if(!(FDT>before&&FDT<now))
    {
        QMessageBox::information(NULL, "进站错误！", "时间错误无法进站！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QString OSTATE=chsVecs.last();
    if(OSTATE=="Unsubscribe" or OSTATE=="check-in" )
    {
        //QMessageBox::StandardButton rb = QMessageBox::information(NULL, "退订错误！", "订单状态错误", QMessageBox::Yes, QMessageBox::Yes);
        QMessageBox::information(NULL, "进站错误！", "进站状态错误", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //qDebug()<<chsVecs;
    //QMessageBox::StandardButton rb = QMessageBox::question(NULL, "订单退订！", "你确定要退订订单:"+QString::number(OID,10)+"从"+FFROM+"飞往"+FTO+" 于 "+FTIME, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "进站！", "你进站:"+QString::number(OID,10), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        //process ORDERinfo
        QSqlQuery query;
        //qDebug()<<"opName:"<<myModel->opName;
        query.prepare("update "+myModel->opName+" set OSTATE = :OSTATE where OID = :OID ");
        query.bindValue(":OSTATE","check-in");
        query.bindValue(":OID",OID);
        bool isOk1 = query.exec();
        // process the FSTATUS
        if(!isOk1)
        {
            // roll back
            QMessageBox::about(NULL, "Attention", "进站失败");
            qDebug()<<"进站失败";
            return;
        }

        // print pdf
        bool isOK;
        QString SID;
        QString SeatRank;
        QVector<QString> pV;
        pV.append(userName);
        //SID
        query.clear();
        query.prepare("select * from ORDERinfo where OID =:OID ");
        query.bindValue(":OID",OID);
        isOK=query.exec();
        if(!isOK)
        {
            qDebug()<<"SID查询错误！";
        }
        if(query.next())
        {
            SID=query.value(3).toString();
            pV.append(SID);
        }



        //seat rank
        query.clear();
        query.prepare("select SeatRank from FSTATUSinfo where SID = :SID and FID=:FID ");
        query.bindValue(":SID",SID);
        query.bindValue(":FID",FID);
        isOK=query.exec();
        if(!isOK)
        {
            qDebug()<<"seat rank查询错误！";
        }
        if(query.next())
        {
            SeatRank=query.value(0).toString();
            pV.append(SeatRank);
        }

        //Flightinfo
        query.clear();
        query.prepare("select * from FLIGHTinfo where FID =:FID ");
        query.bindValue(":FID",FID);
        query.exec();
        if(query.next())
        {
            for(int i=0;i<10;i++)
            {
                pV.append(query.value(i).toString());
            }
        }

        // TICKET time
        pV.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        print_Dialog* printdialog=new print_Dialog(pV,this);
        //set printdialog
        printdialog->exec();


        QMessageBox::about(NULL, "Attention", "进站成功");
        setTab();
        qDebug()<<"进站成功";
        return;

    }
}

void MainWindow::on_pushButton_clicked()
{
    report_Dialog* reportdialog=new report_Dialog(this);
    reportdialog->exec();
}
