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


editWidget::editWidget(QWidget *parent)
{
    ref_num = 0;
    refTable.append(ref_num);


    toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(25, 25));
    textedit = new textEdit(this);

    saveAction = new QAction(QIcon(":/ico/ico/save.ico"), "保存", this);
    saveAction->setShortcut(QKeySequence::Save);
    toolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(slotSave()));


    //字体
    //label1 = new QLabel( tr( "" ) );
    fontBox = new QFontComboBox();
    fontBox->setFontFilters( QFontComboBox::ScalableFonts );
    //toolBar->addWidget( label1 );
    toolBar->addWidget( fontBox );

    //字号
    //label2 = new QLabel( tr( "" ) );
    sizeBox = new QComboBox();
    //toolBar->addWidget( label2 );
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

    italicBtn = new QToolButton();
    italicBtn->setIcon( QIcon( ":/ico/ico/format_text_italic.ico" ) );
    italicBtn->setCheckable( true );
    toolBar->addWidget( italicBtn );

    underlineBtn = new QToolButton();
    underlineBtn->setIcon( QIcon( ":/ico/ico/format_text_underline.ico" ) );
    underlineBtn->setCheckable( true );
    toolBar->addWidget( underlineBtn );

    toolBar->addSeparator();
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



    prevRefAction = new QAction(QIcon(":/ico/ico/prevref.png"), "上一个引用", this);
    toolBar->addAction(prevRefAction);
    connect(prevRefAction, SIGNAL(triggered()), this, SLOT(slotPrevRef()));

    nextRefAction = new QAction(QIcon(":/ico/ico/nextref.png"), "上一个引用", this);
    toolBar->addAction(nextRefAction);
    connect(nextRefAction, SIGNAL(triggered()), this, SLOT(slotNextRef()));




    //设置布局 工具栏在上 编辑器在下
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(toolBar);
    layout->addWidget(textedit);
    this->setLayout(layout);
}

editWidget::~editWidget()
{

}


void editWidget::slotSave()
{
    QFile file(name);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "ERROR", "Can't save the file", QMessageBox::Yes);
        return;
    }

    QTextStream out(&file);

    out << textedit->toHtml();

    textedit->document()->setModified(false);

    file.close();

    saveRefTable();
}

void editWidget::loadText(QString str)
{
    name = str;
    QFile openFile(str);

    if (!openFile.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&openFile);
    textedit->setText(in.readAll());
    openFile.close();

    loadRefTable();
}



void editWidget::slotFont(QString f)
{
       QTextCharFormat fmt;
       fmt.setFontFamily(f);
       mergeFormat(fmt);
}

void editWidget::slotSize(QString num)
{
       QTextCharFormat fmt;
       fmt.setFontPointSize(num.toFloat());
       mergeFormat(fmt);
}

void editWidget::slotBold()
{
       QTextCharFormat fmt;
       fmt.setFontWeight(boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
       mergeFormat(fmt);
}

void editWidget::slotItalic()
{
       QTextCharFormat fmt;
       fmt.setFontItalic(italicBtn->isChecked());
       mergeFormat(fmt);
}

void editWidget::slotUnder()
{
       QTextCharFormat fmt;
       fmt.setFontUnderline(underlineBtn->isChecked());
       mergeFormat(fmt);
}

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

void editWidget::slotInsertReference(QString str, int ref_id)
{
    ++ref_num;
    int thisnum;
    qDebug() << str;

    QTextDocument * doc = textedit->document();
    QTextCursor cursor(textedit->textCursor());

    //设置格式为上标
    QTextCharFormat fmt(cursor.charFormat());
    fmt.setVerticalAlignment(QTextCharFormat::AlignSuperScript);

    //查找上一个引用的数字
    QRegularExpression re("\[[0-9]+\]");
    QTextCursor c = doc->find(re, cursor, QTextDocument::FindBackward);
    QTextCursor c2 = doc->find(re, cursor);


    QTextCursor ref_pos;
    QTextCursor ref_pos2;
    ref_pos = doc->find("参考文献", cursor);
    while (!ref_pos.isNull())
    {
        ref_pos2 = ref_pos;
        ref_pos = doc->find("参考文献", ref_pos);
    }



    bool atfirst = false;
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
            cursor.insertText("[1]", fmt);
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

        refc2.movePosition(QTextCursor::EndOfLine);

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


void editWidget::slotPrevRef()
{
    QTextDocument * doc = textedit->document();
    QTextCursor cursor(textedit->textCursor());

    QRegularExpression re("\[[0-9]+\]");

    textedit->setTextCursor(doc->find(re, cursor, QTextDocument::FindBackward));

    qDebug() << "prev";
}



void editWidget::slotNextRef()
{
    QTextDocument * doc = textedit->document();

    QTextCursor cursor(textedit->textCursor());


    QRegularExpression re("\[[0-9]+\]");


    textedit->setTextCursor(doc->find(re, cursor));


    qDebug() << "next";
}


void editWidget::saveRefTable()
{

    QString path = name;
    path.replace(".", "2.");

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
void editWidget::loadRefTable()
{
    QString path = name;
    path.replace(".", "2.");



    refTable.clear();




    refTable.append(ref_num);


    QFile f(path);
    if (f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&f);
        ref_num = in.readLine().toInt();

        for (int i = 0; i < ref_num; ++i)
        {
            refTable.append(in.readLine().toInt());
        }
        f.close();
    }


}
