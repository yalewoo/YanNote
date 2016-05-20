#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include "editwidget.h"
#include "notetree.h"

class noteWidget : public QWidget
{
    Q_OBJECT

    QString filename;

protected slots:
    void reloadfileSlot(QString str);

public:
    noteWidget(QWidget *parent = 0);
    editWidget * ew;
    noteTree * nt;
};

#endif // NOTEWIDGET_H
