#include "docdetail.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>
#include <QDebug>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>


#include "docimport_dialog.h"

docdetail::docdetail(QWidget *parent) : QWidget(parent)
{
    rootpath = "D:\\notes\\doc";

    table = new QTableView(this);

    model = new QStandardItemModel();
    setHeader();
    table->setModel(model);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

    table->setContextMenuPolicy(Qt::ActionsContextMenu);

    num = 0;
    num_total = 0;


    table->show();

    toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(25, 25));


    saveAction = new QAction(QIcon(":/ico/ico/save.ico"), "保存", this);
    saveAction->setShortcut(QKeySequence::Save);
    toolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(slotSave()));
    toolBar->addSeparator();

    addAction = new QAction(QIcon(":/ico/ico/import.ico"), "导入", this);
    toolBar->addAction(addAction);
    connect(addAction, SIGNAL(triggered()), this, SLOT(slotAddDocFromFile()));

    openInExplorerAction = new QAction(QIcon(":/ico/ico/doc_openinexplorer.png"), "定位文件", this);
    toolBar->addAction(openInExplorerAction);
    connect(openInExplorerAction, SIGNAL(triggered()), this, SLOT(slotOpenInExplorer()));


    openAction = new QAction(QIcon(":/ico/ico/openinexplorer.png"), "打开文件", this);
    toolBar->addAction(openAction);
    connect(openAction, SIGNAL(triggered()), this, SLOT(slotOpen()));
    toolBar->addSeparator();


    createReference = new QAction(QIcon(":/ico/ico/doc_createreference.png"), "生成参考文献格式", this);
    toolBar->addAction(createReference);
    connect(createReference, SIGNAL(triggered()), this, SLOT(slotCreateReference()));

    insertReference = new QAction(QIcon(":/ico/ico/insert_reference.png"), "插入参考标记", this);
    toolBar->addAction(insertReference);
    connect(insertReference, SIGNAL(triggered()), this, SLOT(slotInsertReference()));



    readNumTotal();










    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(toolBar);
    layout->addWidget(table);



    this->setLayout(layout);
}


void docdetail::slotSave()
{
    save();
}

void docdetail::loadSlot(QString str)
{

}

void docdetail::slotAddDocFromFile()
{
    docimport_dialog * d = new docimport_dialog(this);
    connect(d, SIGNAL(sendData(Docparam)), this, SLOT(slotImport(Docparam)));

    d->exec();
}


void docdetail::slotImport(Docparam p)
{
    //qDebug() << p.fullpath << "aaaa";

    model->setItem(num, 0, new QStandardItem(p.name));
    model->setItem(num, 1, new QStandardItem(p.author));
    model->setItem(num, 2, new QStandardItem(p.pub));
    model->setItem(num, 3, new QStandardItem(p.year));
    model->setItem(num, 4, new QStandardItem(p.fullpath));

    //存储该文献的唯一id
    ++num_total;
    model->setItem(num, 5, new QStandardItem(QString::number(num_total)));


    refTable_path.append(nowpath);
    refTable_pos.append(num);



    ++num;
}

void docdetail::slotNowDirChanged(QString path)
{
    save();


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
        for (int j = 0; j < 6; ++j)
        {
            out << model->item(i, j)->text() << endl;
        }
    }
    out.flush();
    f.close();


    //存储总文献数量
    path = rootpath + "/data.txt";
    QFile f2(path);
    f2.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out2(&f2);
    out2 << num_total << endl;


    //存储文献索引表
    int n = refTable_path.length();
    for (int i = 1; i < n; ++i)
    {
        out2 << refTable_path[i] << endl;
        out2 << refTable_pos[i] << endl;
    }
    out2.flush();
    f2.close();

}
void docdetail::load()
{
    model->clear();
    setHeader();


    QString path = nowpath + "/doc.txt";
    //qDebug() << path;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        num = 0;
        return;
    }

    QTextStream in(&f);
    num = in.readLine().toInt();


    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < 6; ++j)
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
    model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("年份代码")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("链接")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("id")));
}



void docdetail::slotOpenInExplorer()
{
    int n = table->currentIndex().row();
    QString str = model->data(model->index(n,4)).toString();//第n行第4列的内容



    QFileInfo fi=QFileInfo(str);
    QString filePath;
    filePath=fi.absolutePath();

    qDebug() << filePath;

    QDesktopServices::openUrl(QUrl(filePath, QUrl::TolerantMode));
}

void docdetail::slotOpen()
{
    int n = table->currentIndex().row();
    QString str = model->data(model->index(n,4)).toString();//第n行第4列的内容



    str = "file:///" + str;

    qDebug() << str;

    QDesktopServices::openUrl(QUrl(str, QUrl::TolerantMode));
}

void docdetail::slotCreateReference()
{
    int n = table->currentIndex().row();
    QString str1 = model->data(model->index(n,0)).toString();
    QString str2 = model->data(model->index(n,1)).toString();
    QString str3 = model->data(model->index(n,2)).toString();
    QString str4 = model->data(model->index(n,3)).toString();

    QString str = "  " + str2 + "." + str1 + "." + str3 + "." + str4;

    qDebug() << str;

    QClipboard *board = QApplication::clipboard();
    board->setText(str);

}


void docdetail::slotInsertReference()
{
    int n = table->currentIndex().row();
    QString str1 = model->data(model->index(n,0)).toString();
    QString str2 = model->data(model->index(n,1)).toString();
    QString str3 = model->data(model->index(n,2)).toString();
    QString str4 = model->data(model->index(n,3)).toString();
    int str5 = model->data(model->index(n,5)).toInt();

    QString str = "  " + str2 + "." + str1 + "." + str3 + "." + str4;

    emit signalInsertReference(str, str5);
}


void docdetail::readNumTotal()
{
    refTable_path.clear();
    refTable_pos.clear();

    //初始化文献表
    refTable_path.append("");
    refTable_pos.append(num_total);

    QString path = rootpath + "/data.txt";
    QFile f(path);
    if (f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&f);
        num_total = in.readLine().toInt();

        for (int i = 0; i < num_total; ++i)
        {
            refTable_path.append(in.readLine());
            refTable_pos.append(in.readLine().toInt());
        }
        f.close();
    }
}


void docdetail::skipToRef(int id)
{
    nowpath = refTable_path[id];

    emit expandDir(nowpath);

    load();

    table->selectRow(refTable_pos[id]);
}


