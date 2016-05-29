#include "docwidget.h"

#include <QHBoxLayout>
#include <qDebug>

docwidget::docwidget(QWidget *parent) : QWidget(parent)
{
    dd = new docdetail(this);
    dt = new doctree(this);


    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(dt);
    layout->addWidget(dd);

    layout->setStretchFactor(dt, 2);
    layout->setStretchFactor(dd, 5);

    this->setLayout(layout);

    //点击左侧文件夹后自动加载该分类下的文献
    connect(dt, SIGNAL(doubleclicked(QString)), dd, SLOT(slotNowDirChanged(QString)));
    //从笔记处跳转过来后自动定位文献的位置
    connect(dd, SIGNAL(expandDir(QString)), dt, SLOT(slotSkipRef(QString)));
}
