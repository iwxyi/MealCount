#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QObject>
#include <QDialog>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QList>
#include <QRegExp>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QClipboard>

class ShowWindow : public QDialog
{
    Q_OBJECT

struct Cls {
    int start_week, end_week;      // 第几周单位
    int day;                       // 周日~周六，0~6
    int start_course, end_course;  // 课程范围：第1~12节课
    bool dual;                     // 双周
    bool single;                   // 单周
    int member;                    // 人数

    Cls()
    {
        dual = single = false;
        start_week = end_week = 0; // 判断失败设置为0
    }

    QString toString()
    {
        return QString("第 %1 ~ %2 周，周 %3 ，第 %4 ~ %5 节，%6 人 %7 %8").arg(start_week)
                .arg(end_week).arg(day).arg(start_course).arg(end_course).arg(member)
                .arg(dual?"双周":"").arg(single?"单周":"");
    }
};

public:
    ShowWindow(QWidget* parent, QString c1, QString c2);

private:
    void initView();
    void analyze(QStringList times, QStringList numbs, QList<Cls> &clss);
    QList<Cls> clssFromString(QString time, QString numb);
    Cls clsFromString(QString time, QString numb);
    void refreshInfomation();

signals:

public slots:
    void slotWeekChanged(int x);
    void slotDayChanged(int x);
    void slotCopyAll();

private:
    QStringList times, numbs;
    QComboBox* week_combo;
    QListWidget* time_list;
    QTextEdit* numb_edit;
    QPushButton* copy_btn;

    QList<Cls> clss;
    int current_week;
    int current_day;
};

#endif // SHOWWINDOW_H
