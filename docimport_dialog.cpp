#include "docimport_dialog.h"

#include <QDebug>
#include <QGridLayout>
#include <QFileDialog>


docimport_dialog::docimport_dialog(QWidget * parent) : QDialog(parent)
{
    label_path = new QLabel("路径");
    label_name = new QLabel("标题");
    label_author = new QLabel("作者");
    label_pub = new QLabel("出版地");
    label_year = new QLabel("年份");

    path = new QLineEdit(this);
    name = new QLineEdit(this);
    author = new QLineEdit(this);
    pub = new QLineEdit(this);
    year = new QLineEdit(this);

    openButton = new QPushButton("浏览");
    okButton = new QPushButton("确定");

    QGridLayout * layout = new QGridLayout;

    layout->addWidget(label_path, 0, 0);
    layout->addWidget(label_name, 1, 0);
    layout->addWidget(label_author, 2, 0);
    layout->addWidget(label_pub, 3, 0);
    layout->addWidget(label_year, 4, 0);

    layout->addWidget(path, 0, 1);
    layout->addWidget(name, 1, 1);
    layout->addWidget(author, 2, 1);
    layout->addWidget(pub, 3, 1);
    layout->addWidget(year, 4, 1);

    layout->addWidget(openButton, 0, 2);
    layout->addWidget(okButton, 4, 2);

    this->setLayout(layout);


    connect(openButton, SIGNAL(clicked(bool)), this, SLOT(on_openbutton()));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(on_okButton()));

}

//docimport_dialog::~docimport_dialog(){
//    delete label_path;
//    delete label_name;
//    delete label_author;
//    delete label_pub;
//    delete label_year;
//    delete path;
//    delete name;
//    delete author;
//    delete pub;
//    delete year;
//    delete openButton;
//    delete okButton;
//}

void docimport_dialog::on_okButton()
{
    Docparam p;
    p.fullpath = path->text();
    p.name = name->text();
    p.author = author->text();
    p.pub = pub->text();
    p.year = year->text();


    emit sendData(p);

    this->close();
}

void docimport_dialog::on_openbutton()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"));
    qDebug() << fileName;

    QFileInfo f(fileName);

    path->setText(fileName);
    name->setText(f.baseName());



    //qDebug() << "queding clicked";
}
