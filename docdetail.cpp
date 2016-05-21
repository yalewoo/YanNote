#include "docdetail.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>

#include "docimport_dialog.h"

docdetail::docdetail(QWidget *parent) : QWidget(parent)
{
    table = new QTableWidget(this);
    table->setColumnCount(4); //设置列数
    QStringList header;
    header<<"名称"<<"作者" << "出版地" << "年份";
    table->setHorizontalHeaderLabels(header);

    table->setItem(0,0,new QTableWidgetItem("Jan"));
    table->setItem(1,0,new QTableWidgetItem("Feb"));
    table->setItem(2,0,new QTableWidgetItem("Mar"));



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
    QFile file(name);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "ERROR", "Can't save the file", QMessageBox::Yes);
        return;
    }

    QTextStream out(&file);

    out << "table->";


    file.close();
}

void docdetail::loadSlot(QString str)
{
    name = str;
    QFile openFile(str);

    if (!openFile.open(QIODevice::ReadOnly))
        return;

    openFile.close();
}

void docdetail::addSlot()
{
    docimport_dialog * d = new docimport_dialog(this);
    d->exec();
}
