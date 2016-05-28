#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //初始化标签页
    tabwidget = new QTabWidget(this);


    nw = new noteWidget(this);  //笔记
    dw = new docwidget(this);   //文献管理

    //插入文献信号槽
    connect(dw->dd, SIGNAL(signalInsertReference(QString, int)), this, SLOT(slotInsertReference(QString, int)));
    //定位文献信号槽
    connect(nw->ew, SIGNAL(signalSkipToRef(int)), this, SLOT(slotSkipToRef(int)));

    tabwidget->addTab(nw, "笔记");
    tabwidget->addTab(dw, "文献");

    this->resize(1000, 600);

    this->setCentralWidget(tabwidget);
}

MainWindow::~MainWindow()
{

}

//插入文献函数
void MainWindow::slotInsertReference(QString str, int ref_id)
{
    //在编辑区域插入参考文献
    nw->ew->slotInsertReference(str, ref_id);

    //激活笔记标签
    tabwidget->setCurrentIndex(0);
}

//跳转到文献槽函数
void MainWindow::slotSkipToRef(int id)
{
    //文献详细信息区显示该文献
    dw->dd->skipToRef(id);

    //激活文献管理标签
    tabwidget->setCurrentIndex(1);
}
