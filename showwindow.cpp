#include "showwindow.h"

ShowWindow::ShowWindow(QWidget *parent, QString c1, QString c2) : QDialog(parent)
{
    Monday_first = true;
    current_week = 1;
    current_day = 1;

    initView();

    // 开始统计分离的课程
    analyze(c1.split("\n"), c2.split("\n"), clss);
}

void ShowWindow::initView()
{
    this->setMinimumSize(100, 200);

    QHBoxLayout * main_layout = new QHBoxLayout;
    QVBoxLayout* vlayout = new QVBoxLayout;
    week_combo = new QComboBox(this);
    time_list = new QListWidget(this);
    numb_edit = new QTextEdit(this);
    copy_btn = new QPushButton("复制所有", this);
    vlayout->addWidget(week_combo);
    vlayout->addWidget(time_list);
    main_layout->addLayout(vlayout);
    main_layout->addWidget(numb_edit);
    main_layout->addWidget(copy_btn);
    this->setLayout(main_layout);

    // 添加周数
    QStringList sl;
    for (int i = 1; i < 30; i++)
    {
        sl << QString::number(i);
    }
    week_combo->addItems(sl);

    // 添加每星期
    for (int i = (Monday_first ? 1 : 0); i < 7+(Monday_first ? 1 : 0); i++)
    {
        time_list->addItem("周"+weekDayTrans(i));
    }

    // 链接信号槽
    connect(week_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWeekChanged(int)));
    connect(time_list, SIGNAL(currentRowChanged(int)), this, SLOT(slotDayChanged(int)));
    connect(copy_btn, SIGNAL(clicked()), this, SLOT(slotCopyAll()));
}

/**
 * 获取某一天的情况
 * @param week 周数
 * @param day  周几
 * @return     一天餐点的人数
 */
ShowWindow::DayNums ShowWindow::getDayNums(int week, int day)
{
    DayNums dn;

    // 遍历每一个课程
    for (int i = 0; i < clss.size(); i++)
    {
        Cls cls = clss.at(i);

        // 判断周的范围
        if (cls.start_week > week || cls.end_week < week)
            continue;

        // 判断单双周
        if (cls.dual && week & 1)
            continue;
        if (cls.single && !(week & 1))
            continue;

        // 判断周几
        if (cls.day != day)
            continue;

        // 判断课程时间：上午4
        if (cls.end_course == 4)
            dn.m4 += cls.member;
        // 判断课程时间：上午5
        if (cls.end_course == 5)
            dn.m5 += cls.member;
        // 判断课程时间：下午9
        if (cls.end_course == 9)
            dn.a9 += cls.member;
        // 判断课程时间：晚上11
        if (cls.end_course == 11)
            dn.n11 += cls.member;
        // 判断课程时间：晚上12
        if (cls.end_course == 12)
            dn.n12 += cls.member;
    }

    return dn;
}

/**
 * 星期几整型转换为字符串型
 * @param x 星期几
 * @return  字符串的周几
 */
QString ShowWindow::weekDayTrans(int x)
{
    switch (x)
    {
    case 0 :
        return "日";
    case 1 :
        return "一";
    case 2 :
        return "二";
    case 3 :
        return "三";
    case 4 :
        return "四";
    case 5 :
        return "五";
    case 6 :
        return "六";
    case 7 :
        return "日";
    }
    return "";
}

int ShowWindow::weekDayTrans(QString s)
{
    if (!Monday_first && s == "日")
        return 0;
    else if (s == "一")
        return 1;
    else if (s == "二")
        return 2;
    else if (s == "三")
        return 3;
    else if (s == "四")
        return 4;
    else if (s == "五")
        return 5;
    else if (s == "六")
        return 6;
    else if (s == "日")
        return 7;
    return -1;
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
    // 遍历数组中的每一行
    for (int i = 0; i < times.size(); i++)
    {
        QList<Cls> ress = clssFromString(times.at(i), numbs.at(i));
        if (ress.size() > 0)
        {
            for (int j = 0; j < ress.size(); j++)
            {
                clss.append(ress.at(j));
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
    // 遍历数组中每一行的每个时间段（分号分隔）
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

    // 判断格式、使用正则表达式捕获对应位置
    QRegExp re("周(.)第?([\\d,-]+)节\\{第?(\\d+)-(\\d+)周\\}");
    if (!re.exactMatch(time))
    {
        // 特判：周二第3-4节{17周}
        re.setPattern("周(.)第?([\\d,-]+)节\\{第?(\\d+)周\\}");
        if (!re.exactMatch(time))
        {
            qDebug() << "无法正则匹配时间：" << time;
            return cls;
        }
    }
    QStringList ress = re.capturedTexts();
//    qDebug() << "capturedTexts:" << ress; // 调试输出匹配结果

    // 判断第几周
    QString week = re.capturedTexts().at(1);
    cls.day = weekDayTrans(week);

    // 判断课程范围，可能是 1,2  也可能是 3-5
    QStringList courses = ress.at(2).split(QRegExp("[,-]"));
    cls.start_course = courses.first().toInt();
    cls.end_course = courses.last().toInt();

    // 判断上课时间
    cls.start_week = ress.at(3).toInt();
    if (ress.size() > 4)
        cls.end_week = ress.at(4).toInt();
    else
        cls.end_week = cls.start_week;

//    qDebug() << cls.toString();
    return cls;
}

/**
 * 刷新显示的时间
 */
void ShowWindow::refreshInfomation()
{
    DayNums dn = getDayNums(current_week, current_day);

    numb_edit->clear();
    QString xingqi = weekDayTrans(current_day);
    numb_edit->append("第"+QString::number(current_week)+"周 星期"+xingqi+"：\n");
    numb_edit->append("上午第 4节：" + QString::number(dn.m4) + " 人\n");
    numb_edit->append("上午第 5节：" + QString::number(dn.m5) + " 人\n");
    numb_edit->append("下午第 9节：" + QString::number(dn.a9) + " 人\n");
    numb_edit->append("晚上第11节：" + QString::number(dn.n11) + " 人\n");
    numb_edit->append("晚上第12节：" + QString::number(dn.n12) + " 人");
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
    current_day = x + (Monday_first ? 1 : 0);
    refreshInfomation();
}

/**
 * 复制所有的内容到表格
 */
void ShowWindow::slotCopyAll()
{
    QString ex_str = "周数\t星期\t上午4\t上午5\t下午9\t晚上11\t晚上12\n";
    for (int week = 1; week < 30; week++)
    {
        int none_day = 0;
        for (int day = (Monday_first ? 1 : 0); day < 7 + (Monday_first ? 1 : 0); day++)
        {
            DayNums dn = getDayNums(week, day);
            if (!dn.m4 && !dn.m5 && !dn.a9 && !dn.n11 && !dn.n12)
                none_day++;
            ex_str += "第" + QString::number(week) + "周\t星期" + weekDayTrans(day) + "\t" + dn.toString() + "\n";
        }
        // 判断有没有超过期末（直到一周完全没课）
        if (week > 18 && none_day == 7)
            break;
    }
    QClipboard *board = QApplication::clipboard();
    board->setText(ex_str);
    QMessageBox::information(this, "操作完成", "已复制到剪贴板，请打开 Excel 粘贴");
}
