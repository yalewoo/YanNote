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

    QString filename;

protected slots:
    void reloadfileSlot(QString str);

public:

    docdetail * dd;
    doctree * dt;
};

#endif // DOCWIDGET_H
