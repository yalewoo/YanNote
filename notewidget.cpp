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

    this->setLayout(layout);

    connect(nt, SIGNAL(doubleclicked(QString)), this, SLOT(reloadfileSlot(QString)));
}


void noteWidget::reloadfileSlot(QString str)
{
    ew->loadText(str);
}
