#ifndef DOCWIDGET_H
#define DOCWIDGET_H

#include <QWidget>

#include "docdetail.h"
#include "doctree.h"

class docwidget : public QWidget
{
    Q_OBJECT
public:
    docwidget(QWidget * parent = 0);

public:
    docdetail * dd; //文献详细信息界面
    doctree * dt;   //文献分类显示界面
};

#endif // DOCWIDGET_H
