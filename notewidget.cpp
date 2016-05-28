#include "notewidget.h"

#include <QHBoxLayout>
#include <qDebug>

noteWidget::noteWidget(QWidget *parent) : QWidget(parent)
{
    ew = new editWidget(this);
    nt = new noteTree(this);



    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(nt);
    layout->addWidget(ew);

    layout->setStretchFactor(nt, 2);
    layout->setStretchFactor(ew, 5);

    this->setLayout(layout);

    //双击笔记时打开笔记
    connect(nt, SIGNAL(doubleclicked(QString)), ew, SLOT(slotLoadText(QString)));
}

