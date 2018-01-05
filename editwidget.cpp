#include "editwidget.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>

#include <QColorDialog>
#include <QDebug>


#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextObject>

#include <QRegularExpression>

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

editWidget::editWidget(QWidget *parent) : QWidget(parent)
{
    ref_num = 0;
    refTable.append(ref_num);


    //创建工具栏
    toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(25, 25));

    //创建编辑窗口
    textedit = new QTextEdit(this);

    //保存按钮
    saveAction = new QAction(QIcon(":/ico/ico/save.ico"), "保存", this);
    saveAction->setShortcut(QKeySequence::Save);
    toolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(slotSave()));


    //字体
    fontBox = new QFontComboBox();
    fontBox->setFontFilters( QFontComboBox::ScalableFonts );
    toolBar->addWidget( fontBox );
    //字号
    sizeBox = new QComboBox();
    toolBar->addWidget( sizeBox );
    QFontDatabase db;
    foreach( int nSize, db.standardSizes() )
           sizeBox->addItem( QString::number( nSize ) );

    toolBar->addSeparator();

    //加粗、斜体、下划线、颜色
    boldBtn = new QToolButton();
    boldBtn->setIcon( QIcon( ":/ico/ico/format_text_bold.ico" ) );
    boldBtn->setCheckable( true );
    toolBar->addWidget( boldBtn );
    //斜体
    italicBtn = new QToolButton();
    italicBtn->setIcon( QIcon( ":/ico/ico/format_text_italic.ico" ) );
    italicBtn->setCheckable( true );
    toolBar->addWidget( italicBtn );
    //下划线
    underlineBtn = new QToolButton();
    underlineBtn->setIcon( QIcon( ":/ico/ico/format_text_underline.ico" ) );
    underlineBtn->setCheckable( true );
    toolBar->addWidget( underlineBtn );

    toolBar->addSeparator();

    //查找按钮
    findAction = new QAction(QIcon(":/ico/ico/lookup.png"), "查找", this);
    findAction->setShortcut(QKeySequence::Find);
    toolBar->addAction(findAction);
    connect(findAction, SIGNAL(triggered()), this, SLOT(showFindText()));

    findDlg = new QDialog(this);
    findDlg->setWindowTitle("查找");
    findLineEdit = new QLineEdit(findDlg);
    QPushButton *btn = new QPushButton("查找下一个", findDlg);
    QVBoxLayout * layout_finddlg = new QVBoxLayout(findDlg);
    layout_finddlg->addWidget(findLineEdit);
    layout_finddlg->addWidget(btn);
    connect(btn, &QPushButton::clicked, this, &editWidget::slotFind);


    //颜色
    colorBtn = new QToolButton();
    colorBtn->setIcon( QIcon( ":/ico/ico/format_color.ico" ) );
    toolBar->addWidget( colorBtn );

    //连接信号与槽函数
    connect( fontBox, SIGNAL( activated( QString ) ), this, SLOT( slotFont( QString ) ) );
    connect( sizeBox, SIGNAL( activated( QString ) ), this, SLOT( slotSize( QString ) ) );
    connect( boldBtn, SIGNAL( clicked() ), this, SLOT( slotBold() ) );
    connect( italicBtn, SIGNAL( clicked() ), this, SLOT( slotItalic() ) );
    connect( underlineBtn, SIGNAL( clicked() ), this, SLOT( slotUnder() ) );
    connect( colorBtn, SIGNAL( clicked() ), this, SLOT( slotColor() ) );
    connect( textedit, SIGNAL( currentCharFormatChanged( const QTextCharFormat & ) ), this, SLOT( slotNowFormatChanged( const QTextCharFormat& ) ) );


    //上一个引用
    prevRefAction = new QAction(QIcon(":/ico/ico/prevref.png"), "上一个引用", this);
    toolBar->addAction(prevRefAction);
    connect(prevRefAction, SIGNAL(triggered()), this, SLOT(slotPrevRef()));
    //下一个引用
    nextRefAction = new QAction(QIcon(":/ico/ico/nextref.png"), "上一个引用", this);
    toolBar->addAction(nextRefAction);
    connect(nextRefAction, SIGNAL(triggered()), this, SLOT(slotNextRef()));

    //跳转到参考文献
    skipRefAction = new QAction(QIcon(":/ico/ico/skipRef.png"), "跳转到文献", this);
    toolBar->addAction(skipRefAction);
    connect(skipRefAction, SIGNAL(triggered()), this, SLOT(slotSkipToRef()));

    //删除参考文献
    deleteRefAction = new QAction(QIcon(":/ico/ico/deletered.ico"), "删除文献", this);
    toolBar->addAction(deleteRefAction);
    connect(deleteRefAction, SIGNAL(triggered()), this, SLOT(deleteRef()));





    //设置布局 工具栏在上 编辑器在下
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(toolBar);
    layout->addWidget(textedit);
    this->setLayout(layout);
}

