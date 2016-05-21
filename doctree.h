#ifndef DOCTREE_H
#define DOCTREE_H


#include <QTreeView>
#include <QList>
#include <QStandardItem>
#include <QMouseEvent>

#include <QFileSystemModel>

class doctree : public QTreeView
{
    Q_OBJECT
public:
    explicit doctree(QWidget *parent = 0);

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

#endif // DOCTREE_H
