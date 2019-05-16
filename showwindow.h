#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QList>
#include <QRegExp>
#include <QDebug>

class ShowWindow : public QDialog
{
    Q_OBJECT

struct Cls {
    int start_week, end_week; // 第几周单位
    int week;   // 周日~周六，0~6
    int start_course, end_course; // 课程范围：第1~12节课
    bool dual; // 双周
    bool single; // 单周
    int member; // 人数

    Cls()
    {
        dual = single = false;
    }
};

public:
    ShowWindow(QWidget* parent, QString c1, QString c2);

private:
    void initView();
    void analyze(QStringList times, QStringList numbs, QList<Cls> &clss);
    QList<Cls> clssFromString(QString time, QString numb);
    Cls clsFromString(QString time, QString numb);

signals:

public slots:
    void slotWeekChanged(int x);
    void slotDayChanged(int x);

private:
    QStringList times, numbs;
    QComboBox* week_combo;
    QListWidget* time_list;
    QTextEdit* numb_edit;

    QList<Cls> clss;
    int current_week;
    int current_day;
};

#endif // SHOWWINDOW_H