editWidget::~editWidget()
{

}

//点击保存按钮后的动作
void editWidget::slotSave()
{
    QFile file(notepath);

    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream out(&file);

    out << textedit->toHtml();

    textedit->document()->setModified(false);

    file.close();

    //保存文献信息表
    saveRefTable();
}

void editWidget::slotLoadText(QString str)
{
    if (!firstload)
        slotSave();

    notepath = str;
    QFile openFile(str);

    if (!openFile.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&openFile);
    textedit->setText(in.readAll());
    openFile.close();

    loadRefTable();

    firstload = false;
}

void editWidget::slotFind()
{
    QString str = findLineEdit->text();
    if (!textedit->find(str, QTextDocument::FindBackward))
    {
        QMessageBox::warning(this, "查找", "找不到 "+ str);
    }
}

void editWidget::showFindText()
{
   findDlg->show();
}


//设置字体
void editWidget::slotFont(QString f)
{
       QTextCharFormat fmt;
       fmt.setFontFamily(f);
       mergeFormat(fmt);
}
//设置字号
void editWidget::slotSize(QString num)
{
       QTextCharFormat fmt;
       fmt.setFontPointSize(num.toFloat());
       mergeFormat(fmt);
}
//加粗
void editWidget::slotBold()
{
       QTextCharFormat fmt;
       fmt.setFontWeight(boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
       mergeFormat(fmt);
}
//斜体
void editWidget::slotItalic()
{
       QTextCharFormat fmt;
       fmt.setFontItalic(italicBtn->isChecked());
       mergeFormat(fmt);
}
//下划线
void editWidget::slotUnder()
{
       QTextCharFormat fmt;
       fmt.setFontUnderline(underlineBtn->isChecked());
       mergeFormat(fmt);
}
//设置颜色
void editWidget::slotColor()
{
       QColor color = QColorDialog::getColor(Qt::red, this);

       if (color.isValid())
       {
              QTextCharFormat fmt;
              fmt.setForeground(color);
              mergeFormat(fmt);
       }
}

//当光标所在处的字符格式发生变化时调用，函数根据新的字符格式把工具栏上的各个格式控件的显示更新
void editWidget::slotNowFormatChanged(const QTextCharFormat & fmt)
{
       fontBox->setCurrentIndex(fontBox->findText(fmt.fontFamily()));
       sizeBox->setCurrentIndex(sizeBox->findText(QString::number(fmt.fontPointSize())));
       boldBtn->setChecked(fmt.font().bold());
       italicBtn->setChecked(fmt.fontItalic());
       underlineBtn->setChecked(fmt.fontUnderline());
}
//设置光标的选区，使格式作用于选区内的字符，若没有选区则作用于光标所在处的字符
void editWidget::mergeFormat(QTextCharFormat fmt)
{
       QTextCursor cursor = textedit->textCursor();

       if (!cursor.hasSelection())
              cursor.select(QTextCursor::WordUnderCursor);
       cursor.mergeCharFormat(fmt);
       textedit->mergeCurrentCharFormat(fmt);
}

/*  插入参考文献 str是引用时的文字格式 ref_id是引用文献对应的唯一编号
    该函数在文本中查找光标处上下文引用，生成正确的编号 并更新后面的编号
    该函数还要修改参考文献后面的引用
*/
void editWidget::slotInsertReference(QString str, int ref_id)
{
    //本笔记引用数加1
    ++ref_num;

    int thisnum;
    //qDebug() << str;

    //获取编辑区文档和光标 以供后面修改
    QTextDocument * doc = textedit->document();
    QTextCursor cursor(textedit->textCursor());

    //设置格式为上标
    QTextCharFormat fmt(cursor.charFormat());
    fmt.setVerticalAlignment(QTextCharFormat::AlignSuperScript);

    //查找上一个引用的数字
    QRegularExpression re("\[[0-9]+\]");
    QTextCursor c = doc->find(re, cursor, QTextDocument::FindBackward);
    QTextCursor c2 = doc->find(re, cursor);

    //查找“参考文献”所在光标的位置
    QTextCursor ref_pos;
    QTextCursor ref_pos2;
    ref_pos2 = doc->find("参考文献", cursor);
//    while (!ref_pos.isNull())
//    {
//        ref_pos2 = ref_pos;
//        ref_pos = doc->find("参考文献", ref_pos);
//    }



    bool atfirst = false;   //标记插入位置是否是第一个引用之前
    if (c.isNull()) //没有上一个引用 则从1开始编号
    {
        thisnum = 1;
        atfirst = true;
        if (c2.isNull())    //第一次插入引用
        {
            cursor.insertText("[1]", fmt);



            cursor.movePosition(QTextCursor::End);

            cursor.insertText("\n\n参考文献\n[1]");
            cursor.insertText(str);

            refTable.append(ref_id);

            return;
        }
        else
        {
            cursor.insertText("[1]", fmt);
            refTable.insert(1, ref_id);
        }
    }
    else
    {
        QString id = c.selectedText();
        //qDebug() << id;

        id.replace("[", "");
        id.replace("]", "");
        //qDebug() << id;

        int num = id.toInt();
        thisnum = num;

        //qDebug() << num;

        QString str1;
        str1.sprintf("[%d]", num+1);


        cursor.insertText(str1, fmt);

        refTable.insert(num+1, ref_id);

    }

    //插入后 后面的引用全部加1

    c = doc->find(re, cursor);
    while (!c.isNull() && c < ref_pos2)
    {
        QString id = c.selectedText();

        id.replace("[", "");
        id.replace("]", "");

        int num = id.toInt();

        QString str1;
        str1.sprintf("[%d]", num+1);

        c.removeSelectedText();

        QTextCharFormat fmt2(cursor.charFormat());
        fmt2.setVerticalAlignment(QTextCharFormat::AlignSuperScript);


        c.insertText(str1, fmt2);

        c = doc->find(re, c);


    }


    //修改参考文献处

    QTextCursor refc2 = ref_pos2;


    //插入
    if (atfirst)
    {

        refc2.movePosition(QTextCursor::EndOfLine);

        refc2.insertText("\n[1] ");
        refc2.insertText(str);


    }
    else
    {
        QString tofind;

        qDebug() << "insert" << thisnum;
        tofind.sprintf("[%d]", thisnum);

        refc2 = doc->find(tofind, refc2);
        refc2.select(QTextCursor::BlockUnderCursor);
        QString t1 = refc2.selectedText();
        qDebug() << t1;

        refc2.movePosition(QTextCursor::EndOfBlock);

        QString toinsert;
        toinsert.sprintf("\n[%d]", thisnum+1);
        toinsert = toinsert + str;
        refc2.insertText(toinsert);

    }



    //递增
    QRegularExpression re2("\[[0-9]+\]");
    c = doc->find(re2, refc2);

    while (!c.isNull())
    {
        QString id = c.selectedText();
        //qDebug() << id;

        id.replace("[", "");
        id.replace("]", "");


        int num = id.toInt();


        QString str1;
        str1.sprintf("[%d]", num+1);

        c.removeSelectedText();


        c.insertText(str1);

        c = doc->find(re2, c);

    }


}

//跳转到上一个引用
void editWidget::slotPrevRef()
{
    QTextDocument * doc = textedit->document();
    QTextCursor cursor(textedit->textCursor());

    QRegularExpression re("\[[0-9]+\]");

    textedit->setTextCursor(doc->find(re, cursor, QTextDocument::FindBackward));

    qDebug() << "prev";
}
//跳转到下一个引用
void editWidget::slotNextRef()
{
    QTextDocument * doc = textedit->document();

    QTextCursor cursor(textedit->textCursor());


    QRegularExpression re("\[[0-9]+\]");


    textedit->setTextCursor(doc->find(re, cursor));


    qDebug() << "next";
}

//保存该笔记的文献表
void editWidget::saveRefTable()
{

    QString path = notepath;
    path.replace(".", ".2");

    QFile f2(path);
    f2.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out2(&f2);
    out2 << ref_num << endl;


    //存储文献索引表
    int n = refTable.length();
    for (int i = 1; i <= ref_num; ++i)
    {
        out2 << refTable[i] << endl;
    }
    out2.flush();
    f2.close();
}
//加载本笔记的文献表
void editWidget::loadRefTable()
{
    QString path = notepath;
    path.replace(".", ".2");



    refTable.clear();



    QFile f(path);
    if (f.open(QIODevice::ReadOnly|QIODevice::Text))
    {

        QTextStream in(&f);
        ref_num = in.readLine().toInt();
        refTable.append(ref_num);

        for (int i = 0; i < ref_num; ++i)
        {
            refTable.append(in.readLine().toInt());
        }
        f.close();
    }
    else
    {
        ref_num = 0;
        refTable.append(ref_num);
    }


}

//跳转到该处引用对应的文献处
void editWidget::slotSkipToRef()
{
    //获取光标处序号
    QTextDocument * doc = textedit->document();
    QTextCursor cursor(textedit->textCursor());

    QTextCursor c1 = doc->find("[", cursor,  QTextDocument::FindBackward);
    if (!c1.isNull())
    {

        //查找上一个引用的数字
        QRegularExpression re("\[[0-9]+\]");
        QTextCursor c2 = doc->find(re, c1);

        QString id = c2.selectedText();

        id.replace("[", "");
        id.replace("]", "");

        int num = id.toInt();

        qDebug() << "this is " << num;


        int n = refTable[num];

        emit signalSkipToRef(n);

    }


}


void editWidget::deleteRef()
{


    int thisnum;
    //qDebug() << str;

    //获取编辑区文档和光标 以供后面修改
    QTextDocument * doc = textedit->document();
    QTextCursor cursor(textedit->textCursor());



    //查找上一个引用的数字
    QRegularExpression re("\[[0-9]+\]");
    QTextCursor c = doc->find(re, cursor, QTextDocument::FindBackward);
    QTextCursor c2 = doc->find(re, cursor);

    //查找“参考文献”所在光标的位置
    QTextCursor ref_pos;
    QTextCursor ref_pos2;
    ref_pos2 = doc->find("参考文献", cursor);
//    while (!ref_pos.isNull())
//    {
//        ref_pos2 = ref_pos;
//        ref_pos = doc->find("参考文献", ref_pos);
//    }



    bool atfirst = false;   //标记插入位置是否是第一个引用之前
    if (c.isNull()) //没有上一个引用 则从1开始编号
    {
        return;
    }
    else
    {
        //本笔记引用数减1
        --ref_num;


        QString id = c.selectedText();
        //qDebug() << id;

        id.replace("[", "");
        id.replace("]", "");
        //qDebug() << id;

        int num = id.toInt();
        thisnum = num;

        c.removeSelectedText();

        refTable.remove(num);

    }

    //插入后 后面的引用全部减1

    c = doc->find(re, cursor);
    while (!c.isNull() && c < ref_pos2)
    {
        QString id = c.selectedText();

        id.replace("[", "");
        id.replace("]", "");

        int num = id.toInt();

        QString str1;
        str1.sprintf("[%d]", num-1);



        QTextCharFormat fmt2(c.charFormat());
        fmt2.setVerticalAlignment(QTextCharFormat::AlignSuperScript);

        c.removeSelectedText();

        c.insertText(str1, fmt2);

        c = doc->find(re, c);


    }


    //修改参考文献处

    QTextCursor refc2 = ref_pos2;



    QString tofind;


    tofind.sprintf("[%d]", thisnum);

    refc2 = doc->find(tofind, refc2);



    refc2.select(QTextCursor::BlockUnderCursor);

    refc2.removeSelectedText();









    //递减
    QRegularExpression re2("\[[0-9]+\]");
    c = doc->find(re2, refc2);

    while (!c.isNull())
    {
        QString id = c.selectedText();
        //qDebug() << id;

        id.replace("[", "");
        id.replace("]", "");


        int num = id.toInt();


        QString str1;
        str1.sprintf("[%d]", num-1);

        c.removeSelectedText();


        c.insertText(str1);

        c = doc->find(re2, c);

    }

    if (ref_num == 0)
    {
        ref_pos2.removeSelectedText();
    }

}
