#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include "textedit.h"

#include <QToolBar>
#include <QApplication>

#include <QLabel>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>

class editWidget : public QWidget
{
    Q_OBJECT

public:
    explicit editWidget(QWidget *parent = 0);
    ~editWidget();
    void loadText(QString str);
protected slots:
    void slotSave();    //保存文件
    void slotFont( QString f ); //设置字体
    void slotSize( QString num );   //设置字号
    void slotBold();    //加粗
    void slotItalic();  //斜体
    void slotUnder();   //下划线
    void slotColor();   //设置颜色
    void slotNowFormatChanged( const QTextCharFormat & fmt );   //格式改变
    void mergeFormat(QTextCharFormat fmt);

public:
    void slotInsertReference(QString str);


private:
    //工具栏
    QToolBar * toolBar;
    QAction *saveAction;

    QLabel * label1;
    QLabel * label2;
    QFontComboBox * fontBox;
    QComboBox * sizeBox;
    QToolButton * boldBtn;
    QToolButton * italicBtn;
    QToolButton * underlineBtn;
    QToolButton * colorBtn;

    //编辑区域
    textEdit * textedit;

    //内部数据结构
    QString name;


};

#endif // EDITWIDGET_H
