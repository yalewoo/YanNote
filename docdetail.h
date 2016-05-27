#ifndef DOCDETAIL_H
#define DOCDETAIL_H

#include <QWidget>
#include <QTableWidget>
#include <QToolBar>
#include <QApplication>
#include <QStandardItemModel>
#include "docparam.h"

#include <QVector>



class docdetail : public QWidget
{
    Q_OBJECT
public:
    docdetail(QWidget *parent = 0);

    QTableView * table;

    void loadSlot(QString str);

protected slots:
    void slotSave();    //保存
    void slotAddDocFromFile(); //导入文献
    void slotOpenInExplorer(); //在资源管理器中查看
    void slotOpen();    //直接打开文献
    void slotCreateReference(); //生成参考文献格式
    void slotInsertReference(); //在笔记中插入该文献引用

    void slotImport(Docparam p);
    void slotNowDirChanged(QString path);

private:
    QToolBar * toolBar;
    QString nowpath;    //当前分类路径
    QString rootpath;
    int num;    //存放当前分类文件数
    int num_total;  //存放总文档数

    QVector<QString> refTable_path; //文献的分类地址
    QVector<int> refTable_pos;  //文献在分类中的序号



    QAction * saveAction;
    QAction * addAction;

    QAction * openInExplorerAction;
    QAction * openAction;

    QAction * createReference;
    QAction * insertReference;

    void createInitFile();
    QStandardItemModel *model;
    void save();
    void load();
    void setHeader();

    void readNumTotal();

signals:
    void  signalInsertReference(QString, int);

};

#endif // DOCDETAIL_H
