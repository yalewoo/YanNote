#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include "textedit.h"

#include <QToolBar>
#include <QApplication>

class editWidget : public QWidget
{
    Q_OBJECT

public:
    explicit editWidget(QWidget *parent = 0);
    ~editWidget();
    void loadText(QString str);
protected slots:
    void saveSlot();


private:
    QToolBar * toolBar;
    textEdit * textedit;
    QString name;

    QAction *saveAction;
};

#endif // EDITWIDGET_H
