#include "showwindow.h"

ShowWindow::ShowWindow(QWidget *parent, QString c1, QString c2) : QDialog(parent)
{
    times = c1.split("\n");
    numbs = c2.split("\n");
}

void ShowWindow::initView()
{
    QHBoxLayout * main_layout = new QHBoxLayout;
    QVBoxLayout* vlayout = new QVBoxLayout;
    time_list = new QListWidget(this);
    numb_edit = new QTextEdit(this);
    vlayout->addWidget(week_combo);
    vlayout->addWidget(time_list);
    main_layout->addLayout(vlayout);
    main_layout->addWidget(numb_edit);
    this->setLayout(main_layout);

    // 开始统计
    analyze();
}

void ShowWindow::analyze()
{

}
