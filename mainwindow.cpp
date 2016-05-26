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

    connect(dw->dd, SIGNAL(signalInsertReference(QString)), this, SLOT(slotInsertReference(QString)));

    tabwidget->addTab(nw, "笔记");
    tabwidget->addTab(dw, "文献");

    this->resize(1000, 600);

    this->setCentralWidget(tabwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotInsertReference(QString str)
{
    nw->ew->slotInsertReference(str);

    tabwidget->setCurrentIndex(0);
}
