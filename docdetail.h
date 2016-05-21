#ifndef DOCDETAIL_H
#define DOCDETAIL_H

#include <QWidget>
#include <QTableWidget>
#include <QToolBar>
#include <QApplication>

class docdetail : public QWidget
{
    Q_OBJECT
public:
    docdetail(QWidget *parent = 0);

    QTableWidget * table;

    void loadSlot(QString str);

protected slots:
    void saveSlot();    //保存
    void addSlot(); //导入文献

private:
    QToolBar * toolBar;
    QString name;

    QAction *saveAction;
    QAction * addAction;
};

#endif // DOCDETAIL_H
