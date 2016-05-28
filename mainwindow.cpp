#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabwidget = new QTabWidget(this);


    nw = new noteWidget(this);
    dw = new docwidget(this);

    connect(dw->dd, SIGNAL(signalInsertReference(QString, int)), this, SLOT(slotInsertReference(QString, int)));

    connect(nw->ew, SIGNAL(signalSkipToRef(int)), this, SLOT(slotSkipToRef(int)));

    tabwidget->addTab(nw, "笔记");
    tabwidget->addTab(dw, "文献");

    this->resize(1000, 600);

    this->setCentralWidget(tabwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotInsertReference(QString str, int ref_id)
{
    nw->ew->slotInsertReference(str, ref_id);

    tabwidget->setCurrentIndex(0);
}

void MainWindow::slotSkipToRef(int id)
{
    dw->dd->skipToRef(id);

    tabwidget->setCurrentIndex(1);
}
