/* FileName:print_dialog.h
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:The declaration of print module
 */
#ifndef PRINT_DIALOG_H
#define PRINT_DIALOG_H

#include <QDialog>
#include <QString>
#include <QPrinter>
#include <QPixmap>
#include <QPainter>

namespace Ui {
class print_Dialog;
}

class print_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit print_Dialog(QVector<QString> pV,QWidget *parent = 0);
    QVector<QString> apV;
    ~print_Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::print_Dialog *ui;
};

#endif // PRINT_DIALOG_H
