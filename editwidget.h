#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include "textedit.h"

#include <QToolBar>
#include <QApplication>

#include <QLabel>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>

#include <QVector>


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

    void slotPrevRef();
    void slotNextRef();

    void slotSkipToRef();

public:
    void slotInsertReference(QString str, int ref_id);


private:
    //工具栏
    QToolBar * toolBar;
    QAction *saveAction;
    bool firstload = true;

    QLabel * label1;
    QLabel * label2;
    QFontComboBox * fontBox;
    QComboBox * sizeBox;
    QToolButton * boldBtn;
    QToolButton * italicBtn;
    QToolButton * underlineBtn;
    QToolButton * colorBtn;

    QAction * prevRefAction;
    QAction * nextRefAction;

    QAction *skipRefAction;

    //编辑区域
    textEdit * textedit;

    //内部数据结构
    QString name;
    int ref_num;    //本文引用个数

    QVector<int> refTable;  //存放文章中引用的所有文献
    void saveRefTable();    //保存引用表
    void loadRefTable();    //从文件加载引用表


signals:
    void signalSkipToRef(int id);

};

#endif // EDITWIDGET_H
