/* FileName:print_dialog.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The implementation of print module and call the system API to print the Ticket to PDF file simulatedly
 */
#include "print_dialog.h"
#include "ui_print_dialog.h"

print_Dialog::print_Dialog(QVector<QString> pV,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::print_Dialog)
{

    ui->setupUi(this);
    apV=pV;
    ui->user_label->setText(pV[0]);
    ui->SID_label->setText(pV[1]);
    ui->SeatRank_label->setText(pV[2]);
    ui->FID_label->setText(pV[3]);
    ui->Flight_label->setText(pV[4]);
    ui->FMODEL_label->setText(pV[5]);
    ui->FSTATE_label->setText(pV[6]);
    ui->FFROM_label->setText(pV[7]);
    ui->FTO_label->setText(pV[8]);
    ui->FDATE_label->setText(pV[9]);
    ui->FTIME_label->setText(pV[10]);
    ui->ARRDATE_label->setText(pV[11]);
    ui->ARRTIME_label->setText(pV[12]);
    ui->TIAKCTIME_label->setText(pV[13]);
}

print_Dialog::~print_Dialog()
{
    delete ui;
}

void print_Dialog::on_pushButton_clicked()
{
    ui->pushButton->hide();
    QPrinter printer_pixmap(QPrinter::HighResolution);
    printer_pixmap.setPageSize(QPrinter::A4);                                  //设置纸张大小为A4
    printer_pixmap.setOutputFormat(QPrinter::PdfFormat);                       //设置输出格式为pdf
    printer_pixmap.setOutputFileName("./tickets/"+apV[0]+"_"+apV[3]+".pdf");   //设置输出路径
    QPixmap pixmap = QPixmap::grabWidget(this, this->rect());                  //获取界面的图片

    QPainter painter_pixmap;
    painter_pixmap.begin(&printer_pixmap);
    QRect rect = painter_pixmap.viewport();
    int multiple = rect.width()/pixmap.width();
    painter_pixmap.scale(multiple, multiple);                                 //将图像(所有要画的东西)在pdf上放大multiple-1倍
    painter_pixmap.drawPixmap(0, 0, pixmap);                                  //画图
    painter_pixmap.end();
    this->close();
}
