#ifndef NOTETREE_H
#define NOTETREE_H


/*
    用树形图显示笔记，继承自QTreeView
*/


#include <QTreeView>
#include <QList>
#include <QStandardItem>
#include <QMouseEvent>

#include <QFileSystemModel>

class noteTree : public QTreeView
{
    Q_OBJECT
public:
    explicit noteTree(QWidget *parent = 0);

    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void doubleclicked(QString str);    //双击笔记后的信号

public slots:
    void customContextMenuSlot(const QPoint p); //自定义右键菜单
    void renameSlot();  //重命名
    void newFileSlot(); //新建笔记
    void newDirSlot();  //新建分类

private:
    QFileSystemModel *model;    //存放文件系统的模型
    QFileInfo info; //用于读取文件相关信息（当前笔记）

};

#endif // NOTETREE_H
