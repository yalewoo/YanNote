#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "notewidget.h"
#include "docwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    noteWidget * nw;
    docwidget * dw;
};

#endif // MAINWINDOW_H
