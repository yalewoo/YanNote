#include "docdetail.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>
#include <QDebug>
#include <QStandardItemModel>

#include "docimport_dialog.h"

docdetail::docdetail(QWidget *parent) : QWidget(parent)
{
    table = new QTableView(this);

    model = new QStandardItemModel();
    setHeader();
    table->setModel(model);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

    table->setContextMenuPolicy(Qt::ActionsContextMenu);

    num = 0;


    table->show();

    toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(25, 25));


    saveAction = new QAction(QIcon(":/ico/ico/save.ico"), "保存", this);
    saveAction->setShortcut(QKeySequence::Save);
    toolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveSlot()));

    addAction = new QAction(QIcon(":/ico/ico/import.ico"), "导入", this);
    toolBar->addAction(addAction);
    connect(addAction, SIGNAL(triggered()), this, SLOT(addSlot()));


    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(toolBar);
    layout->addWidget(table);



    this->setLayout(layout);
}


void docdetail::saveSlot()
{
    save();
}

void docdetail::loadSlot(QString str)
{

}

void docdetail::addSlot()
{
    docimport_dialog * d = new docimport_dialog(this);
    connect(d, SIGNAL(sendData(Docparam)), this, SLOT(importSlot(Docparam)));

    d->exec();
}


void docdetail::importSlot(Docparam p)
{
    //qDebug() << p.fullpath << "aaaa";

    model->setItem(num, 0, new QStandardItem(p.name));
    model->setItem(num, 1, new QStandardItem(p.author));
    model->setItem(num, 2, new QStandardItem(p.pub));
    model->setItem(num, 3, new QStandardItem(p.year));
    model->setItem(num, 4, new QStandardItem(p.fullpath));


    ++num;
}

void docdetail::changeDir(QString path)
{
    nowpath = path;
    //qDebug() << nowpath;

    model->clear();
    setHeader();

    load();
}

void docdetail::createInitFile()
{
    QString path = nowpath + "/doc.txt";
    QFile f(nowpath);
    f.open(QIODevice::WriteOnly|QIODevice::Text);

    QTextStream out(&f);
    out<<"0"<<endl;
    out.flush();
    f.close();
}

void docdetail::save()
{
    QString path = nowpath + "/doc.txt";
    //qDebug() << path;
    QFile f(path);
    f.open(QIODevice::WriteOnly|QIODevice::Text);

    QTextStream out(&f);
    out << num << endl;

    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            out << model->item(i, j)->text() << endl;
        }
    }
    out.flush();
    f.close();
}
void docdetail::load()
{
    QString path = nowpath + "/doc.txt";
    //qDebug() << path;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QTextStream in(&f);
    num = in.readLine().toInt();


    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            QString str = in.readLine();
            model->setItem(i, j, new QStandardItem(str));
        }
    }

    f.close();
}

void docdetail::setHeader()
{
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("标题")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("作者")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("出版地")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("年份")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("链接")));
}
