#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nw = new noteWidget(this);
    this->setCentralWidget(nw);
}

MainWindow::~MainWindow()
{
    delete ui;
}
