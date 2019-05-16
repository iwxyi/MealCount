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

class ShowWindow : public QDialog
{
    Q_OBJECT

struct Cls {
    int start_week, end_week; // 第几周
    int week;   // 周日~周六，0~6
    int course; // 课程：第1~12节课
    bool dual; // 双周
    int member; // 人数
};

public:
    ShowWindow(QWidget* parent, QString c1, QString c2);

private:
    void initView();
    void analyze(QStringList times, QStringList numbs, QList<Cls> &clss);

signals:

public slots:
    void slotWeekChanged(int x);

private:
    QStringList times, numbs;
    QComboBox* week_combo;
    QListWidget* time_list;
    QTextEdit* numb_edit;

    QList<Cls> clss;
};

#endif // SHOWWINDOW_H
