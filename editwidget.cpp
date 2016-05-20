#include "editwidget.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>

editWidget::editWidget(QWidget *parent)
{
    toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(25, 25));
    textEdit = new QTextEdit(this);

    saveAction = new QAction(QIcon(":/ico/ico/save.ico"), "保存", this);
    saveAction->setShortcut(QKeySequence::Save);
    toolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveSlot()));

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(toolBar);
    layout->addWidget(textEdit);
    this->setLayout(layout);


    //setGeometry(400, 130, 700, 500);
}

editWidget::~editWidget()
{

}


void editWidget::saveSlot()
{
    QFile file(name);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "ERROR", "Can't save the file", QMessageBox::Yes);
        return;
    }

    QTextStream out(&file);

    out << textEdit->toHtml();

    textEdit->document()->setModified(false);

    file.close();
}

void editWidget::loadText(QString str)
{
    name = str;
    QFile openFile(str);

    if (!openFile.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&openFile);
    textEdit->setText(in.readAll());
    openFile.close();
}
