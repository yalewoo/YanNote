#ifndef DOCDETAIL_H
#define DOCDETAIL_H

#include <QWidget>
#include <QTableWidget>
#include <QToolBar>
#include <QApplication>
#include <QStandardItemModel>
#include "docparam.h"

class docdetail : public QWidget
{
    Q_OBJECT
public:
    docdetail(QWidget *parent = 0);

    QTableView * table;

    void loadSlot(QString str);

protected slots:
    void saveSlot();    //保存
    void addSlot(); //导入文献
    void openInExplorerSlot(); //在资源管理器中查看
    void openSlot();    //直接打开文献
    void createReferenceSlot(); //生成参考文献格式

    void importSlot(Docparam p);
    void changeDir(QString path);

private:
    QToolBar * toolBar;
    QString nowpath;
    int num;

    QAction * saveAction;
    QAction * addAction;

    QAction * openInExplorerAction;
    QAction * openAction;

    QAction * createReference;

    void createInitFile();
    QStandardItemModel *model;
    void save();
    void load();
    void setHeader();
};

#endif // DOCDETAIL_H
