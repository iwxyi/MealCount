#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>

class ShowWindow : public QDialog
{
    Q_OBJECT
public:
    ShowWindow(QWidget* parent, QString c1, QString c2);

private:
    void initView();
    void analyze();

signals:

public slots:

private:
    QStringList times, numbs;
    QComboBox* week_combo;
    QListWidget* time_list;
    QTextEdit* numb_edit;
};

#endif // SHOWWINDOW_H
