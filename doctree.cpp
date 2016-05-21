#include "doctree.h"

#include <QModelIndex>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QDesktopServices>
#include <QInputDialog>
#include <QMessageBox>

doctree::doctree(QWidget *parent) : QTreeView(parent)
{
    model = new QFileSystemModel;
    model->setReadOnly(false);
    model->setFilter(QDir::Dirs|QDir::NoDotAndDotDot);        //只显示文件夹
    //qDebug() << QStandardPaths::writableLocation(QStandardPaths::DataLocation);


    model->setRootPath("E:\\Documents\\notes\\doc");
    this->setModel(model);
    this->setRootIndex(model->index("E:\\Documents\\notes\\doc"));

    this->header()->setStretchLastSection(true);
    this->header()->setSortIndicator(0, Qt::AscendingOrder);
    this->header()->setSortIndicatorShown(true);
    this->header()->setSectionsClickable(true);
    this->setSortingEnabled(true);

    //只显示文件名 不显示表头
    this->hideColumn(2);
    this->hideColumn(3);
    this->hideColumn(1);
    this->setHeaderHidden(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuSlot(QPoint)));

}

// 鼠标双击
void doctree::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = currentIndex();

        QFileInfo info = model->fileInfo(index);

        QString name = info.path() + "/" + info.fileName();

        emit doubleclicked(name);

    }
}

//鼠标右键
void doctree::customContextMenuSlot(const QPoint p)
{
    QModelIndex index = currentIndex();

    info = model->fileInfo(index);

    //qDebug() << info.filePath();

    QMenu *menu = new QMenu;

    if (info.isDir())
    {
        menu->addAction(QString("新建笔记"), this, SLOT(newFileSlot()));
        menu->addAction(QString("新建分类"), this, SLOT(newDirSlot()));
    }

    menu->addAction(QString("重命名"), this, SLOT(renameSlot()));
    //menu->addAction(QString("Export"), this, SLOT(slotTest()));
    menu->exec(QCursor::pos());


}

void doctree::renameSlot()
{
    QString path = info.filePath();
    QString oldname = info.baseName();

    QString text = QInputDialog::getText(NULL, "输入新文件名",
                                   "输入新文件名",
                                   QLineEdit::Normal,
                                   oldname);

    QString newname = info.path() + "/" + text + ".html";
    QFile file(path);

    file.rename(newname);
    file.close();
}

void doctree::newFileSlot()
{
    QString path = info.filePath();

    QString text = QInputDialog::getText(NULL, "输入新文件名",
                                   "输入新文件名",
                                   QLineEdit::Normal,
                                   "未命名");

    QString newname = path + "/" + text + ".html";



    QFile file(newname);
    file.open( QIODevice::WriteOnly);
    file.close();

    emit doubleclicked(newname);
}

void doctree::newDirSlot()
{
    QString path = info.filePath();

    QString text = QInputDialog::getText(NULL, "输入新文件名",
                                   "输入新文件名",
                                   QLineEdit::Normal,
                                   "未命名");

    QString newname = path + "/" + text;


    QDir newdir;
    newdir.mkdir(newname);

}
