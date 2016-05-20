#include "textedit.h"

textEdit::textEdit(QWidget * parent) : QTextEdit(parent)
{

}


void textEdit::mousePressEvent ( QMouseEvent * e )
{
    QTextEdit::mousePressEvent(e);


}
