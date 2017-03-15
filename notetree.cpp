#include "notetree.h"

#include <QModelIndex>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QDesktopServices>
#include <QInputDialog>
#include <QMessageBox>


noteTree::noteTree(QWidget *parent) : QTreeView(parent)
{
    //用model存放笔记目录的文件模型
    model = new QFileSystemModel;
    model->setReadOnly(false);  //可以修改文件

    //设置过滤器 只显示html文件
    QStringList strlist;
    strlist << QString("*.html");
    model->setNameFilters(strlist);
    model->setNameFilterDisables(false);



    //设置笔记所在的目录
    model->setRootPath("D:\\notes\\note");

    //将TreeView绑定model
    this->setModel(model);
    this->setRootIndex(model->index("D:\\notes\\note"));

    //设置表头
    this->header()->setStretchLastSection(true);
    this->header()->setSortIndicator(0, Qt::AscendingOrder);
    this->header()->setSortIndicatorShown(true);
    this->header()->setSectionsClickable(true);
    this->setSortingEnabled(true);

    //只显示文件名 隐藏后面3列信息
    this->hideColumn(2);
    this->hideColumn(3);
    this->hideColumn(1);
    this->setHeaderHidden(true);

    //打开右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    //重写右键菜单
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuSlot(QPoint)));

}




// 鼠标双击
void noteTree::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = currentIndex();
        QFileInfo info = model->fileInfo(index);
        QString name = info.path() + "/" + info.fileName();
        //至此 name是要打开的笔记的绝对路径

        //发送信号 进行双击打开笔记
        emit doubleclicked(name);

    }
}

//鼠标右键
void noteTree::customContextMenuSlot(const QPoint p)
{
    QModelIndex index = currentIndex();
    info = model->fileInfo(index);

    QMenu *menu = new QMenu;

    if (info.isDir())   //只在目录中有新建选项
    {
        menu->addAction(QString("新建笔记"), this, SLOT(newFileSlot()));
        menu->addAction(QString("新建分类"), this, SLOT(newDirSlot()));
    }

    menu->addAction(QString("重命名"), this, SLOT(renameSlot()));

    //在鼠标点击处弹出菜单
    menu->exec(QCursor::pos());
}

//重命名
void noteTree::renameSlot()
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

//新建笔记
void noteTree::newFileSlot()
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

//新建分类
void noteTree::newDirSlot()
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
