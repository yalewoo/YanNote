#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

/*
    笔记部分主窗口 包含左边的树形分类显示和右边的编辑区域
*/
#include "editwidget.h"
#include "notetree.h"

class noteWidget : public QWidget
{
    Q_OBJECT

public:
    noteWidget(QWidget *parent = 0);
    editWidget * ew;
    noteTree * nt;
};

#endif // NOTEWIDGET_H
