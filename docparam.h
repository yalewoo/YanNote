#ifndef DOCPARAM_H
#define DOCPARAM_H

#include <QString>

struct Docparam{
    int type;   //文献位置 0表示本地文件 1表示url
    QString fullpath;
    QString url;
    QString name;
    QString author;
    QString pub;
    QString year;
};

#endif // DOCPARAM_H
