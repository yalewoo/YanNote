#ifndef NOTETREE_H
#define NOTETREE_H

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
    void doubleclicked(QString str);

public slots:
    void customContextMenuSlot(const QPoint p);
    void renameSlot();
    void newFileSlot();
    void newDirSlot();

private:
    QFileSystemModel *model;
    QFileInfo info;

};

#endif // NOTETREE_H
