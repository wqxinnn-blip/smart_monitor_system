#ifndef FILE_H
#define FILE_H
#include <QString>
#include <QDir>
#include <QDate>
#include "../model/settinginfo.h"
#include <QCoreApplication>

//配置文件名
// 0-回放普通截图
// 1-自动截取的异常图片
// 2-手动异常截图
class File
{
private:
    static int num[4];
public:
    File();
    static QString getShotFileName(int chId,int type,QString ch,QString videoName);
    static QString getFullPath();
    static QString getFrameType(QString imgName);
    static QString getVideoPathByFrameType(QString imgPath);
};

#endif // FILE_H
