#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "notewidget.h"
#include "docwidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    noteWidget * nw;    //笔记面板
    docwidget * dw;     //文献管理面板
    QTabWidget * tabwidget; //Tab窗口，包含笔记和文献管理两个标签页

private slots:

    /*插入文献信息槽函数
    str是要插入的引用文字：格式：  杨志平，杜堇等.个人知识管理工具综述.中国科学院国家图书馆.2013.03.30
    ref_id是该文献在文献管理部分对应的唯一编号，用于快速定位该文献
    该函数功能：  1. 调用nw中笔记编辑器的类的插入文献槽函数
                2. 修改标签页跳转到笔记界面*/
    void slotInsertReference(QString str, int ref_id);

    /*跳转到文献处槽函数
    id是标识文献的唯一编号*/
    void slotSkipToRef(int id);
};

#endif // MAINWINDOW_H
