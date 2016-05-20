#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class textEdit : public QTextEdit
{
    void mousePressEvent ( QMouseEvent * e );
public:
    textEdit(QWidget * parent = 0);
};



#endif // TEXTEDIT_H
