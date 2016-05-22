#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTabWidget * tabwidget = new QTabWidget(this);


    nw = new noteWidget(this);
    dw = new docwidget(this);

    tabwidget->addTab(nw, "笔记");
    tabwidget->addTab(dw, "文献");

    this->resize(1000, 600);

    this->setCentralWidget(tabwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
