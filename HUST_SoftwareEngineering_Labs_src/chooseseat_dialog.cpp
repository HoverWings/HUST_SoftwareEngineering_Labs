/* FileName:chooseseat_dialog.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The implementation of chooseseat module
 */
#include "chooseseat_dialog.h"
#include "ui_chooseseat_dialog.h"

chooseSeat_Dialog::chooseSeat_Dialog(class MainWindow *parent,int FID) :
    QDialog(parent),
    ui(new Ui::chooseSeat_Dialog)
{
    ui->setupUi(this);
    this->FID=FID;
    qDebug()<<this->FID;
    setWindowTitle(tr("请选择座位！"));
    setStyleSheet("background-color:white;"
                  "QPushButton{"
                  "background-color:white;"
                  "color:black;"
                  "text-align:center;"
                  "border-radius: 8px;"
                  "border: 2px groove gray;"
                  "border-style: outset;"
                  "}");
    setButtonGroup(FID);
    setImage();
    queryFSTATUS(FID);
    mw=parent;

//    //set qss
//    QFile qssfile(":/test.qss");
//    qssfile.open(QFile::ReadOnly);
//    QString qss;
//    qss = qssfile.readAll();
//    this->setStyleSheet(qss);

    //setAttribute(Qt::WA_DeleteOnClose);
    // set focus

}

//RESIZE!
void chooseSeat_Dialog::setImage()
{
    QString str = "select * from FMODELinfo where FMODEL= (select FMODEL FROM FLIGHTinfo where FID= "+QString::number(FID,10)+" )";
    QSqlQuery query;
    query.exec(str);
    QPixmap photo;
    if(query.next())
    {
        QLabel *PicLabel = new QLabel();
        photo.loadFromData(query.value(1).toByteArray(), "JPG"); //从数据库中读出图片为二进制数据，图片格式为JPG，然后显示到QLabel里
        //photo.scaledToHeight(ui->graphicsView->maximumHeight());
        //photo.scaledToWidth(ui->graphicsView->maximumWidth());
        photo.scaled(ui->graphicsView->maximumSize());
        PicLabel->setPixmap(photo);
        PicLabel->setScaledContents(true);
    }

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(photo);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(photo.width()*0.5 + 10, photo.height()*0.5 + 10);
    ui->graphicsView->show();
}


void chooseSeat_Dialog::setButtonGroup(int FID)
{
    // set button group
    int totalNum=0;
    int usableNum=0;
    QGridLayout *layout = ui->gridLayout_0;
    QSqlQuery query;
    QString opName="FLIGHTinfo";
    QString showItem="FLIGHT";
    QString str="select * from FSTATUSinfo where FID ="+QString::number(FID, 10);
    query.prepare(str);
    bool isOk = query.exec();
    if(!isOk)
    {
        qDebug()<<"set seat button fail!";
        return;
    }
    ;
    QPushButton* pushbutton=NULL;
    int rowBefore=1;
    int row=1;
    int col=-1;
    int len=0;
    int usable=-1;
    while (query.next()) // when .next() then go to the next
    {
        totalNum+=1;
        //qDebug()<<query.value(2).toString();
        usable=query.value(2).toString().toInt();
        len=query.value(1).toString().length();
        //qDebug()<<query.value(1).toString();
        row=query.value(1).toString().mid(0,len-1).toInt();
        if(row!=rowBefore)
        {
            col=0;
            rowBefore=row;
        }
        else
        {
            col+=1;
        }
        pushbutton = new QPushButton();
        pushbutton->setText(query.value(1).toString());
        pushButtonMap[query.value(1).toString()]=pushbutton;
        layout->addWidget(pushbutton,row,col,1,1);
        if(usable)
        {
            usableNum+=1;
            pushbutton->setCheckable(true);
            pushbutton->setAutoExclusive(true);
        }
        else
        {
            pushbutton->setCheckable(false);
            pushbutton->setStyleSheet("background-color: rgb(170, 0, 255);"); //set the background color
        }
    }
    qDebug()<<"totalNum"<<totalNum;
    qDebug()<<"usableNum"<<usableNum;
    ui->label_totalSeatNum->setText(QString::number(totalNum,10));
    ui->label_usableSeatNum->setText(QString::number(usableNum,10));
    float fnum=((float)totalNum-usableNum)/totalNum*100;
    ui->label_fullRate->setText(QString("%1").arg(fnum)+"%");
}


chooseSeat_Dialog::~chooseSeat_Dialog()
{
    delete ui;
}

//query the choosable seat
void chooseSeat_Dialog::queryFSTATUS(int FID)
{
    StatusModel=new MySqlQueryModel(this);
    StatusModel->opTable=2;
    StatusModel->set_op();
    QString str="select * from "+StatusModel->opName+" where FID ="+QString::number(FID, 10)+" AND USABLE = 1";
    //qDebug()<<str;
    StatusModel->setQuery(str);
    StatusModel->query().bindValue(":FID",FID);
    StatusModel->query().exec();
    if(StatusModel->query().next())
    {
        qDebug()<<StatusModel->query().value(0).toString();
    }
    else
    {
        qDebug()<<"No Search Result!";
    }
    for(int i=0;i<StatusModel->opTitle.size();i++)
    {
        StatusModel->setHeaderData(i, Qt::Horizontal, StatusModel->opTitle[i]);
    }
    ui->FSTATUS_tableView->setModel(StatusModel);
    ui->FSTATUS_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void chooseSeat_Dialog::on_chooseSteat_pushButton_clicked()
{

    QMapIterator<QString,QPushButton*>  i(pushButtonMap);
    seatName="";
    while (i.hasNext())
    {
//        qDebug() <<i.next().key()<<i.next().value();
        if(i.next().value()->isChecked()==true)
        {
            seatName=i.key();
            break;
        }
    }
    if(seatName=="")
    {
        QModelIndex selIndex=ui->FSTATUS_tableView->currentIndex();
        int selRow=selIndex.row();
        //int selCol=selIndex.column();
        qDebug()<<selRow;
        QModelIndex index= StatusModel->index(selRow,1);
        QVariant data = StatusModel->data(index);
        qDebug()<<data.toString();
        seatName=data.toString();
    }
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "请确认座位", "您选择的座位为:"+seatName, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        mw->seatName=seatName;
        this->close();
    }
    if(rb == QMessageBox::No)
    {
        return;
    }
    //rb = information(NULL, "Show Qt", "Do you want to show Qt dialog?", QMessageBox::Yes QMessageBox::No, QMessageBox::Yes);
}
