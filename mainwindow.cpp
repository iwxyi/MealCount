#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isEqualColumn(QString c1, QString c2)
{
    QStringList list1 = c1.split("\n");
    QStringList list2 = c2.split("\n");
    qDebug() << "行数：" << list1.size() << "    " << list2.size();
    return list1.size() == list2.size();
}

void MainWindow::on_pushButton_clicked()
{
    QString time_str = ui->textEdit->toPlainText();
    QString numb_str = ui->textEdit_2->toPlainText();
    if (!isEqualColumn(time_str, numb_str))
    {
        QMessageBox::warning(this, "行数不一致", "上课时间数量：" + QString::number(time_str.split("\n").size()) + "\n上课人数数量：" +
                             QString::number(numb_str.split("\n").size()));
        return ;
    }


}
