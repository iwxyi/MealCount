#include "showwindow.h"

ShowWindow::ShowWindow(QWidget *parent, QString c1, QString c2) : QDialog(parent)
{
    times = c1.split("\n");
    numbs = c2.split("\n");

    current_week = 1;
    current_day = 1;

    initView();
}

void ShowWindow::initView()
{
    this->setMinimumSize(100, 200);

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

    // 添加每星期
    time_list->addItem("周日");
    time_list->addItem("周一");
    time_list->addItem("周二");
    time_list->addItem("周三");
    time_list->addItem("周四");
    time_list->addItem("周五");
    time_list->addItem("周六");

    // 链接信号槽
    connect(week_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWeekChanged(int)));
    connect(time_list, SIGNAL(currentRowChanged(int)), this, SLOT(slotDayChanged(int)));

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
    clss.clear();
    for (int i = 0; i < times.size(); i++)
    {
        QList<Cls> temps = clssFromString(times.at(i), numbs.at(i));
        if (temps.size() > 0)
        {
            for (int j = 0; j < temps.size(); j++)
            {
                clss.append(temps.at(j));
            }
        }
    }
}

/**
 * 针对某一行，添加一个课程对象数组（因为每个课程可能有多个时间段）
 * @param time 时间字符串
 * @param numb 人数
 * @return     课程对象数组
 */
QList<ShowWindow::Cls> ShowWindow::clssFromString(QString time, QString numb)
{
    QList<Cls> clss;
    //if (time.indexOf(";") > -1) // 多个时间段
    //{
    QStringList times = time.split(";");
    for (int i = 0; i < times.size(); i++)
    {
        clss.append(clsFromString(times.at(i), numb));
    }
    //}
    //else ;
    return clss;
}

/**
 * 添加一个具体的课程对象
 * @param time 唯一时间
 * @param numb 唯一人数
 * @return     课程对象
 */
ShowWindow::Cls ShowWindow::clsFromString(QString time, QString numb)
{
    Cls cls;
    cls.member = numb.toInt();

    // 去掉“星期”
    time = time.replace("星期", "周");

    // 判断单双周
    if (time.indexOf("|双周") > -1)
    {
        cls.dual = true;
        time = time.replace("|双周", "");
    }
    if (time.indexOf("(双)") > -1)
    {
        cls.dual = true;
        time = time.replace("(双)", "");
    }
    if (time.indexOf("（双）") > -1)
    {
        cls.dual = true;
        time = time.replace("（双）", "");
    }
    if (time.indexOf("|单周") > 1)
    {
        cls.dual = true;
        time = time.replace("|单周", "");
    }
    if (time.indexOf("（单）") > 1)
    {
        cls.dual = true;
        time = time.replace("（单周）", "");
    }
    if (time.indexOf("(单)") > 1)
    {
        cls.dual = true;
        time = time.replace("(单)", "");
    }

    // 判断格式、使用正则表达式捕获
    QRegExp re("周(.)第?([\\d,-]+)节\\{第?(\\d+)-(\\d+)周\\}");
    if (!re.exactMatch(time))
    {
        qDebug() << "无法正则匹配时间：" << time;
        return cls;
    }
    QStringList ress = re.capturedTexts();
//    qDebug() << "capturedTexts:" << ress;

    // 判断第几周
    QString week = re.capturedTexts().at(1);
    if (week == "日")
        cls.day = 0;
    else if (week == "一")
        cls.day = 1;
    else if (week == "二")
        cls.day = 2;
    else if (week == "三")
        cls.day = 3;
    else if (week == "四")
        cls.day = 4;
    else if (week == "五")
        cls.day = 5;
    else if (week == "六")
        cls.day = 6;

    // 判断课程范围，可能是 1,2  也可能是 3-5
    QStringList courses = ress.at(2).split(QRegExp("[,-]"));
    cls.start_course = courses.first().toInt();
    cls.end_course = courses.last().toInt();

    // 判断上课时间
    cls.start_week = ress.at(3).toInt();
    cls.end_week = ress.at(4).toInt();

//    qDebug() << cls.toString();
    return cls;
}

/**
 * 刷新显示的时间
 */
void ShowWindow::refreshInfomation()
{
    int morning4 = 0;
    int morning5 = 0;
    int afternoon9 = 0;
    int night11 = 0;
    int night12 = 0;
//qDebug() << "clss.size = " << clss.size();
    // 遍历每一个课程
    for (int i = 0; i < clss.size(); i++)
    {
        Cls cls = clss.at(i);
//        qDebug() << "cls:" << cls.toString();
//qDebug() << "0";
        // 判断周的范围
        if (cls.start_week > current_week || cls.end_week < current_week)
            continue;
//qDebug() << "1";
        // 判断单双周
        if (cls.dual && current_week & 1)
            continue;
        if (cls.single && !(current_week & 1))
            continue;
//qDebug() << "2";
        // 判断周几
        if (cls.day != current_day)
            continue;
//qDebug() << "3";
        // 判断课程时间：上午4
        if (cls.end_course == 4)
            morning4 += cls.member;
        // 判断课程时间：上午5
        if (cls.end_course == 5)
            morning5 += cls.member;
        // 判断课程时间：下午9
        if (cls.end_course == 9)
            afternoon9 += cls.member;
        // 判断课程时间：晚上11
        if (cls.end_course == 11)
            night11 += cls.member;
        // 判断课程时间：晚上12
        if (cls.end_course == 12)
            night12 += cls.member;
    }

    numb_edit->clear();
    QString xingqi = "日";
    if (current_day == 1) xingqi = "一";
    else if (current_day == 2) xingqi = "二";
    else if (current_day == 3) xingqi = "三";
    else if (current_day == 4) xingqi = "四";
    else if (current_day == 5) xingqi = "五";
    else if (current_day == 6) xingqi = "六";
    numb_edit->append("第"+QString::number(current_week)+"周 星期"+xingqi+"：\n");
    numb_edit->append("上午第 4节：" + QString::number(morning4) + " 人\n");
    numb_edit->append("上午第 5节：" + QString::number(morning5) + " 人\n");
    numb_edit->append("下午第 9节：" + QString::number(afternoon9) + " 人\n");
    numb_edit->append("晚上第11节：" + QString::number(night11) + " 人\n");
    numb_edit->append("晚上第12节：" + QString::number(night12) + " 人");
}

/**
 * 学期的第几周被改变
 * @param x 第几周
 */
void ShowWindow::slotWeekChanged(int x)
{
    current_week = x+1;
    refreshInfomation();
}

/**
 * 一周的第几天被改变
 * @param x 周几
 */
void ShowWindow::slotDayChanged(int x)
{
    current_day = x;
    refreshInfomation();
}
