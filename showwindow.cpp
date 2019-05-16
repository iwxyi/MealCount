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
    week_combo = new QComboBox(this);
    time_list = new QListWidget(this);
    numb_edit = new QTextEdit(this);
    vlayout->addWidget(week_combo);
    vlayout->addWidget(time_list);
    main_layout->addLayout(vlayout);
    main_layout->addWidget(numb_edit);
    this->setLayout(main_layout);

    // 添加周数
    QStringList sl;
    for (int i = 1; i < 30; i++)
    {
        sl << QString::number(i);
    }
    week_combo->addItems(sl);

    // 链接信号槽
    connect(week_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWeekChanged(int)));

    // 开始统计分离的课程
    analyze(times, numbs, clss);
}

/**
 * 根据所有的内容进行解析，拆分成多个数组
 * @param times 课程时间数组：周一第1,2节{第1-8周};周四第6,7节{第2-8周|双周};周四第8节{第2-8周|双周}
 * @param numbs 上课人数数组，为一堆整数
 * @param clss  进行统计的课程数量，如果有多个时间段就算作多个课程（因为部分具体那个名字）
 */
void ShowWindow::analyze(QStringList times, QStringList numbs, QList<Cls> &clss)
{

}

void ShowWindow::slotWeekChanged(int x)
{

}
