#ifndef EDITWIDGET_H
#define EDITWIDGET_H

/*
    编辑区
*/

#include <QTextEdit>

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
    void slotLoadText(QString str);

    void slotPrevRef(); //跳转到上一个引用处
    void slotNextRef(); //跳转到下一个引用处

    void slotSkipToRef();   //跳转到文献位置处

public:
    void slotInsertReference(QString str, int ref_id);  //插入引用


private:
    //工具栏
    QToolBar * toolBar;
    QAction *saveAction;    //保存笔记
    bool firstload = true;


    QFontComboBox * fontBox;    //字体
    QComboBox * sizeBox;    //字号
    QToolButton * boldBtn;  //粗体
    QToolButton * italicBtn;    //斜体
    QToolButton * underlineBtn; //下划线
    QToolButton * colorBtn; //颜色

    QAction * prevRefAction;    //上一个引用
    QAction * nextRefAction;    //下一个引用

    QAction *skipRefAction; //跳转到文献面板

    //编辑区域
    QTextEdit * textedit;

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
