#ifndef DOCIMPORT_DIALOG_H
#define DOCIMPORT_DIALOG_H

#include "docparam.h"
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class docimport_dialog : public QDialog
{
    Q_OBJECT
public:
    docimport_dialog(QWidget * parent = 0);
    //~docimport_dialog();
signals:
    void  sendData(Docparam);
private slots:
    void on_okButton();
    void on_openbutton();

private:
    QLineEdit * path;
    QPushButton * openButton;
    QPushButton * okButton;

    QLineEdit * name;
    QLineEdit * author;
    QLineEdit * pub;
    QLineEdit * year;

    QLabel * label_path;
    QLabel * label_name;
    QLabel * label_author;
    QLabel * label_pub;
    QLabel * label_year;


};

#endif // DOCIMPORT_DIALOG_H
