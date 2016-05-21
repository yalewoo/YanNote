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

    connect(dt, SIGNAL(doubleclicked(QString)), dd, SLOT(changeDir(QString)));
}


